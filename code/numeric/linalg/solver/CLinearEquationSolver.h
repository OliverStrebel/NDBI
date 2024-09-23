// CLinearEquationSolver.h: Schnittstelle für die Klasse CLinearEquationSolver.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CLINEAREQUATIONSOLVER_H)
#define CLINEAREQUATIONSOLVER_H


#ifndef CCMDATAMANAGER_H
#include "numeric/linalg/CCMDataManager.h"
#endif

class CCMMatrix;
class CCMVector;

class CLinearEquationSolver : public CCMDataManager
{
public:
	CLinearEquationSolver();
	virtual ~CLinearEquationSolver();

	virtual void solve( CCMMatrix& A, CCMVector& b, CCMVector& x,
		double dPrecision = 1.0E-012, int iMaxIter = 10000 ) = 0;

protected:
	void solveLowerTriangular(CCMMatrix& aM, CCMVector& aV, CCMVector& aR);
	void solveUpperTriangular(CCMMatrix& aM, CCMVector& aV, CCMVector& ar);
};

#endif //CLINEAREQUATIONSOLVER_H