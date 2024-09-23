//CCMLUDecomposer.cpp: Implementierung der Klasse CCMLUDecomposer.
//
///////////////////////////////////////////////////////////////////////////////
#include "CCMLUDecomposer.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

void CCMLUDecomposer::decompose( CCMMatrix& aM )
{
	int i, j, k;
	double eta = 0.0;

	CCMLUDecomposer e;
	CCMVector w( aM.getRowCount() );
	double* pd = e.getData( w );
	double* pdM = e.getData( aM );

	int iRowCount = aM.getRowCount();
	int iColCount = aM.getColCount();

	for ( k = 0; k < iRowCount; k++ )
	{
		for ( j = k+1; j < iRowCount; j++ )pd[j] = pdM[k*iColCount + j];

		for ( i = k+1; i < iRowCount; i++ )
		{
			eta = pdM[i*iColCount + k]/pdM[k*iColCount + k];
			pdM[i*iColCount + k] = eta;
			for ( j = k+1; j < iRowCount; j++ )pdM[i*iColCount + j]-=(eta*pd[j]);
		}
	}
}

