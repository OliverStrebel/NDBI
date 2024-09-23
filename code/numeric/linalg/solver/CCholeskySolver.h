// CCholeskySolver.h: Schnittstelle für die Klasse CCholeskySolver.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CCHOLESKYSOLVER_H)
#define CCHOLESKYSOLVER_H

#ifndef CLINEAREQUATIONSOLVER_H
#include "numeric/linalg/solver/CLinearEquationSolver.h"
#endif

class CCMMatrix;
class CCMVector;

class CCholeskySolver : public CLinearEquationSolver
{
public:
	CCholeskySolver();
	virtual ~CCholeskySolver();

	void solve( CCMMatrix& A, CCMVector& b, CCMVector& x, double dPrecision = 1.0E-012, int iMaxIter = 10000 );
private:
	void decompose(CCMMatrix& aM);

};

#endif //CCHOLESKYSOLVER_H