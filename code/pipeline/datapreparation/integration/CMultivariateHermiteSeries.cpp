#include "CMultivariateHermiteSeries.h"

#include "numeric/linalg/CCMVector.h"

//#include "util/CLog.h"
#include "util/CEADSyException.h"

/*
In m_P are nComp vectors containing CHermiteUnivariatePolynomial, where nComp is
the number of components of the data. The mechanics of the addition of Hermitepolynomials in this constructor
becomes understandable, if one looks at the dump of m_P at the end of this file 
for iDegreePlusOne = 3 and iDimension = 3..

The format of this output becomes understandable, when one looks at
the method CMultivariateHermiteSeries::dumpDegrees().
*/
CMultivariateHermiteSeries::CMultivariateHermiteSeries(int iDegree, int iDimension)
{
	if ((iDimension < 1) || (iDegree < 1)) throw CEADSyException("CMultivariateHermiteSeries::ctor: invaild parameters");

	std::vector<int> exponents, repetitions, repetitionCounter;
	for (int iComp = 0; iComp < iDimension; iComp++)
	{
		std::vector<CHermiteUnivariatePolynomial> v;
		m_P.push_back(v);
        exponents.push_back(0);
        repetitions.push_back((int)pow(iDegree+1, iComp));
        repetitionCounter.push_back(0);
    }

    int iSize = (int) pow(iDegree+1, iDimension);
    for (int k = 0; k < iSize; k++)
    {
        if (getSumExponents(exponents) <= iDegree) 
            for (int iComp = 0; iComp < (int)m_P.size(); iComp++)
                m_P[iComp].push_back(CHermiteUnivariatePolynomial(exponents[iComp]));

        for (int iComp = 0; iComp < (int) m_P.size(); iComp++)
        {
            repetitionCounter[iComp]++;
            if (repetitionCounter[iComp] == repetitions[iComp])
            {
                repetitionCounter[iComp] = 0;
                exponents[iComp]++;
                if (exponents[iComp] == iDegree+1) exponents[iComp] = 0;
            }
        }
    }
    //CLog::debugEndl(dumpDegrees().c_str());
}

double CMultivariateHermiteSeries::evaluateSummand(int iMonome, std::vector<double>& x)
{
	if ((iMonome >= m_P[0].size()) || (x.size() != m_P.size()))
		throw CEADSyException("CMultivariateHermiteSeries::evaluateSummand: invaild parameters");

	double dResult = 1.0;
	for (int iComp = 0; iComp < (int)m_P.size(); iComp++) dResult *= m_P[iComp][iMonome].H(x[iComp]);

	return dResult;
}

double CMultivariateHermiteSeries::evaluateSummand(int iMonome, CCMVector& x)
{
	std::vector<double> v(x.getDimension());
	for (int iComp = 0; iComp < x.getDimension(); iComp++) v[iComp] = x.getElement(iComp);
	return evaluateSummand(iMonome, v);
}

int CMultivariateHermiteSeries::getSumExponents(std::vector<int>& exponents)
{
    int iSum = 0;
    for (size_t i = 0; i < exponents.size(); i++) iSum += exponents[i];
    return iSum;
}

std::string CMultivariateHermiteSeries::dumpDegrees()
{
    std::string dump("");

    for (int j = 0; j < (int) m_P[0].size(); j++)
    {
        for (int iComp = 0; iComp < (int) m_P.size(); iComp++)
            dump.append( std::to_string(m_P[iComp][j].getDegree())).append(" ");
		dump.append("\n");
	}
	return dump;
}

/*
iDegree = iDimension = 3
0 0 0
1 0 0
2 0 0
3 0 0
0 1 0
1 1 0
2 1 0
0 2 0
1 2 0
0 3 0
0 0 1
1 0 1
2 0 1
0 1 1
1 1 1
0 2 1
0 0 2
1 0 2
0 1 2
0 0 3
*/