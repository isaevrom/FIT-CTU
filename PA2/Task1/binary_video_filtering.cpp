#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const uint32_t BIG_ENDIAN_ID    = 0x4d6d6d4d;
const uint32_t LITTLE_ENDIAN_ID = 0x49696949;
const uint32_t FRAME_ID         = 0x46727246;
const uint32_t VIDEO_DATA_ID    = 0x56696956;
const uint32_t AUDIO_LIST_ID    = 0x416c6c41;
const uint32_t AUDIO_DATA_ID    = 0x41757541;
#endif /* __PROGTEST__ */

struct TGlobal {
    uint32_t ID;
    uint32_t framesAmount;
    uint32_t payloadLen;
    //uint32_t CRC;
};

struct TFrame {
    uint32_t ID;
    uint32_t payloadLen;
    //uint32_t CRC;
};

struct TVideoData {
    uint32_t ID;
    uint32_t payloadLen;
    //uint32_t CRC;
};

struct TAudioList {
    uint32_t ID;
    uint32_t audioDataAmount;
    uint32_t payloadLen;
    //uint32_t CRC;
};

struct TAudioData {
    uint32_t ID;
    uint8_t  lang[2];
    uint32_t payloadLen;
    //uint32_t CRC;
};

struct TPayloadLenPosition {
    // streampos MAY NEED TO BE CHANGED TO uint32_t IN ORDER TO SAVE SOME BYTES
    streampos globalHeader,
              frame,
              audioList;
};

struct TNewPayloadLen {
    // streampos MAY NEED TO BE CHANGED TO uint32_t IN ORDER TO SAVE SOME BYTES
    uint32_t  globalHeader,
              frame,
              audioList;
};

void convertEndian( uint32_t &val  ) {
    // 78 56 34 12 --> 12 34 56 78
    uint32_t  b[4];
    b[0] = ((val << 24u) & 0xff000000);
    b[1] = ((val << 8u)  & 0x00ff0000);
    b[2] = ((val >> 8u)  & 0x0000ff00);
    b[3] = ((val >> 24u) & 0x000000ff);
    
    val = b[0] | b[1] | b[2] | b[3];
    cout << "AFTER ENDIAN CONVERSION: " << val << endl;
}

bool readSkipper ( ifstream &is, uint32_t len ) {
    uint8_t tmp;
    for ( uint32_t i = 0; i < len; i++ )
        if ( ! is.read( (char*)&tmp, sizeof(tmp)) ) { cout << "Cpy  .in fail!" << endl; return false; }
    
    //return is;
    return true;
}

bool cpy(ifstream &is, ofstream &os, uint32_t len)
{
    uint8_t tmp;
    for ( uint32_t i = 0; i < len; i++ )
    {
      if ( ! is.read( (char*)&tmp, sizeof(tmp)) ) { cout << "Cpy  .in fail!" << endl; return false; }
      if ( ! os.write((char*)&tmp, sizeof(tmp)) ) { cout << "Cpy .out fail!" << endl; return false; }
    }
    //return is && os;
    return true;
}

uint32_t endian;
bool readUint32_t( uint32_t &r_tmp, ifstream &m_inFile )
{
    if ( ! m_inFile.read((char*)&r_tmp, sizeof(r_tmp)) )   { cout << "Unsuccessful attempt to read uint32_t!" << endl; return false; }
    //if (endian == BIG_ENDIAN_ID) convertEndian(r_tmp);
    //return m_inFile;
    return true;
}

bool writeUint32_t ( uint32_t &w_tmp, ofstream &m_outFile )
{
    if ( ! m_outFile.write((char*)&w_tmp, sizeof(w_tmp)) ) { cout << "Unsuccessful attempt to write uint32_t!" << endl; return false; }
    //return m_outFile;
    return true;
}


