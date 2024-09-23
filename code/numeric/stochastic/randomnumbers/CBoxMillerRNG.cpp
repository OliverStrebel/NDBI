// CBoxMillerRNG.cpp: Implementierung der Klasse CBoxMillerRNG.
//
//////////////////////////////////////////////////////////////////////

#include "CBoxMillerRNG.h"

#include "numeric/stochastic/randomnumbers/CUniformRNG.h"

#include "util/CLog.h"

#include "math.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

const double CBoxMillerRNG::m_dPi = 3.14159265358979;

CBoxMillerRNG::CBoxMillerRNG( 
	double dSigma, CUniformRNG* pU1, CUniformRNG* pU2 )
{
	m_dSigma = dSigma;
	m_pU1 = pU1;
	m_pU2 = pU2;
}

CBoxMillerRNG::~CBoxMillerRNG()
{
	if ( m_pU1 != 0 )
	{
		delete m_pU1;
		m_pU1 = 0;
	}

	if ( m_pU2 != 0 )
	{
		delete m_pU2;
		m_pU2 = 0;
	}
}

double CBoxMillerRNG::G()
{
	return m_dSigma*sqrt( -2.0*log(m_pU1->U()) )*cos( 2.0*m_dPi*(m_pU2->U()) );
}
