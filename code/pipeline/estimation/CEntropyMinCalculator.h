// CEntropyMinCalculator.h: Schnittstelle für die Klasse CEntropyMinCalculator.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CENTROPYMINCALCULATOR_H)
#define CENTROPYMINCALCULATOR_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif


class CCMMatrix;
class CCMVector;

class CEntropyMinCalculator  
{
public:
	CEntropyMinCalculator(bool bLog = false);
	virtual ~CEntropyMinCalculator();

	int getMinIndex(std::vector<CCMVector>& samples, CCMVector& means, double dSigmaScale);
	int getMinIndex(CCMVector& means, CCMVector& sd, double dSigmaScale);

    double getEntropy(int k, double det);
    double getEntropy(std::vector<CCMVector>& samples, CCMVector& means, double dSigmaScale);
    double getMaxDistribution(std::vector<CCMVector>& samples, CCMVector& means);
    double getMaxDistributionUnscaled(std::vector<CCMVector>& samples, CCMVector& means);

	CCMVector getMean(std::vector<CCMVector>& samples);
	CCMVector getSD(std::vector<CCMVector>& samples, CCMVector& mean);


private:
	CEntropyMinCalculator(): m_bLog(false) {};

    CCMMatrix getMatrix(std::vector<CCMVector>& samples, CCMVector& means, double dSigmaScale);
    double calculateDeterminant(CCMMatrix& A);
    std::vector<double> calculateEntropies(CCMMatrix&  S);
	int getMin(CCMMatrix& SR, std::vector<double> entropies);

	bool m_bLog;
};

#endif //CENTROPYMINCALCULATOR_H
