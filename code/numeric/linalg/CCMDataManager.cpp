//CCMDataManager.cpp: Implementierung der Klasse CCMDataManager.
//
//////////////////////////////////////////////////////////////////////

#include "CCMDataManager.h"

#include "CCMVector.h"
#include "CCMMatrix.h"

#include "util/CEADsyException.h"

#include "util/CLog.h"

#include <math.h>

/**
 * This class is a friend of CCMMatrix and CCMVector and allows
 * access to their data.
 */
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

double* CCMDataManager::getData( CCMVector& aV ){return aV.m_pd;}

double* CCMDataManager::getData( CCMMatrix& aM ){return aM.m_pd;}

CCMMatrix CCMDataManager::createCMMatrix( double* pd, int aRowCount, int aColCount )
{
	return CCMMatrix( pd, aRowCount, aColCount );
}

CCMVector CCMDataManager::createCMVector( double* pd, int aDimension )
{
	return CCMVector( pd, aDimension );
}

CCMMatrix CCMDataManager::createCMSquareUnitMatrix( int aRowCount )
{
	CCMMatrix I = CCMMatrix( aRowCount, aRowCount );
	for ( int i = 0; i < aRowCount; i++ )
		I.setElement( i, i, 1.0 );

	return I;
}

void CCMDataManager::check( double dNum, char* pc, double dLevel )
{
	if ( fabs( dNum ) < dLevel )
	{
		CLog::debugEndl( pc );
		throw CEADSyException( pc );
	}
}

void CCMDataManager::checkIter( int i, int iMax, char* pc )
{
	if ( i >= iMax )
	{
		CLog::debugEndl( pc );
		throw CEADSyException( pc );
	}
}
