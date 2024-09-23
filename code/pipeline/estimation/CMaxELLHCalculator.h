// CMaxELLHCalculator.h: Schnittstelle für die Klasse CMaxELLHCalculator.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CMAXELLHCALCULATOR_H)
#define CMAXELLHCALCULATOR_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif


class CCMMatrix;
class CCMVector;
class CPipelineData;

class CMaxELLHCalculator  
{
public:
	CMaxELLHCalculator(bool bLog = false);
	virtual ~CMaxELLHCalculator();

	int getMinIndex(std::vector<CCMVector>& samples, CCMVector& means, CPipelineData* pPD, int iComp);

private:
	CMaxELLHCalculator(): m_bLog(false) {};
    CEADSyConfig& cfg(){return CEADSyConfig::instance();};

    double setRegressionProblem(CCMMatrix& A, CCMVector& x, CCMVector& b, CPipelineData* pPD, int iComp);
    double getStdDev(CCMMatrix& A, CCMVector& x, CCMVector&b);
    double getELLH(std::vector<CCMVector>& samples, CCMMatrix& A, CCMVector& b, double dSigma, int iExcludedCol = -1);
    bool hasLargerELLH(double dOldMAPL, CCMVector& mapl);
    int getLargestAfterRemoval(CCMVector& mapl);

	bool m_bLog;
};

#endif //CMaxELLHCalculator_H
