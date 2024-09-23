#include "CHermiteUnivariatePolynomial.h"

CHermiteUnivariatePolynomial::CHermiteUnivariatePolynomial(int iDegree)
: m_iDegree(iDegree)
{
}

CHermiteUnivariatePolynomial::CHermiteUnivariatePolynomial(const CHermiteUnivariatePolynomial& aH)
{
    m_iDegree = aH.m_iDegree;
}

CHermiteUnivariatePolynomial::~CHermiteUnivariatePolynomial()
{
}

CHermiteUnivariatePolynomial CHermiteUnivariatePolynomial::operator=(const CHermiteUnivariatePolynomial& aH)
{
    if (this == &aH) return *this;
    m_iDegree = aH.m_iDegree;
    return *this;
}

double CHermiteUnivariatePolynomial::H(double& x)
{
	double Hminus2 = 1.0, Hminus1 = 2.0*x;
	if (m_iDegree == 0) return Hminus2;
	if (m_iDegree == 1) return Hminus1;
	
	int iPreviousDegree = 1;
	double dResult = 0.0;
	while (iPreviousDegree < m_iDegree)
	{
		dResult = 2.0*x*Hminus1 - (2.0*iPreviousDegree)*Hminus2;
		Hminus2 = Hminus1;
		Hminus1 = dResult;
        iPreviousDegree++;
	}

	return dResult;
}
