// CBoxMillerRNG.h: Schnittstelle für die Klasse CBoxMillerRNG.
//
//////////////////////////////////////////////////////////////////////
#if !defined(CBOXMILLERRNG_H)
#define CBOXMILLERRNG_H

class CUniformRNG;

class CBoxMillerRNG
{
public:
	CBoxMillerRNG( double dSigma, CUniformRNG* pU1, CUniformRNG* pU2 );
	virtual ~CBoxMillerRNG();
    virtual double G();

private:
	CBoxMillerRNG();

	CUniformRNG* m_pU1;
	CUniformRNG* m_pU2;

	double m_dSigma;
	const static double m_dPi;
};

#endif // !defined(CBOXMILLERRNG_H)
