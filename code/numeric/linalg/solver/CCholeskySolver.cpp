//CCholeskySolver.cpp: Implementierung der Klasse 
//CCholeskySolver.
//
///////////////////////////////////////////////////////////////////////////////
#include "CCholeskySolver.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "util/CEADSyException.h"



CCholeskySolver::CCholeskySolver()
{
}

CCholeskySolver::~CCholeskySolver()
{
}

void CCholeskySolver::solve( CCMMatrix& A, CCMVector& b, CCMVector& x, double dPrecision, int iMaxIter )
{
	decompose( A );
	CCMMatrix B = A.getTransposed();
    CCMVector y(A.getRowCount());
	solveLowerTriangular( A, b, y );
	solveUpperTriangular( B, y, x );
}

void CCholeskySolver::decompose(CCMMatrix& aM)
{
	int i, k, p, iRowCount = aM.getRowCount(), iColCount = aM.getColCount();
	double dSum;
	double* pdM = getData(aM);

	for (k = 0; k < iRowCount; k++)
	{
		dSum = 0.0;
		for (p = 0; p < k; p++) dSum += pdM[k * iColCount + p] * pdM[k * iColCount + p];

		if ((pdM[k * iColCount + k] - dSum) < 0.0) throw CEADSyException("MATRIX NOT POSITIVE DEFINIT");
		pdM[k * iColCount + k] = sqrt(pdM[k * iColCount + k] - dSum);
		if (fabs(pdM[k * iColCount + k]) < 1.0E-300) throw CEADSyException("MATRIX_NOT_REGULAR");

		for (i = k + 1; i < iColCount; i++)
		{
			dSum = 0.0;
			for (p = 0; p < k; p++)
				dSum += pdM[i * iColCount + p] * pdM[k * iColCount + p];
			pdM[i * iColCount + k] = (pdM[i * iColCount + k] - dSum) / pdM[k * iColCount + k];
		}
	}

	for (i = 0; i < iRowCount; i++)
		for (k = i + 1; k < iColCount; k++) pdM[i * iColCount + k] = 0.0;
}

