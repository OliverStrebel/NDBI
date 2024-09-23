#include "CNewtonEquationSolver.h"

#include "util/CLog.h"
#include "util/CEADSyException.h"

#include <float.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CNewtonEquationSolver::CNewtonEquationSolver( bool bLog )
{
}

CNewtonEquationSolver::~CNewtonEquationSolver()
{
}

void CNewtonEquationSolver::doLog( int iCompNo, int iIter, double dNorm )
{
	CLog::debug( iCompNo, getLog() );
	CLog::debug( ";", getLog() );
	CLog::debug( iIter, getLog() );
	CLog::debug( ";", getLog() );
	CLog::debugEndl( dNorm, getLog() );
	checkNorm(dNorm);
}

void CNewtonEquationSolver::checkNorm( double dNorm )
{
	if ( _isnan( dNorm ) != 0 )
		throw CEADSyException( "Norm not a Number");
}
