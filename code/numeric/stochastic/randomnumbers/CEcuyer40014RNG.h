// CEcuyer40014RNG.h: Schnittstelle für die Klasse CEcuyer40014RNG.
//
//////////////////////////////////////////////////////////////////////
#if !defined(CECUYER40014RNG_H)
#define CECUYER40014RNG_H

#ifndef CUNIFORMRNG_H
#include "numeric/stochastic/randomnumbers/CUniformRNG.h"
#endif

class CEcuyer40014RNG : public CUniformRNG 
{
public:
	CEcuyer40014RNG( unsigned int iSeed );
	virtual ~CEcuyer40014RNG();
    virtual double U();
    virtual double PM();
    virtual double E( double dLambda );
    virtual unsigned int next();

private:
	CEcuyer40014RNG();
	unsigned int m_iSeed;

	const static unsigned int m;
	const static unsigned int a;
};

#endif // !defined(CECUYER40014RNG_H)