bool  filterFile ( const char* srcFileName, const char* dstFileName, const char* lang )
{
    ifstream inFile ( srcFileName, ios::binary|ios::in );
    ofstream outFile( dstFileName, ios::binary|ios::out );
    if ( !inFile || !outFile ) { cout << "Some of the files could not be opened" << endl; return false; }
    
//  GLOBAL PART:
    TGlobal Global;
    if ( ! inFile.read( (char*)&Global, sizeof(Global) ) ) { cout << "Unsuccessful Tglobal reading" << endl; return false; }
    if ( Global.ID == BIG_ENDIAN_ID ) {
        endian = BIG_ENDIAN_ID;
    } else if ( Global.ID == LITTLE_ENDIAN_ID ) {
        endian = LITTLE_ENDIAN_ID;
    } else {
        cout << "Global ID mismatch!" << endl;
        return false;
    }
    if ( ! outFile.write( (char*)&Global, sizeof(Global) ) ) { cout << "Unsuccessful Tglobal writing" << endl; return false; }
    if ( endian == BIG_ENDIAN_ID ) {
        convertEndian( Global.framesAmount );
        convertEndian( Global.payloadLen );
    }
    cout << "GLOBAL PAYLOAD: " << Global.payloadLen << endl;
    
    
    
//  FRAME PART:
    TFrame Frame;
    TVideoData VideoData;
    TAudioList AudioList;
    TAudioData AudioData;
    uint32_t NULLED_CRC = 0x00000000;
    uint32_t ORIGINAL_CRC;
    bool langMatch = false;
    TNewPayloadLen NewPayloadLen = {0,0,0};
    streampos currentPos;
    uint32_t audioDataWritten = 0;
    // Expecting to process Global.framesAmount many frames
    for ( uint32_t i = 0; i < Global.framesAmount; i++ )
    {
        // < Frame attributes cpy
        if ( ! inFile.read(   (char*)&Frame, sizeof(Frame) ) ) { cout << "Unsuccessful Tframe reading" << endl; return false; }
        if ( Frame.ID != FRAME_ID ) { cout << "Frame ID mismatch!" << " Got: " << Frame.ID << endl; return false;}
        if ( ! outFile.write( (char*)&Frame, sizeof(Frame) ) ) { cout << "Unsuccessful Tframe writing" << endl; return false; }
        if ( endian == BIG_ENDIAN_ID )
            convertEndian( Frame.payloadLen );
        Global.payloadLen -= 8;
        //   Frame attributes cpy >
//  VIDEO PART
        // < Video attributes cpy
        if ( ! inFile.read(   (char*)&VideoData, sizeof(VideoData) ) ) { cout << "Unsuccessful Tvideo reading" << endl; return false; }
        if ( VideoData.ID != VIDEO_DATA_ID ) { cout << "VideoData ID mismatch!" << endl; return false;}
        if ( ! outFile.write( (char*)&VideoData, sizeof(VideoData) ) ) { cout << "Unsuccessful Tvideo writing" << endl; return false; }
        if ( endian == BIG_ENDIAN_ID )
            convertEndian( VideoData.payloadLen );
        if ( ! cpy( inFile, outFile, VideoData.payloadLen ) ) { cout << "VideoData payload reading unsuccessful!" << endl; return false; }
        Frame.payloadLen  -= (8 + (int)VideoData.payloadLen);
        Global.payloadLen -= (8 + (int)VideoData.payloadLen);
        //   Video attributes cpy >
        // < Video CRC
        if ( ! inFile.read(   (char*)&ORIGINAL_CRC, sizeof(ORIGINAL_CRC) ) ) { cout << "Unsuccessful VIDEO CRC reading" << endl; return false; }
        if ( ! outFile.write( (char*)&NULLED_CRC,   sizeof(NULLED_CRC  ) ) ) { cout << "Unsuccessful VIDEO CRC writing" << endl; return false; }
        Frame.payloadLen  -= 4;
        Global.payloadLen -= 4;
        //   Video CRC >
//  AUDIOLIST PART
        // < AudioList attributes cpy
        if ( ! inFile.read(   (char*)&AudioList, sizeof(AudioList) ) ) { cout << "Unsuccessful Taudiolist reading" << endl; return false; }
        if ( AudioList.ID != AUDIO_LIST_ID ) { cout << "AudioList ID mismatch!" << endl; return false;}
        if ( ! outFile.write( (char*)&AudioList, sizeof(AudioList) ) ) { cout << "Unsuccessful Taudiolist writing" << endl; return false; }
        Frame.payloadLen  -= 12;
        Global.payloadLen -= 12;
        if ( endian == BIG_ENDIAN_ID ) {
            convertEndian( AudioList.audioDataAmount );
            cout << "AudioList.audioDataAmount == " << AudioList.audioDataAmount << endl;
            convertEndian( AudioList.payloadLen );
            cout << "AudioList.payloadLen == " << AudioList.payloadLen  << endl;
        }
        //   AudioList attributes cpy >
//  AUDIODATA PART
        NewPayloadLen.audioList = 0; // nastavim na 0, protoze nevim, zda audiolist nebude treba prazdny po odfiltrovani
        audioDataWritten = 0; // nastavim na 0, pokud se ale uspesne nacte nejaky audioData, tak se nastavi na 1 uvnitr for'u
        for ( uint32_t j = 0; j < AudioList.audioDataAmount; j++ )
        {
            /*
                Keep reading, until you match a language.
                If you match the language, then write out the data which follow it:
                @ AUDIODATA ID, PAYLOADLEN, PAYLOAD, (NULLED) CRC
             */
            if ( ! readUint32_t( AudioData.ID, inFile ) ) { cout << "Unsuccessful AudioData ID reading!" << endl; return false; }
            if ( AudioData.ID != AUDIO_DATA_ID ) { cout << "AudioData ID mismatch!" << endl; return false; }
            if ( inFile.read( (char*)&AudioData.lang, sizeof(AudioData.lang) ).gcount() != 2*sizeof(uint8_t) ) { cout << "Unsuccessful language reading" << endl; return false; }
            if ( ! readUint32_t( AudioData.payloadLen, inFile ) ) { cout << "Audiodata payloadLen reading unsuccessful!" << endl; return false; }
            cout << "AUDIODATA.PAYLOADLEN == " << AudioData.payloadLen << endl;
            if ( endian == BIG_ENDIAN_ID )
                convertEndian( AudioData.payloadLen );
            cout << "AUDIODATA.PAYLOADLEN == " << AudioData.payloadLen << endl;

            AudioList.payloadLen -= (4 + 2 + 4);
            Frame.payloadLen     -= (4 + 2 + 4);
            Global.payloadLen    -= (4 + 2 + 4);
            // BEFORE FRAME PROCESSING BEGINS, @langMatch IS SET TO BE false
            // AUDIODATA ATTRIBUTES WILL BE WRITTEN OUT ONLY ONCE, AS ALL THE AUDIODATA LANGUAGES ARE DISTINCT
            // AND WRITE OPERATION OCCURS EXCLUSIVELY DURING THE FIRST MATCH, IF EVER
            // ... IF LANGUAGES NEVER MATCH, NO AUDIODATA ATTRIBUTES ARE WRITTEN OUT, NOT EVEN CRC
            if ( AudioData.lang[0] == lang[0] && AudioData.lang[1] == lang[1] ) { langMatch = true; }
            if ( langMatch ) {
                cout << "Languages match!" << endl;
                // ALREADY READ:      @ AUDIODATA ID, LANG, PAYLOADLEN
                // TO BE READ:        @ PAYLOAD, (NULLED) CRC
                // TO BE WRITTEN OUT: @ EVERYTHING LISTED ABOVE
                //if ( ! outFile.write( (char*)&AudioData, sizeof(AudioData) ) ) { cout << "Unsuccessful Taudiodata writing" << endl; return false; }
                
                if ( ! writeUint32_t( AudioData.ID, outFile ) ) { cout << "Unsuccessful AudioData ID writing!" << endl; return false; }
                if ( ! outFile.write( (char*)&AudioData.lang, sizeof(AudioData.lang) ) ) { cout << "Unsuccessful language writing" << endl; return false; }
                if ( endian == BIG_ENDIAN_ID )
                    convertEndian( AudioData.payloadLen );
                if ( ! writeUint32_t( AudioData.payloadLen, outFile ) ) { cout << "Audiodata payloadLen writing unsuccessful!" << endl; return false;}
                if ( endian == BIG_ENDIAN_ID )
                    convertEndian( AudioData.payloadLen );
                if ( ! cpy( inFile, outFile, AudioData.payloadLen ) ) { cout << "Audiodata payload writing unsuccessful!" << endl; return false; }
                // < AudioData CRC  !!! CAN BE DONE EASIER USING CPY AND SETTER OF TMP UINT32_T VARIABLE TO 0
                if ( ! inFile.read(   (char*)&ORIGINAL_CRC, sizeof(ORIGINAL_CRC) ) ) { cout << "Unsuccessful AUDIODATA CRC reading" << endl; return false; }
                if ( ! outFile.write( (char*)&NULLED_CRC,   sizeof(NULLED_CRC  ) ) ) { cout << "Unsuccessful AUDIODATA CRC writing" << endl; return false; }
                //   AudioData CRC >
                langMatch = false;
                NewPayloadLen.audioList = 4 + 2 + 4 + AudioData.payloadLen + 4;
                audioDataWritten = 1;
                AudioList.payloadLen -= (AudioData.payloadLen + 4);
                Frame.payloadLen     -= (AudioData.payloadLen + 4);
                Global.payloadLen    -= (AudioData.payloadLen + 4);
                /*
                // !!! TIME COMES TO CHANGE AUDIOLIST PAYLOADLEN
                // backtrack, rewrite, go back
                NewPayloadLen.audioList = 4 + 2 + 4 + AudioData.payloadLen + 4;
                cout << "NEW AUDIOLIST PLEN = " << NewPayloadLen.audioList << endl;
                cout << "currOutPos = " << outFile.tellp() << endl;
                outFile.seekp( - ((int)NewPayloadLen.audioList + 8), ios::cur);
                cout << "currOutPos = " << outFile.tellp() << endl;
                if ( ! writeUint32_t( oneAudioData, outFile ) ) { cout << "Audiolist amount of audiodata rewriting unsuccessful!" << endl; return false;}
                if ( ! writeUint32_t( NewPayloadLen.audioList, outFile ) ) { cout << "Audiolist payloadLen rewriting unsuccessful!" << endl; return false;}
                // go back to previous position
                outFile.seekp( NewPayloadLen.audioList,    ios::cur );
                cout << "currOutPos = " << outFile.tellp() << endl;
                 */
            } else {
                // IF LANGUAGES DIDN'T MATCH, JUST KEEP ON READING THROUGH THE FILE
                cout << "Languages don't match!" << endl;
                // TBD
                // READ ONLY !!! !!! !!! !!!
                // @ PAYLOADLEN, PAYLOAD, CRC
                cout << "TEST: AudioData.payloadLen = " << AudioData.payloadLen << endl;
                if ( ! readSkipper( inFile, AudioData.payloadLen ) ) { cout << "Audiodata payload reading unsuccessful!" << endl; return false; }
                if ( ! inFile.read(   (char*)&ORIGINAL_CRC, sizeof(ORIGINAL_CRC) ) ) { cout << "Unsuccessful AUDIODATA CRC reading" << endl;
                    return false; }
                AudioList.payloadLen -= (AudioData.payloadLen + 4);
                Frame.payloadLen     -= (AudioData.payloadLen + 4);
                Global.payloadLen    -= (AudioData.payloadLen + 4);
            }
        }
        if ( AudioList.payloadLen != 0 ) { cout << "NOT ALL AUDIOLIST DATA WERE READ!" << endl; return false; }
        // !!! TIME COMES TO CHANGE AUDIOLIST PAYLOADLEN
        // backtrack, rewrite, go back
        cout << "NEW AUDIOLIST PLEN = " << NewPayloadLen.audioList << endl;
        cout << "currOutPos = " << outFile.tellp() << endl;
        outFile.seekp( - ((int)NewPayloadLen.audioList + 8), ios::cur);
        cout << "currOutPos = " << outFile.tellp() << endl;
        if ( endian == BIG_ENDIAN_ID ) {
            convertEndian(audioDataWritten);
            convertEndian(NewPayloadLen.audioList);
        }
        if ( ! writeUint32_t( audioDataWritten, outFile ) ) { cout << "Audiolist amount of audiodata rewriting unsuccessful!" << endl; return false;}
        if ( ! writeUint32_t( NewPayloadLen.audioList, outFile ) ) { cout << "Audiolist payloadLen rewriting unsuccessful!" << endl; return false;}
        if ( endian == BIG_ENDIAN_ID ) {
            convertEndian(audioDataWritten);
            convertEndian(NewPayloadLen.audioList);
        }
        // go back to previous position
        outFile.seekp( NewPayloadLen.audioList,    ios::cur );
        cout << "currOutPos = " << outFile.tellp() << endl;
        
        // !!! CHANGE FRAME PAYLOADLEN
        NewPayloadLen.frame = (4 + 4 + VideoData.payloadLen + 4) + ( 4 + 4 + 4 + NewPayloadLen.audioList + 4 );
        outFile.seekp( - ((int)NewPayloadLen.frame ), ios::cur);
         cout << "currOutPos = " << outFile.tellp() << endl;
        if ( endian == BIG_ENDIAN_ID ) {
            convertEndian(NewPayloadLen.frame);
        }
        if ( ! writeUint32_t( NewPayloadLen.frame, outFile ) ) { cout << "Frame payloadLen rewriting unsuccessful!" << endl; return false;}
        if ( endian == BIG_ENDIAN_ID ) {
            convertEndian(NewPayloadLen.frame);
        }
        outFile.seekp( NewPayloadLen.frame-4,    ios::cur );
        // !!! ADD TO THE GLOBAL PLEN
        NewPayloadLen.globalHeader += 4 + 4 + NewPayloadLen.frame + 4;

        
        // < AudioList CRC  !!! CAN BE DONE EASIER USING CPY AND SETTER OF TMP UINT32_T VARIABLE TO 0
        if ( ! inFile.read(   (char*)&ORIGINAL_CRC, sizeof(ORIGINAL_CRC) ) ) { cout << "Unsuccessful AUDIOLIST CRC reading" << endl; return false; }
        if ( ! outFile.write( (char*)&NULLED_CRC,   sizeof(NULLED_CRC  ) ) ) { cout << "Unsuccessful AUDIOLIST CRC writing" << endl; return false; }
        Frame.payloadLen     -= 4;
        Global.payloadLen    -= 4;
        if ( Frame.payloadLen != 0 ) { cout << "SPATNY FRAME SOUCET" << endl; return false; }
        //   AudioList CRC >
        
        // < Frame CRC      !!! CAN BE DONE EASIER USING CPY AND SETTER OF TMP UINT32_T VARIABLE TO 0
        if ( ! inFile.read(   (char*)&ORIGINAL_CRC, sizeof(ORIGINAL_CRC) ) ) { cout << "Unsuccessful FRAME CRC reading" << endl; return false; }
        if ( ! outFile.write( (char*)&NULLED_CRC,   sizeof(NULLED_CRC  ) ) ) { cout << "Unsuccessful FRAME CRC writing" << endl; return false; }
        Global.payloadLen -= 4;
        //   Frame CRC >
        cout << "FRAME_" << i << " filtered!" << endl;
        cout << "OUT POS = " << outFile.tellp() << endl;
        
        
    }
    // < Global CRC     !!! CAN BE DONE EASIER USING CPY AND SETTER OF TMP UINT32_T VARIABLE TO 0
    if ( ! inFile.read(   (char*)&ORIGINAL_CRC, sizeof(ORIGINAL_CRC) ) ) { cout << "Unsuccessful GLOBAL CRC reading" << endl; return false; }
    if ( ! outFile.write( (char*)&NULLED_CRC,   sizeof(NULLED_CRC  ) ) ) { cout << "Unsuccessful GLOBAL CRC writing" << endl; return false; }
    //   Global CRC >
    cout << "END OUT POS = " << outFile.tellp() << endl;

    // !!! REWRITE GLOBAL PLEN
    outFile.seekp(8, ios::beg);
    if ( endian == BIG_ENDIAN_ID ) {
        convertEndian(NewPayloadLen.globalHeader);
    }
    if ( ! outFile.write( (char*)&NewPayloadLen.globalHeader, sizeof(NewPayloadLen.globalHeader) ) ) { cout << "Unsuccessful GLOBAL CRC writing" << endl; return false; }
    
    if ( Global.payloadLen != 0 ) { cout << "NOT ALL OF THE GLOBAL DATA WERE READ" << endl; return false; };
    
    if ( inFile.peek() != -1 ) { cout << "CORRUPTED INPUT FILE!" << endl; return false; }
    
    
    
    inFile.close();
    outFile.close();
    return true;
}



int main () {
    
    
    return 0;
}
