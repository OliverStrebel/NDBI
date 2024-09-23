// CErrorSquareNorm_NES.h: Schnittstelle für die Klasse CErrorSquareNorm_NES.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CERRORSQUARENORM_NES_H)
#define CERRORSQUARENORM_NES_H

#ifndef CNEWTONEQUATIONSOLVER_H
#include "CNewtonEquationSolver.h"
#endif

class CCMMatrix;
class CCMVector;
class CNewtonEquationInterface;
class CLinearEquationSolver;

class CErrorSquareNorm_NES : public CNewtonEquationSolver
{
public:
	CErrorSquareNorm_NES( bool bLog = false );
	virtual ~CErrorSquareNorm_NES();

	virtual bool solve( CNewtonEquationInterface& aI, CLinearEquationSolver& aS);
	virtual CCMVector solveEquation( CCMMatrix& aM, CCMVector& aV ) = 0;
};

#endif // !defined(CERRORSQUARENORM_NES_H)
