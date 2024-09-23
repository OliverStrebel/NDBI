//CFileReader.cpp: Implementierung der Klasse 
//CFileReader.
//
////////////////////////////////////////////////////////////////////////////////

#include "CFileReader.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include "util/CLog.h"
#include "util/CEADSyException.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CFileReader::CFileReader()
{
}

CFileReader::~CFileReader()
{

}

std::stringstream CFileReader::getFileData(std::string aFileName)
{
	std::fstream cfgFile(aFileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!cfgFile.is_open()) throw CEADSyException(std::string("File cannot be opened: ") + aFileName);
	
	std::stringstream sData("");
	std::streampos size = cfgFile.tellg();
	std::streampos diff(1);
	char* memblock = new char[size + diff];
	cfgFile.seekg(0, std::ios::beg);
	cfgFile.read(memblock, size);
	memblock[size] = '\0';
	sData = std::stringstream(memblock);
	delete[] memblock;
	cfgFile.close();

	return sData;
}

