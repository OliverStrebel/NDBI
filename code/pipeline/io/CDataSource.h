// CDataSource.h: Schnittstelle für die Klasse CDataSource.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CDATASOURCE_H)
#define CDATASOURCE_H


#ifndef __STRING__
#include <string>
#endif

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef __SSTREAM__
#include <sstream>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

class CPipelineData;
class CDEData;

class CDataSource : public CPipelineElement
{
public:
	CDataSource(CPipelineElement* pPE);
	virtual ~CDataSource();

	virtual CPipelineData* next( );
    void setSourceFile(std::string strFile){ m_strFile = strFile;};
	void setSkipRecords(int iSkipRecords){ m_iSkipRecords = iSkipRecords;};

	CDEData* getDEData();

protected:
	CDataSource():  m_strFile(""), m_iSkipRecords(0) {};

private:
	int getColCount(std::string line);
	int getRowCount(std::vector<std::string> lines);
	int getDependendVariableOffset(std::string line);
	std::vector<std::string> getLinesFromFile();

	std::string m_strFile;
	int m_iSkipRecords;
};

#endif //CDATASOURCE_H