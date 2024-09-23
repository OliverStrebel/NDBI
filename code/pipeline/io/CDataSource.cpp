//CDataSource.cpp: Implementierung der Klasse CDataSource.
///////////////////////////////////////////////////////////////////////////////
#include "CDataSource.h"

#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"
#include "pipeline/datastructures/CResultData.h"

#include "pipeline/de/CDEFactory.h"

#include "util/CFileReader.h"
#include "util/CEADSyException.h"
#include "util/CLog.h"


CDataSource::CDataSource(CPipelineElement* pPE)
: CPipelineElement( pPE), m_strFile(""), m_iSkipRecords(0)
{
}

CDataSource::~CDataSource()
{
}

CPipelineData* CDataSource::next()
{ 
    CPipelineData* p = new CPipelineData(m_strFile, getDEData());
	p->setSkipRecords(m_iSkipRecords);
    return p;
}

CDEData* CDataSource::getDEData()
{
    std::vector<double*>* pD = new std::vector<double*>();
    std::vector<std::string> lines = getLinesFromFile();
	int iColCount = getColCount(lines[1]);//lines[0] enthaelt dependendVariablesOffset
	int iRowCount = getRowCount(lines);
	for (int j = 0; j < iColCount; j++) pD->push_back(new double[iRowCount]);

	int iRow = 0;
	for (int i = 1; i < (int) lines.size(); i+=(m_iSkipRecords+1))
	{
		std::stringstream ss(lines[i]);
		std::string cell;
		int j = 0;
		while (std::getline(ss, cell, CEADSyConfig::instance().m_csvSeparator[0]))
		{
		    (*pD)[j][iRow] = std::stod(cell);
			j++;
		}
		iRow++;
	}
    return new CDEData(iRowCount, getDependendVariableOffset(lines[0]), pD);
}

std::vector<std::string>  CDataSource::getLinesFromFile()
{
    CFileReader r;
	std::stringstream sData = r.getFileData(m_strFile);

	std::string item;
	std::vector<std::string> lines;
	if (cfg().getNumberOfLinesLimit() <= 0) while (std::getline(sData, item)) lines.push_back(item);
    else
    {
        int iLimit = 0;
        while (std::getline(sData, item) && iLimit <= cfg().getNumberOfLinesLimit())
        {
            lines.push_back(item);
            iLimit++;
        }
    }
    return lines;
}

int CDataSource::getColCount( std::string line)
{
    int iColCount = 0;
    std::string cell;
    std::stringstream ss(line);
	while (std::getline(ss, cell, ' ')) iColCount++;    
    return iColCount;
}

int CDataSource::getDependendVariableOffset(std::string line)
{
    return std::stoi(line);
}

int CDataSource::getRowCount(std::vector<std::string> lines)
{
    int iRowCount = 0;
	for (int i = 1; i < (int) lines.size(); i+=(m_iSkipRecords+1)) iRowCount++;
	return iRowCount;
}
