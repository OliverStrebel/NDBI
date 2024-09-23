// CEcuyer39373RNG.h: Schnittstelle für die Klasse CEcuyer39373RNG.
//
//////////////////////////////////////////////////////////////////////
#if !defined(CECUYER39373RNG_H)
#define CECUYER39373RNG_H

#ifndef CUNIFORMRNG_H
#include "numeric/stochastic/randomnumbers/CUniformRNG.h"
#endif

class CEcuyer39373RNG : public CUniformRNG 
{
public:
	CEcuyer39373RNG( unsigned int iSeed );
	virtual ~CEcuyer39373RNG();
    virtual double U();
    virtual double PM();
    virtual double E( double dLambda );
    virtual unsigned int next();

private:
	CEcuyer39373RNG();
	unsigned int m_iSeed;

	const static unsigned int m;
	const static unsigned int a;
};

#endif // !defined(CECUYER39373RNG_H)
