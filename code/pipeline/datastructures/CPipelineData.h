// CPipelineData.h: Schnittstelle für die Klasse CPipelineData.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CPIPELINEDATA_H)
#define CPIPELINEDATA_H

#ifndef __STRING__
#include <string>
#endif

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif


class CDEData;
class CResultData;

class CPipelineData
{
public:
	CPipelineData(std::string& aFileName, CDEData* pDEData, double dNoiseAdded = 0.0);
	virtual ~CPipelineData();

    CDEData& getDEData(){return *m_pDEData;};
	CResultData& getResultData(){return *m_pResultData;};
	void addPreparedDEData(CDEData* pDEData){m_PreparedDEData.push_back(pDEData);};
	int getPreparedDEDataSize(){return (int) m_PreparedDEData.size();};
	CDEData& getPreparedDEData(int iPos){return *(m_PreparedDEData[iPos]);};

	std::string getFileName(){return m_FileName;};
	double getNoiseAdded(){return m_dNoiseAdded;};
	void setNoiseAdded(double dNoiseAdded){m_dNoiseAdded = dNoiseAdded;};
	int getSkipRecords(){return m_iSkipRecords; };
	void setSkipRecords(int iSkipRecords){m_iSkipRecords = iSkipRecords;};

private:
	CPipelineData(): m_pDEData(0), m_pResultData(0), m_FileName(""), m_dNoiseAdded(0.0), m_iSkipRecords(0) {};

	CEADSyConfig& cfg(){return CEADSyConfig::instance();};

	CDEData* m_pDEData;
	CResultData* m_pResultData;
	std::vector<CDEData*> m_PreparedDEData;
	std::string m_FileName;

	double m_dNoiseAdded;
	int m_iSkipRecords;
};

#endif //CPIPELINEDATA_H