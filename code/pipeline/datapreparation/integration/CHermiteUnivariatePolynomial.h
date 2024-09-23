#ifndef CHERMITEUNIVARIATEPOLYNOMIAL_H
#define CHERMITEUNIVARIATEPOLYNOMIAL_H


class CHermiteUnivariatePolynomial
{
public:
	CHermiteUnivariatePolynomial(int iDegree);
    CHermiteUnivariatePolynomial(const CHermiteUnivariatePolynomial& aH);
    virtual ~CHermiteUnivariatePolynomial();

	double H(double& d);
	int getDegree() {return m_iDegree;};
    CHermiteUnivariatePolynomial operator=(const CHermiteUnivariatePolynomial& aH);

private:
    int m_iDegree;
	CHermiteUnivariatePolynomial() : m_iDegree(0){};
};

#endif  // CHERMITEUNIVARIATEPOLYNOMIAL_H
