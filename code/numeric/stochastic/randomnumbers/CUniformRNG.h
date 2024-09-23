// CUniformRNG.h: Schnittstelle für die Klasse CUniformRNG.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CUNIFORMRNG_H)
#define CUNIFORMRNG_H

class CBO;

class CUniformRNG  
{
public:
	CUniformRNG();
	virtual ~CUniformRNG();
    virtual double U() = 0;
    virtual double PM() = 0;
    virtual double E( double dLambda ) = 0;
    virtual unsigned int next() = 0;
};

#endif // !defined(CUNIFORMRNG_H)
