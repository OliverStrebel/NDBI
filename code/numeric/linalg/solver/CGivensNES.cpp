//CGivensNES.cpp: Implementierung der Klasse 
//CGivensNES.
//
///////////////////////////////////////////////////////////////////////////////
#include "CGivensNES.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"


CGivensNES::CGivensNES()
{
}

CGivensNES::~CGivensNES()
{
}

void CGivensNES::solve( CCMMatrix& aM, CCMVector& aV, CCMVector& x, double dPrecision, int iMaxIter )
{
	CCMMatrix QT( 2, 2 );
	decompose( aM, QT );
	CCMVector b = QT*aV;
	solveUpperTriangular( aM, b, x  );
}

void CGivensNES::decompose(CCMMatrix& aM, CCMMatrix& aQ, bool bAllocate)
{
	int i, j, k, iRowCount = aM.getRowCount(), iColCount = aM.getColCount();

	if (bAllocate) aQ = createCMSquareUnitMatrix(aM.getRowCount());
	int iSquareCount = aQ.getColCount();

	double* pdM = getData(aM);
	double* pdQ = getData(aQ);
	double dc = 0.0, ds = 0.0, di_1 = 0.0, di = 0.0;

	for (j = 0; j < iColCount; j++)
	{
		for (i = iRowCount - 1; i > j; i--)
		{
			givens(pdM[(i - 1) * iColCount + j], pdM[i * iColCount + j], &dc, &ds);
			for (k = j; k < iColCount; k++)
			{
				di_1 = dc * pdM[(i - 1) * iColCount + k] - ds * pdM[i * iColCount + k];
				di = ds * pdM[(i - 1) * iColCount + k] + dc * pdM[i * iColCount + k];
				pdM[(i - 1) * iColCount + k] = di_1;
				pdM[i * iColCount + k] = di;
			}

			for (k = 0; k < iSquareCount; k++)
			{
				di_1 = dc * pdQ[(i - 1) * iSquareCount + k] - ds * pdQ[i * iSquareCount + k];
				di = ds * pdQ[(i - 1) * iSquareCount + k] + dc * pdQ[i * iSquareCount + k];
				pdQ[(i - 1) * iSquareCount + k] = di_1;
				pdQ[i * iSquareCount + k] = di;
			}
		}
	}
}

void CGivensNES::givens(double da, double db, double* pdc, double* pds)
{
	if (fabs(db) < 1.0E-100)
	{
		*pdc = 1.0;
		*pds = 0.0;
	}
	else if (fabs(da) < 1.0E-100)
	{
		*pdc = 0.0;
		*pds = 1.0;
	}
	else
	{
		if (fabs(db) > fabs(da))
		{
			double dt = -da / db;
			*pds = 1 / sqrt(1 + dt * dt);
			*pdc = (*pds) * dt;
		}
		else
		{
			double dt = -db / da;
			*pdc = 1 / sqrt(1 + dt * dt);
			*pds = (*pdc) * dt;
		}
	}
}

