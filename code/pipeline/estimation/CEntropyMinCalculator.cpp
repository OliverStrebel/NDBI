#include "CEntropyMinCalculator.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "numeric/linalg/CCMLUDecomposer.h"
#include "util/CLog.h"

#include <limits>
#include <cmath>
#include <numbers>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////////////////////////////////////////////////


CEntropyMinCalculator::CEntropyMinCalculator(bool bLog)
	: m_bLog(bLog)
{
}

CEntropyMinCalculator::~CEntropyMinCalculator()
{
}

int CEntropyMinCalculator::getMinIndex(std::vector<CCMVector>& samples, CCMVector& means, double dSigmaScale)
{
	if (means.getDimension() == 1) return -1;
	CCMMatrix S = getMatrix(samples, means, dSigmaScale);
	CCMMatrix SR = S;
	return getMin(SR, calculateEntropies(S));
}


int CEntropyMinCalculator::getMinIndex(CCMVector& means, CCMVector& sd, double dSigmaScale)
{
	if (means.getDimension() == 1) return -1;//Stop in AbstractMECmd
	int iMin = -1;

	double dMin = DBL_MAX;
	for (int i = 0; i < means.getDimension(); i++)
	{
		if ((fabs(means.getElement(i)) < dSigmaScale * sd.getElement(i)) && (fabs(means.getElement(i)) / sd.getElement(i) < dMin))
		{
			dMin = fabs(means.getElement(i)) / sd.getElement(i);
			iMin = i;
		}
	}
	return iMin;
}

double CEntropyMinCalculator::getEntropy(std::vector<CCMVector>& samples, CCMVector& means, double dSigmaScale)
{
    CCMMatrix S = getMatrix(samples, means, dSigmaScale);
    double det = calculateDeterminant(S);
    return getEntropy(means.getDimension(), det);
}

double CEntropyMinCalculator::getMaxDistribution(std::vector<CCMVector>& samples, CCMVector& means)
{
    CCMMatrix S = getMatrix(samples, means, 1.0);
    double sqrtdet = sqrt(calculateDeterminant(S));
    return 1.0/(pow(sqrt(2.0*std::numbers::pi), 1.0*means.getDimension())*sqrtdet);
}

double CEntropyMinCalculator::getMaxDistributionUnscaled(std::vector<CCMVector>& s, CCMVector& m)
{
    int iParameterCount = m.getDimension();
    CCMMatrix S(iParameterCount, iParameterCount);

    for (int k = 0; k < (int)s.size(); k++)
        for (int i = 0; i < iParameterCount; i++)
            for (int j = 0; j < iParameterCount; j++)
            {
                double dS = (s[k].getElement(i) - m.getElement(i))*(s[k].getElement(j) - m.getElement(j));
                S.setElement(i, j, S.getElement(i, j) + dS);
            }

    S = S*(1.0/(1.0*s.size()));
    double sqrtdet = sqrt(calculateDeterminant(S));
    return 1.0/(pow(sqrt(2.0 * std::numbers::pi), 1.0*m.getDimension()) * sqrtdet);
}

double CEntropyMinCalculator::getEntropy(int k, double det)
{
    return 0.5*k + 0.5*std::log(2.0*std::numbers::pi)*k + 0.5*std::log(det);
}

CCMVector CEntropyMinCalculator::getMean(std::vector<CCMVector>& samples)
{
	CCMVector m(samples.at(0).getDimension());
	for (unsigned int i = 0; i < samples.size(); i++) m = m + samples.at(i);
	m = m*(1.0/samples.size());
	return m;
}

CCMVector CEntropyMinCalculator::getSD(std::vector<CCMVector>& samples, CCMVector& m)
{
	CCMVector sd(m.getDimension());
	for (int j = 0; j < sd.getDimension(); j++)
	{
		double d = 0.0;
		for (unsigned int i = 0; i < samples.size(); i++)
			d = d + pow(m.getElement(j) - samples.at(i).getElement(j), 2.0);

		sd.setElement(j, sqrt(d/(1.0*samples.size())));
	}
	return sd;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//private helpers
CCMMatrix CEntropyMinCalculator::getMatrix(std::vector<CCMVector>& s, CCMVector& m, double dSigmaScale)
{
	int iParameterCount = m.getDimension();
	CCMMatrix S(iParameterCount, iParameterCount);

	for ( int k = 0; k < (int) s.size(); k++)
		for (int i = 0; i < iParameterCount; i++)
			for (int j = 0; j < iParameterCount; j++)
			{
				double dS = (s[k].getElement(i)- m.getElement(i))*(s[k].getElement(j) - m.getElement(j))/(m.getElement(i)*m.getElement(j));
				S.setElement(i, j, S.getElement(i, j) + dS);
			}

	S = S*(dSigmaScale*dSigmaScale/(1.0*s.size()));
	return S;
}

std::vector<double> CEntropyMinCalculator::calculateEntropies(CCMMatrix& S)
{
	std::vector<double> entropies;
    int iSize = S.getColCount()-1;
	for (int i = 0; i < S.getColCount(); i++)
	{
		CCMMatrix A = S.getSubMatrix(i, i);
        entropies.push_back(getEntropy(iSize, calculateDeterminant(A)));
	}
	return entropies;
}

double CEntropyMinCalculator::calculateDeterminant(CCMMatrix& A)
{
	double det = 1.0;
	CCMLUDecomposer::decompose(A);
	for (int d = 0; d < A.getColCount(); d++) det*=(A.getElement(d, d));
	return det;
}

int CEntropyMinCalculator::getMin(CCMMatrix& SR, std::vector<double> entropies)
{
    int iSize = SR.getColCount();
	double dMin = getEntropy(iSize, calculateDeterminant(SR));
	int iMin = -1;
    CLog::debugEndl(dMin, m_bLog);
	for (unsigned int k = 0; k < entropies.size(); k++)
	{
		CLog::debug(entropies[k], m_bLog);
		CLog::debug(";", m_bLog);
	}
	CLog::debugEndl("", m_bLog);
	for (unsigned int i = 0; i < entropies.size(); i++)
		if (entropies[i] < dMin)
		{
			dMin = entropies[i];
			iMin = (int) i;
		}
	return iMin;
}
