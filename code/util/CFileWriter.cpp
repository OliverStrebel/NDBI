//CFileWriter.cpp: Implementierung der Klasse 
//CFileWriter.
//
////////////////////////////////////////////////////////////////////////////////
#include "CFileWriter.h"

#include "pipeline/datastructures/CDEData.h"

#include "util/CLog.h"
#include "util/CEADSyException.h"

#include <iostream>
#include <fstream>
#include <iomanip>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CFileWriter::CFileWriter()
{
}

CFileWriter::~CFileWriter()
{

}

void CFileWriter::writeDEData(CDEData& data, std::string& aFileName)
{
    std::ofstream aFile;
    aFile.open(aFileName, std::ios::trunc);
    if (!aFile.is_open()) throw CEADSyException(std::string("File cannot be opened: ") + aFileName);

    aFile << data.getDependendVariableOffset() << std::endl;
    aFile << std::setprecision(10);
    for (int i = 0; i < data.getRowCount(); i++)
    {
        for (int j = 0; j < data.getColCount() - 1; j++) aFile << std::scientific << data.get(j,i) << " ";
        aFile << std::scientific << data.get(data.getColCount() - 1, i) << std::endl;
    }

    aFile.flush();
    aFile.close();
}

void CFileWriter::writeDEData(CDEData& data, const char* pchFileName)
{
    std::string s(pchFileName);
    writeDEData(data, s);
}

