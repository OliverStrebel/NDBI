#ifndef CMULTIVARIATEHERMITESERIES_H
#define CMULTIVARIATEHERMITESERIES_H


#ifndef __VECTOR__
#include <vector>
#endif

#ifndef __STRING__
#include <string>
#endif


#ifndef CHERMITEUNIVARIATEPOLYNOMIAL_H
#include "CHermiteUnivariatePolynomial.h"
#endif

class CCMVector;

class CMultivariateHermiteSeries
{
public:
	CMultivariateHermiteSeries(int iDegree, int iDimension);
	virtual ~CMultivariateHermiteSeries(){};

	int getLength(){return (int) m_P[0].size();};
	double evaluateSummand(int iMonome, std::vector<double>& x);
	double evaluateSummand(int iMonome, CCMVector& x);

	std::string dumpDegrees();

private:
	CMultivariateHermiteSeries(){};

	std::vector<std::vector<CHermiteUnivariatePolynomial>> m_P;
    int getSumExponents(std::vector<int>& exponents);
};

#endif  // CMULTIVARIATEHERMITESERIES_H
