// CResidualSquareNorm_NES.h: Schnittstelle für die Klasse CResidualSquareNorm_NES.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CRESIDUALSQUARENORM_NES_H)
#define CRESIDUALSQUARENORM_NES_H

#ifndef CNEWTONEQUATIONSOLVER_H
#include "CNewtonEquationSolver.h"
#endif

class CCMMatrix;
class CCMVector;
class CNewtonEquationInterface;
class CLinearEquationSolver;

class CResidualSquareNorm_NES : public CNewtonEquationSolver
{
public:
	CResidualSquareNorm_NES( bool bLog = false );
	virtual ~CResidualSquareNorm_NES();

	virtual bool solve( CNewtonEquationInterface& aI, CLinearEquationSolver& aS);
};

#endif // !defined(CRESIDUALSQUARENORM_NES_H)
