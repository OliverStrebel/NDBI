// CEcuyer40692RNG.h: Schnittstelle für die Klasse CEcuyer40692RNG.
//
//////////////////////////////////////////////////////////////////////
#if !defined(CECUYER40692RNG_H)
#define CECUYER40692RNG_H

#ifndef CUNIFORMRNG_H
#include "numeric/stochastic/randomnumbers/CUniformRNG.h"
#endif

class CEcuyer40692RNG : public CUniformRNG 
{
public:
	CEcuyer40692RNG( unsigned int iSeed );
	virtual ~CEcuyer40692RNG();
    virtual double U();
    virtual double PM();
    virtual double E( double dLambda );
    virtual unsigned int next();

private:
	CEcuyer40692RNG();
	unsigned int m_iSeed;

	const static unsigned int m;
	const static unsigned int a;
};

#endif // !defined(CECUYER40692RNG_H)
