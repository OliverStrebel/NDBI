#include "CResidualSquareNorm_NES.h"

#include "CNewtonEquationInterface.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "numeric/linalg/solver/CLinearEquationSolver.h"

#include "util/CLog.h"
#include "util/CEADSyException.h"

#include <float.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CResidualSquareNorm_NES::CResidualSquareNorm_NES( bool bLog )
{
	setLog( bLog );
}

CResidualSquareNorm_NES::~CResidualSquareNorm_NES()
{
}

bool CResidualSquareNorm_NES::solve( CNewtonEquationInterface& aI, CLinearEquationSolver& aS)
{
	CCMMatrix A( aI.getRowCount(), aI.getColCount() );
	CCMVector b( A.getRowCount() ), x( A.getColCount() ), d( A.getColCount() );

	aI.initNewton( A, x, b );
	CCMMatrix AT = A.getTransposed();
	double dNorm = (AT*b).getNorm();
	checkNorm( dNorm );
	
	int iIter = 0;
	doLog( aI.getComponentNumber(), iIter, dNorm );
	while ( dNorm > aI.getPrecision() )
	{
		if ( iIter > aI.getMaxIterations() ) return false;
		aS.solve( A, b, d );
		x.saxpy( 1.0, d );
		aI.set( A, x, b );
		checkNorm( dNorm = (A.getTransposed()*b).getNorm() );
		iIter++;
		doLog( aI.getComponentNumber(), iIter, dNorm );
	}
	aI.transferResult( A, x, b );
	return true;
}
