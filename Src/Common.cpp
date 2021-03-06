/*
 * Common.cpp
 *
 *  Created on: 2/12/2020
 *      Author: FearLose
 *      Encoder: UTF-8
 */

#include "Stdafx.hpp"

void Encryption(std::fstream * lpFile, std::string szNFile);
void Decryption(std::fstream * lpFile, std::string szNFile);

int main(int argc, char* argv[])
{
    if( !argv[1] )  // std::cin.get();  ???
    {
        std::cout << "Please add file parameters." << std::endl;
        getchar();
        return 0;
    }

    std::fstream _fBatch;  // Open file

    if( !argv[2] )
    {
        Encryption(&_fBatch, argv[1]);
        return 0;
    }

    Decryption(&_fBatch, argv[1]);

    return 0;
}

void Encryption(std::fstream * lpFile, std::string szNFile)
{
    const int _iUnicode[] = { 0xFE, 0xFF, 0x0D, 0x0A };   // Unicode + '\r\n'

    char _szGetLN;         // Get file line text
    std::string _szFData;  // append _szGetLN data.

    // First open get file data.
    if(!lpFile->is_open())
        lpFile->open(szNFile, std::ios::in|std::ios::out|std::ios::binary);

    while ( lpFile->good( ) && !lpFile->eof( ) )
    {
        lpFile->get( _szGetLN );
        _szFData += _szGetLN;
    }

    // Agent open on the clear file.
    lpFile->close();
    lpFile->open(szNFile, std::ios::in|std::ios::out|std::ios::binary|std::ios::trunc);

    // Write Unicode
    for(int iCode : _iUnicode)
        lpFile->put( iCode );

    _szFData = _szFData.substr(0, _szFData.size() - 1);    // (ABCDD)->(D) 1 byte 0x00 ?
    *lpFile << _szFData; // Write to file

    lpFile->close(); // Save to file
}

void Decryption(std::fstream * lpFile, std::string szNFile)
{
    const int _iUnicode[] = { 0xFE, 0xFF, 0x0D, 0x0A };   // Unicode + '\r\n'

    char _szGetLN;         // Get file line text
    std::string _szFData;  // append _szGetLN data.

    // First open get file data.
    if(!lpFile->is_open())
        lpFile->open(szNFile, std::ios::in|std::ios::out|std::ios::binary);

    while ( lpFile->good( ) && !lpFile->eof( ) )
    {
        lpFile->get( _szGetLN );
        _szFData += _szGetLN;
    }

    if ( _szFData[0] != _iUnicode[0] && _szFData[1] != _iUnicode[1] && _szFData[2] != _iUnicode[2] && _szFData[3] != _iUnicode[3] )
        return;

    // Agent open on the clear file.
    lpFile->close();
    lpFile->open(szNFile, std::ios::in|std::ios::out|std::ios::binary|std::ios::trunc);

    // Remove Unicode
    _szFData = _szFData.substr(4, _szFData.size() - 5);    // (ABCDDD\n)->(DD\n) 6 byte 0x00 0x00 0x00 ?
    *lpFile << _szFData;  // Write to file

    lpFile->close();  // Save to file
}
