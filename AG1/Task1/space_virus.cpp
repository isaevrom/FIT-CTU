#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <stack>
#include <list>
#include <climits>

using namespace std;

class Planets {
public:
    int s;
    int numPlanets    , numRoutes,
        startPlanet   , endPlanet, stepsCount,
        numSickPlanets, numMedPlanets;
    vector<bool> sickPlanets, medPlanets;
    vector< vector<int> >  adj;  // vector of adjacency
    vector< vector<int> > parents;
    vector<int> lives;
    
    Planets();                   // constructor
    
    void addRoute(int v, int w); // method for adding read routes between 2 given planets
    void sickBFS();              // BFS algorithm
    void BFS();                  // no sick planets
    void readSickPlanets();      // mark sick planets
    void readMedPlanets();       // mark med  planets
    void readRoutes();           // method for reading routes between 2 given planets
};

Planets::Planets() {
    cin >> numPlanets  >> numRoutes;                // N, M
    adj.resize(numPlanets);                         // resizing the outer vector
    parents.resize(numPlanets);
    lives.resize(numPlanets, INT_MIN );
    cin >> startPlanet >> endPlanet >> stepsCount;  // s, t, l
    cin >> numSickPlanets;                          // K
    if ( numSickPlanets > 0 )
        readSickPlanets();
    cin >> numMedPlanets;                           // D
    if ( numMedPlanets  > 0 )
        readMedPlanets();
    if ( numRoutes > 0 )
        readRoutes();
}

void Planets::readSickPlanets() {
    int tmp;
    // first  - set all planets as not sick
    sickPlanets.resize(numPlanets, false);
    // second - mark sick planets
    for ( int i = 0; i < numSickPlanets; i++ ) {
        cin >> tmp;
        sickPlanets[tmp] = true;
    }
}

void Planets::readMedPlanets() {
    int tmp;
    // first  - set all planets as not medicated
    sickPlanets.resize(numPlanets, false);
    // second - mark medicated planets
    for ( int i = 0; i < numMedPlanets; i++ ) {
        cin >> tmp;
        medPlanets[tmp] = true;
    }
}

void Planets::readRoutes() {
    int t, p;
    for ( int i = 0; i < numRoutes; i++ ) {
        cin >> t >> p;
        // create route between planets T and P
        addRoute(t, p);
    }
}

void Planets::addRoute(int v, int w) {
    adj[v].push_back(w);    // add w to v’s adjacency ... v_adj = {..., w}
    adj[w].push_back(v);    // add v to w’s adjacency ... w_adj = {..., v}
}
  
void Planets::sickBFS() {
    vector<bool> visited(numPlanets, false);
    list<int> queue;
    int currentPlanet;
    
    queue.push_back(endPlanet);
    visited[endPlanet] = true;
    
    lives[endPlanet] = stepsCount;
    
    list<int>::iterator i;
    
    while ( !queue.empty() ) {
        currentPlanet = queue.front();
        queue.pop_front();
        
        for ( auto i = adj[currentPlanet].begin(); i != adj[currentPlanet].end(); i++ ) {
            if ( lives[currentPlanet] > lives[*i] && (!sickPlanets[*i] || lives[currentPlanet] > 0) ) {
                    parents[*i].push_back(currentPlanet);
                    lives[*i] = lives[currentPlanet] - 1;
                if ( !visited[*i] ) {
                    visited[*i] = true;
                    queue.push_back(*i);
                }
            }
        }
    }
    
    if ( !(parents[startPlanet].empty()) ) {
        int h = startPlanet;
        int tmp = INT_MIN;
        
        while ( 1 ) {
            if ( tmp != INT_MIN )
                parents[tmp].pop_back();
            if ( h == endPlanet ) {
                cout << h << endl;
                break;
            }
            cout << h << " ";
            tmp = h;
            h = parents[h].back();
        }
    } else {
        cout << "-1" << endl;
    }
}

void Planets::BFS() {
    vector<bool> visited(numPlanets, false);
    list<int> queue;
    int currentPlanet;
    
    queue.push_back(endPlanet);
    visited[endPlanet] = true;
    
    lives[endPlanet] = stepsCount;
    
    list<int>::iterator i;
    
    while ( !queue.empty() ) {
        currentPlanet = queue.front();
        queue.pop_front();
        
        for ( auto i = adj[currentPlanet].begin(); i != adj[currentPlanet].end(); i++ ) {
            if ( !visited[*i] ) {
                visited[*i] = true;
                queue.push_back(*i);
            }
            
            if ( lives[currentPlanet] > lives[*i] ) {
                    parents[*i].push_back(currentPlanet);
                    lives[*i] = lives[currentPlanet] - 1;
            }
        }
    }
 
    if ( !(parents[startPlanet].empty()) ) {
        int h = startPlanet;
        int tmp = INT_MIN;
        
        while ( 1 ) {
            if ( tmp != INT_MIN )
                parents[tmp].pop_back();
            if ( h == endPlanet ) {
                cout << h << endl;
                break;
            }
            cout << h << " ";
            tmp = h;
            h = parents[h].back();
        }
    } else {
        cout << "-1" << endl;
    }
}

int main() {
    Planets planets;
    if ( planets.numSickPlanets > 0 )
        planets.sickBFS();
    if ( planets.numSickPlanets == 0 )
        planets.BFS();
    
    
    return 0;
}

