// CResultData.h: Schnittstelle für die Klasse CResultData.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CRESULTDATA_H)
#define CRESULTDATA_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef __STRING__
#include <string>
#endif


#ifndef CCMVECTOR_H
#include "numeric/linalg/CCMVector.h"
#endif

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif


class CDE;

class CResultData
{
public:
	CResultData(std::vector<CDE*> aDE);
	virtual ~CResultData();

    enum Error{NO_MONOME_FOUND=-9, VARIANCE_UNCALCULATABLE=-8, NO_MONOME_ELIMINATED=-7};

	int getDECount(){return (int) m_DE.size();};
	CDE& getDE(int iPos){return *(m_DE[iPos]);};

	void prepareStep(int iComp, const char* pTypeName);
	void addSampleResults(int iComp, int iParameterNumber, double dMean, double dStdDev);
	void storeError(int iComp, int iErrorCode);
	std::vector<std::string> getLastResult(int iComp);
    double getFirstAddedMean(int iComp){return m_Means[iComp][0][0];};
    double getFirstAddedStdDev(int iComp){return m_StdDevs[iComp][0][0];};
    bool hasData(){return m_ParameterNumbers[0].size() > 0;};

private:
	CResultData(){};

	CEADSyConfig& cfg(){return CEADSyConfig::instance();};

	std::vector<CDE*> m_DE;

	//1st index: ode component. 2nd index: elimination step. 3rd index: values for parameter
	std::vector<std::vector<std::string>> m_Types;
	std::vector<std::vector<std::vector<int>>> m_ParameterNumbers;
	std::vector<std::vector<std::vector<double>>> m_Means;
	std::vector<std::vector<std::vector<double>>> m_StdDevs;
};

#endif //CRESULTDATA_H