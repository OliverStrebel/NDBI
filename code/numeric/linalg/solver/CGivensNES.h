// CGivensNES.h: Schnittstelle für die Klasse CGivensNES.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CGIVENSNES_H)
#define CGIVENSNES_H

#ifndef CNORMALEQUATIONSOLVER_H
#include "numeric/linalg/solver/CLinearEquationSolver.h"
#endif

class CCMMatrix;
class CCMVector;

class CGivensNES : public CLinearEquationSolver
{
public:
	CGivensNES();
	virtual ~CGivensNES();

	void solve( CCMMatrix& A, CCMVector& b, CCMVector& x, double dPrecision = 1.0E-012, int iMaxIter = 10000 );
	void decompose(CCMMatrix& aM, CCMMatrix& aQ, bool bAllocate = true);

private:
 	void givens(double da, double db, double* pdc, double* pds);
};

#endif //CGIVENSNES_H