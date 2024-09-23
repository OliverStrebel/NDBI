#include "CErrorSquareNorm_NES.h"

#include "CNewtonEquationInterface.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "numeric/linalg/solver/CLinearEquationSolver.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CErrorSquareNorm_NES::CErrorSquareNorm_NES( bool bLog )
{
	setLog( bLog );
}

CErrorSquareNorm_NES::~CErrorSquareNorm_NES()
{
}

bool CErrorSquareNorm_NES::solve( CNewtonEquationInterface& aI, CLinearEquationSolver& aS)
{
	CCMMatrix A( aI.getRowCount(), aI.getColCount() );
	CCMVector b( A.getRowCount() ), x( A.getColCount() ), d( A.getColCount() );
	aI.initNewton( A, x, b );

	double dNorm = 1.0;
	int iIter = 0;
	doLog( aI.getComponentNumber(), iIter, dNorm );
	
	while ( dNorm > aI.getPrecision() )
	{
		if ( iIter > aI.getMaxIterations() ) return false;
		aS.solve(A, b, d);
		x.saxpy( 1.0, d );
		aI.set( A, x, b );
		checkNorm( dNorm = d.getNorm() );
		iIter++;
		doLog( aI.getComponentNumber(), iIter, dNorm );
	}
	aI.transferResult( A, x, b );
	return true;
}
