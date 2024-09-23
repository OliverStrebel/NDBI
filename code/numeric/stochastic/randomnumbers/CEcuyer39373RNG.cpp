// CEcuyer39373RNG.cpp: Implementierung der Klasse CEcuyer39373RNG.
//
//////////////////////////////////////////////////////////////////////

#include "CEcuyer39373RNG.h"

#include "util/CLog.h"

#include "math.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

const unsigned int CEcuyer39373RNG::m = 2147483647;

const unsigned int CEcuyer39373RNG::a = 39373;

CEcuyer39373RNG::CEcuyer39373RNG( unsigned int iSeed )
{
	m_iSeed = iSeed;
}

CEcuyer39373RNG::~CEcuyer39373RNG()
{

}

double CEcuyer39373RNG::U()
{
	m_iSeed = (a*m_iSeed)%m;
	return (1.0*m_iSeed)/m;
}

double CEcuyer39373RNG::PM()
{
	if ( U() < 0.5 ) return 1.0;
	return -1.0;
}

double CEcuyer39373RNG::E( double dLambda )
{
	return -log(1-U())/dLambda;
}

unsigned int CEcuyer39373RNG::next()
{
	m_iSeed = (a*m_iSeed)%m;
	return m_iSeed;
}
