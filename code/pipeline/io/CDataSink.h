// CDataSink.h: Schnittstelle für die Klasse CDataSink.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CDATASINK_H)
#define CDATASINK_H

#ifndef __STRING__
#include <string>
#endif

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif


class CPipelineData;
class CResultData;
class CDEData;

class CDataSink : public CPipelineElement
{
public:
	CDataSink(CPipelineElement* pPE);
	virtual ~CDataSink();

	virtual CPipelineData* next( );

protected:
	CDataSink(){};

private:
    void writeResultFile(CResultData& aRD, std::string aFileName);
    void writeDEDataFile(CDEData& aDED, std::string aFileName);

    std::vector<std::string> getPaths(CPipelineData* pPD);
    std::string getName(CPipelineData* pPD, std::string sinkFilePath, std::string ident);
    std::string getPrefix(std::string ident);
    std::string getSuffix(std::string ident);

    std::string getIdentifierSkip(CPipelineData* pPD);
    std::string getIdentifierNoise(CPipelineData* pPD, int iPrecision);
};

#endif //CDATASINK_H