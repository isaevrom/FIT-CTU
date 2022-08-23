#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
mutex g_mtxCout;
void PrintOut(const string& str) {
    lock_guard<mutex> lk(g_mtxCout);
    cout << str << endl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class CLineWrapper {
 public:
    explicit CLineWrapper(int lineId, AProductionLine line)
        : m_lineId(lineId),
          m_nextSheetId(0),
          m_line(std::move(line))
    {
    }

    int m_lineId;
    int m_nextSheetId;
    AProductionLine m_line;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class CSheetWrapper {
 public:
    explicit CSheetWrapper(int sheetOrderId, int lineProducedId, ASheet sheet)
        : m_sheetOrderId(sheetOrderId),
          m_lineProducedId(lineProducedId),
          m_sheet(std::move(sheet))
    {
    }

    int m_sheetOrderId;
    int m_lineProducedId;
    ASheet m_sheet;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

class CompareSheets {
public:
    bool operator()( CSheetWrapper &a, CSheetWrapper &b)
    {
        return a.m_sheetOrderId > b.m_sheetOrderId;
    }
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

class CQualityControl {
public:
    static void checkAlgorithm(ASheet sheet);

    ~CQualityControl() {
        delete [] m_condVarsSend;
        delete [] m_mtxsSend;
    }

    void addLine(AProductionLine line);

    void work(const int& workerId);

    void receiveHandler(const int& lineId);

    void sendHandler(const int& lineId);

    void start(int workThreads);

    void stop(void);
private:
    int m_nextLineId = 0;
    vector<CLineWrapper> m_vWrappedLines;
    queue<CSheetWrapper> m_qWrappedSheets;

    vector<thread> m_workers;
    vector<thread> m_adders;
    vector<thread> m_senders;

    vector<priority_queue<CSheetWrapper, vector<CSheetWrapper>, CompareSheets > > m_wrappedSheetsQueues;

    mutex m_mtxQueue;
    condition_variable m_condForQueue;
    condition_variable *m_condVarsSend;
    mutex *m_mtxsSend;
    
    int m_sendHandlersEnded = 0;
    int m_workersEnded = 0;
    int m_receiversEnded = 0;
};

void CQualityControl::checkAlgorithm(ASheet sheet) {
    int thicknessIndex = 0;
    int** thickness2d = new int*[sheet->m_Length];
    for (int i = 0; i < sheet->m_Length; ++i) {
        thickness2d[i] = new int[sheet->m_Width];
        for (int j = 0; j < sheet->m_Width; j++) {
            thickness2d[i][j] = sheet->m_Thickness[thicknessIndex++];
        }
    }

    int area = 0;
    for (auto &a: sheet->m_RelDev) {
        area = maxRectByRelDev(thickness2d, sheet->m_Width,
                               sheet->m_Length, a.first);
        sheet->updateRelDev(a.first, area);
    }
    for (auto &a: sheet->m_MinMax) {
        area = maxRectByMinMax(thickness2d, sheet->m_Width,
                               sheet->m_Length, a.first.m_Lo, a.first.m_Hi);
        sheet->updateMinMax(a.first, area);
    }
    for (auto &a: sheet->m_Volume) {
        area = maxRectByVolume(thickness2d, sheet->m_Width, sheet->m_Length, a.first);
        sheet->updateVolume(a.first, area);
    }

    for(int i = 0; i < sheet->m_Length; ++i) {
        delete [] thickness2d[i];
    }
    delete [] thickness2d;
}

//** using AProductionLine = std::shared_ptr<CProductionLine>; **//
void CQualityControl::addLine(AProductionLine line) {
    PrintOut("#" + to_string(m_nextLineId) + " line added.");
    m_vWrappedLines.emplace_back(m_nextLineId++, std::move(line));
}

void CQualityControl::work(const int& workerId) {
    PrintOut("\nwork");

    while(true) {
        PrintOut("sendHandlersEnded == m_nextLineId ....." + to_string(m_sendHandlersEnded) + " == "
                  + to_string(m_nextLineId));
        PrintOut("Ended:  W-" + to_string(m_workersEnded) + ", R-" + to_string(m_receiversEnded)
                         + ", S-" + to_string(m_sendHandlersEnded));

        unique_lock lk(m_mtxQueue);
        m_condForQueue.wait(lk, [this, &workerId] {
            PrintOut("\nWorker is #" + to_string(workerId) +
                         " waiting for a queue to become non-empty... " + to_string(m_qWrappedSheets.size()) + "\n"
                         + ", meanwhile ended sendThreads == " + to_string(m_sendHandlersEnded)
                         + " and queue size is == " + to_string(m_qWrappedSheets.size())
                         );
            return !m_qWrappedSheets.empty() || m_sendHandlersEnded == m_nextLineId;
        });
        if (m_sendHandlersEnded == m_nextLineId) {
            PrintOut("Worker #" + to_string(workerId) + " ENDING WORK");
            ++m_workersEnded;
            break;
        }
        PrintOut("\nWorker #" + to_string(workerId) + " wakes up!!!\n");
        CSheetWrapper wrpSheet = m_qWrappedSheets.front();
        m_qWrappedSheets.pop();
        lk.unlock();

        PrintOut("Processing sheet #" + to_string(wrpSheet.m_sheetOrderId));

        if (wrpSheet.m_sheet != nullptr) {
            // convert thickness vector to 2D array
            int thicknessIndex = 0;
            int** thickness2d = new int*[wrpSheet.m_sheet->m_Length];
            for (int i = 0; i < wrpSheet.m_sheet->m_Length; ++i) {
                thickness2d[i] = new int[wrpSheet.m_sheet->m_Width];
                for (int j = 0; j < wrpSheet.m_sheet->m_Width; j++) {
                    thickness2d[i][j] = wrpSheet.m_sheet->m_Thickness[thicknessIndex++];
                }
            }

            // calculations
            int area = 0;
            for (auto &a: wrpSheet.m_sheet->m_RelDev) {
                area = maxRectByRelDev(thickness2d, wrpSheet.m_sheet->m_Width,
                                wrpSheet.m_sheet->m_Length, a.first);
                wrpSheet.m_sheet->updateRelDev(a.first, area);
            }
            for (auto &a: wrpSheet.m_sheet->m_MinMax) {
                area = maxRectByMinMax(thickness2d, wrpSheet.m_sheet->m_Width,
                                       wrpSheet.m_sheet->m_Length, a.first.m_Lo, a.first.m_Hi);
                wrpSheet.m_sheet->updateMinMax(a.first, area);
            }
            for (auto &a: wrpSheet.m_sheet->m_Volume) {
                area = maxRectByVolume(thickness2d, wrpSheet.m_sheet->m_Width,
                                       wrpSheet.m_sheet->m_Length, a.first);
                wrpSheet.m_sheet->updateVolume(a.first, area);
            }

            for(int i = 0; i < wrpSheet.m_sheet->m_Length; ++i) {
                delete [] thickness2d[i];
            }
            delete [] thickness2d;
        }
        // send data to proper priority queue in m_wrappedSheetQueues vector
        unique_lock lk_send(m_mtxsSend[wrpSheet.m_lineProducedId]);
        m_wrappedSheetsQueues[wrpSheet.m_lineProducedId].push(wrpSheet);
        PrintOut("TOP: L" + to_string(m_wrappedSheetsQueues[wrpSheet.m_lineProducedId].top().m_lineProducedId)
                          + ", S" + to_string(m_wrappedSheetsQueues[wrpSheet.m_lineProducedId].top().m_sheetOrderId));
        PrintOut("-- queue[" + to_string(wrpSheet.m_lineProducedId) + "] size == "
         + to_string(m_wrappedSheetsQueues[wrpSheet.m_lineProducedId].size()));

        m_condVarsSend[wrpSheet.m_lineProducedId].notify_one();
        lk_send.unlock();

        if (wrpSheet.m_sheet == nullptr)
            PrintOut("AAAHAHAHAHAHAHAHAHAHAHAH !! ! !! !");
    }
}

void CQualityControl::receiveHandler(const int& lineId) {
    PrintOut("\nreceiveHandler");
    CLineWrapper& wrpLine = m_vWrappedLines[lineId];

    ASheet p_sheet = nullptr;

    while(true) {
        p_sheet = wrpLine.m_line->getSheet();
        PrintOut("L" + to_string(lineId) + ", sheet #" + to_string(wrpLine.m_nextSheetId) + " received.");
        if (p_sheet == nullptr) {
            PrintOut("Line number " + to_string(lineId) + " received nullptr sheet #" + to_string(wrpLine.m_nextSheetId)
            + ". What do?");
            unique_lock lk(m_mtxQueue); // locked in order to enqueue a sheet
            m_qWrappedSheets.emplace(wrpLine.m_nextSheetId++, wrpLine.m_lineId, p_sheet);
            lk.unlock();

            m_condForQueue.notify_one();
            PrintOut("receiveHandler: BREAKING @@@@@@@@@@@@@@@@@@@@@@@@");
            ++m_receiversEnded;
            break;
        }
        unique_lock lk(m_mtxQueue); // locked in order to enqueue a sheet
        m_qWrappedSheets.emplace(wrpLine.m_nextSheetId++, wrpLine.m_lineId, p_sheet);
        lk.unlock();
        m_condForQueue.notify_one();
        PrintOut("-- worker notified, queue size == " + to_string(m_qWrappedSheets.size()));
    }
}

void CQualityControl::sendHandler(const int& lineId) {
    PrintOut("\nsendHandler");

    int nextSheetId = 0;
    vector<int> vecDone;

    while(true) {
        unique_lock lk_send(m_mtxsSend[lineId]);
        m_condVarsSend[lineId].wait(lk_send, [this, &nextSheetId, &lineId] {
            if ((int)m_wrappedSheetsQueues.size() <= lineId)
                return false;
            if (m_wrappedSheetsQueues[lineId].empty())
                return false;
            if (m_wrappedSheetsQueues[lineId].top().m_sheetOrderId != nextSheetId)
                return false;
            return true;
        });

        PrintOut("attempting to access m_wrappedSheetsQueues (size " + to_string(m_wrappedSheetsQueues.size())
             + ")" + " with lineId == " + to_string(lineId));
        PrintOut("TOP_id = " + to_string(m_wrappedSheetsQueues[lineId].top().m_sheetOrderId));
        CSheetWrapper wrpSheet = m_wrappedSheetsQueues[lineId].top();

        m_wrappedSheetsQueues[lineId].pop();
        lk_send.unlock();
        if (wrpSheet.m_sheet == nullptr) {
            PrintOut("\nsendHandler: NULLPTR ");
            PrintOut("-- nullptr sheet id = " + to_string(wrpSheet.m_sheetOrderId));
            ++m_sendHandlersEnded;
            PrintOut("sendHandlersEnded == " + to_string(m_sendHandlersEnded));

            PrintOut("sendHandler: B R E A K @@@@@@@@@@@@@@@@@@@@@@@@\n");
            m_condForQueue.notify_all();
            break;
        }
        m_vWrappedLines[lineId].m_line->doneSheet(wrpSheet.m_sheet);
        vecDone.push_back(wrpSheet.m_sheetOrderId);
        for (auto &a : vecDone) {
            PrintOut(to_string(a) + ", ");
        }
        PrintOut("");
        ++nextSheetId;
    }
}

void CQualityControl::start(int workThreads) {
    PrintOut("\nstart");

    m_condVarsSend = new condition_variable [m_nextLineId];
    m_mtxsSend     = new mutex [m_nextLineId];
    for (int i = 0; i < m_nextLineId; i++)
        m_wrappedSheetsQueues.emplace_back();

    for (int i = 0; i < workThreads; i++)
        m_workers.emplace_back(&CQualityControl::work, this, i);
    PrintOut(to_string(workThreads) + " workers started.");

    for (auto &line: m_vWrappedLines) {
        m_adders.emplace_back(&CQualityControl::receiveHandler, this, line.m_lineId);
        m_senders.emplace_back(&CQualityControl::sendHandler, this, line.m_lineId);
    }
    PrintOut(to_string(m_nextLineId) + "x2" + " communicators started.");
}

void CQualityControl::stop(void) {
    PrintOut("\nstop");

    for (auto &t: m_workers)
        if (t.joinable())
            t.join();
        else
            cout << "can't join a worker thread" << endl;
    for (auto &t: m_adders)
        if (t.joinable())
            t.join();
        else
            cout << "can't join an adder thread" << endl;
    for (auto &t: m_senders)
        if (t.joinable())
            t.join();
        else
            cout << "can't join a sender thread" << endl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__

int main() {
    CQualityControl control;
    AProductionLineTest line0 = std::make_shared<CProductionLineTest>();
    control.addLine(line0);
    AProductionLineTest line1 = std::make_shared<CProductionLineTest>();
    control.addLine(line1);
    control.start(4);
    control.stop();
    if (!line0->allProcessed())
        throw std::logic_error("(some) sheets were not correctly processsed");
    if (!line1->allProcessed())
        throw std::logic_error("(some) sheets were not correctly processsed");
    PrintOut("\n\n\n\n\n");

    CQualityControl control2;
    AProductionLineTest line2 = std::make_shared<CProductionLineTest>();
    control2.addLine(line2);
    control2.start(4);
    control2.stop();
    if (!line2->allProcessed())
        throw std::logic_error("(some) sheets were not correctly processsed");



    return 0;
}
#endif /* __PROGTEST__ */
