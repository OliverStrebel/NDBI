// CEcuyer40692RNG.cpp: Implementierung der Klasse CEcuyer40692RNG.
//
//////////////////////////////////////////////////////////////////////

#include "CEcuyer40692RNG.h"

#include "util/CLog.h"

#include "math.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

const unsigned int CEcuyer40692RNG::m = 2147483399;

const unsigned int CEcuyer40692RNG::a = 40692;

CEcuyer40692RNG::CEcuyer40692RNG( unsigned int iSeed )
{
	m_iSeed = iSeed;
}

CEcuyer40692RNG::~CEcuyer40692RNG()
{

}

double CEcuyer40692RNG::U()
{
	m_iSeed = (a*m_iSeed)%m;
	return (1.0*m_iSeed)/m;
}

double CEcuyer40692RNG::PM()
{
	if ( U() < 0.5 ) return 1.0;
	return -1.0;
}

double CEcuyer40692RNG::E( double dLambda )
{
	return -log(1-U())/dLambda;
}

unsigned int CEcuyer40692RNG::next()
{
	m_iSeed = (a*m_iSeed)%m;
	return m_iSeed;
}
