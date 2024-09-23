// CEcuyer40014RNG.cpp: Implementierung der Klasse CEcuyer40014RNG.
//
//////////////////////////////////////////////////////////////////////

#include "CEcuyer40014RNG.h"

#include "util/CLog.h"

#include "math.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

const unsigned int CEcuyer40014RNG::m = 2147483563;

const unsigned int CEcuyer40014RNG::a = 40014;

CEcuyer40014RNG::CEcuyer40014RNG( unsigned int iSeed )
{
	m_iSeed = iSeed;
}

CEcuyer40014RNG::~CEcuyer40014RNG()
{

}

double CEcuyer40014RNG::U()
{
	m_iSeed = (a*m_iSeed)%m;
	return (1.0*m_iSeed)/m;
}

double CEcuyer40014RNG::PM()
{
	if ( U() < 0.5 ) return 1.0;
	return -1.0;
}

double CEcuyer40014RNG::E( double dLambda )
{
	return -log(1-U())/dLambda;
}

unsigned int CEcuyer40014RNG::next()
{
	m_iSeed = (a*m_iSeed)%m;
	return m_iSeed;
}
