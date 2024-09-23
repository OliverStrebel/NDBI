//CLinearEquationSolver.cpp: Implementierung der Klasse 
//CLinearEquationSolver.
//
///////////////////////////////////////////////////////////////////////////////
#include "CLinearEquationSolver.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"


CLinearEquationSolver::CLinearEquationSolver()
{
}

CLinearEquationSolver::~CLinearEquationSolver()
{
}

void CLinearEquationSolver::solveLowerTriangular(CCMMatrix& A, CCMVector& b, CCMVector& x)
{
	int iRowCount = b.getDimension(), iColCount = A.getColCount();
	double* pdA = getData(A); double* pdb = getData(b); double* pdx = getData(x);

	for (int i = 0; i < iRowCount; i++)
	{
		pdx[i] = pdb[i];
		for (int j = 0; j < i; j++) pdx[i] -= (pdA[i * iColCount + j] * pdx[j]);
		pdx[i] /= pdA[i * iColCount + i];
	}
}

void CLinearEquationSolver::solveUpperTriangular(CCMMatrix& A, CCMVector& b, CCMVector& x)
{
	int iRowCount = A.getColCount(), iColCount = A.getColCount();
	double* pdA = getData(A); double* pdb = getData(b); double* pdx = getData(x);

	for (int i = iRowCount - 1; i >= 0; i--)
	{
		pdx[i] = pdb[i];
		for (int j = i + 1; j < iRowCount; j++) pdx[i] -= (pdA[i * iColCount + j] * pdx[j]);
		pdx[i] /= pdA[i * iColCount + i];
	}
}
