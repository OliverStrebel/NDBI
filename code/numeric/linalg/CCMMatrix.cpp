//CCMMatrix.cpp: Implementierung der Klasse CCMMatrix.
//
//////////////////////////////////////////////////////////////////////

#include "CCMMatrix.h"

#include "CCMVector.h"
#include "util/CEADSyException.h"

#include "math.h"

/**
 * This class stores the matrix data in a contiguous block of
 * memory by appending one row after the other. The column and 
 * row indexes are expected to be zero based.
 */
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCMMatrix::CCMMatrix( int iRowCount, int iColCount )
{
	m_pd= 0;
	if ( (iColCount < 1) || (iRowCount < 1) ) throw CEADSyException("Row- and columnn-count of a matrix cannot be smaller than 1");

	m_iColCount = iColCount;
	m_iRowCount = iRowCount;

	int iSize = getColCount()*getRowCount();
	m_pd = new double[iSize];
	
	for ( int i = 0; i < iSize; i++ ) m_pd[i] = 0.0;
}

CCMMatrix::CCMMatrix( const CCMMatrix& aM )
{
	m_iColCount = aM.m_iColCount;
	m_iRowCount = aM.m_iRowCount;

	int iSize = getColCount()*getRowCount();
	m_pd = new double[iSize];	
	for ( int i = 0; i < iSize; i++ ) m_pd[i] = aM.m_pd[i];
}

CCMMatrix::CCMMatrix( double* pd, int aRowCount, int aColCount )
{
	m_iColCount = aColCount;
	m_iRowCount = aRowCount;

	int iSize = getColCount()*getRowCount();
	m_pd = new double[iSize];	
	for ( int i = 0; i < iSize; i++ ) m_pd[i] = pd[i];
}

CCMMatrix::~CCMMatrix()
{
	if ( m_pd != 0 )
    { 
		delete []m_pd;
	    m_pd = 0;
    }
}

CCMMatrix& CCMMatrix::operator=(const CCMMatrix& aMatrix )
{
	if ( this == &aMatrix ) return *this;

	if ( m_pd != 0 ) delete []m_pd;
	m_pd = 0;
	int iSize = aMatrix.m_iColCount*aMatrix.m_iRowCount;

	m_pd = new double[iSize];
	m_iColCount = aMatrix.m_iColCount;
	m_iRowCount = aMatrix.m_iRowCount;
	for ( int i = 0; i < iSize; i++ ) m_pd[i] = aMatrix.m_pd[i];

	return *this;
}

CCMMatrix CCMMatrix::operator+( const CCMMatrix& aM )
{
	if ( (aM.m_iColCount != getColCount()) || (aM.m_iRowCount != getRowCount()) )
			throw CEADSyException("CCMMatrix::operator+: number of rows and columns does not match");

	CCMMatrix aMR = 
		CCMMatrix( aM.m_iRowCount, aM.m_iColCount );

	int iColSize = getColCount();
	int iRowSize = getRowCount();
	int iRow = 0;
	int iCol = 0;
	int iRowOffset = 0;

	for ( iRow = 0; iRow < iRowSize; iRow++ )
	{
		iRowOffset = iRow*iColSize; 
		for ( iCol = 0; iCol < iColSize; iCol++ )
		{
			aMR.m_pd[iRowOffset] = m_pd[iRowOffset] + aM.m_pd[iRowOffset];
			iRowOffset++;
		}
	}

	return aMR;
}

CCMMatrix CCMMatrix::operator*( const CCMMatrix& aM )
{
	if ( (aM.m_iRowCount != getColCount()) )
		throw CEADSyException("CCMMatrix::operator*(CCMMatrix=& ): number of rows and columns does not match");

	int i,j,k;
	CCMMatrix aMR = CCMMatrix( m_iRowCount, aM.m_iColCount );

	for ( i = 0; i < aMR.m_iRowCount; i++ )
	{
		int iOffsetR = i*aMR.m_iColCount;
		int iOffset = i*m_iColCount;
		for ( k = 0; k < aMR.m_iColCount; k++ )
			for ( j = 0; j < aM.m_iRowCount; j++ )
				aMR.m_pd[iOffsetR + k] += m_pd[iOffset + j]*aM.m_pd[j*aM.m_iColCount + k];
	}

	return aMR;
}

CCMVector CCMMatrix::operator*( CCMVector& aV )
{
	if ( (aV.getDimension() != getColCount()) )
		throw CEADSyException("CCMMatrix::operator*(CCMVector=& ): number of rows and columns does not match");

	int i,j;
	CCMVector aVR = CCMVector( m_iRowCount );

	for ( i = 0; i < m_iRowCount; i++ )
	{
		double d = 0.0;
		for ( j = 0; j < m_iColCount; j++ ) d+=m_pd[i*m_iColCount + j]*aV.getElement( j );
		aVR.setElement( i, d );
	}

	return aVR;
}

CCMMatrix CCMMatrix::operator*( double dS )
{
	int i = 0;
	int iSize = getColCount()*getColCount();
	CCMMatrix aMR = CCMMatrix( getRowCount(), getColCount() );
	for ( i = 0; i < iSize; i++ ) aMR.m_pd[i] = dS*m_pd[i];
	return aMR;
}

double CCMMatrix::getElement( int iRow, int iCol )
{
	if ( (iRow < 0) || (iRow > m_iRowCount - 1) || (iCol < 0) || (iCol > m_iColCount - 1) )
		 throw CEADSyException("CCMMatrix::getElement: Index out of bounds");

	return m_pd[iRow*getColCount() + iCol];
}
	
void CCMMatrix::setElement( int iRow, int iCol, double dValue )
{
	if ( (iRow < 0) || (iRow > m_iRowCount - 1) || (iCol < 0) || (iCol > m_iColCount - 1) )
		 throw CEADSyException("CCMMatrix::setElement: Index out of bounds");

	m_pd[iRow*getColCount() + iCol] = dValue;
}

CCMMatrix CCMMatrix::getTransposed()
{
	CCMMatrix aMR = CCMMatrix( m_iColCount, m_iRowCount );

	for ( int i = 0; i < m_iColCount; i++ )
		for ( int j = 0; j < m_iRowCount; j++ )
			aMR.m_pd[i*m_iRowCount + j] = m_pd[j*m_iColCount + i];

	return aMR;
}

void CCMMatrix::transpose()
{
	double dTransposed;
	for ( int i = 0; i < m_iRowCount; i++ )
	{
		for ( int j = i+1; j < m_iColCount; j++ )
		{
			dTransposed = m_pd[i*m_iColCount + j];
			m_pd[i*m_iColCount + j] = m_pd[j*m_iColCount + i];
			m_pd[j*m_iColCount + i] = dTransposed;
		}
	}
}

void CCMMatrix::setZero()
{
	for ( int i = 0; i < m_iColCount; i++ )
		for ( int j = 0; j < m_iRowCount; j++ ) m_pd[j*m_iColCount + i] = 0.0;
}

void CCMMatrix::setUnit()
{
	for ( int i = 0; i < m_iColCount; i++ )
	{
		for ( int j = 0; j < m_iRowCount; j++ )
			if ( i == j ) m_pd[j*m_iColCount + i] = 1.0;
			else m_pd[j*m_iColCount + i] = 0.0;
	}
}

CCMVector CCMMatrix::getColumn( int iCol )
{
	if ( (iCol < 0) || (iCol > m_iColCount - 1) )
		 throw CEADSyException("CCMMatrix::getColumn: Index out of bounds");

	CCMVector vReturn = CCMVector( m_iRowCount );
	for ( int j = 0; j < m_iRowCount; j++ )
		vReturn.setElement( j, m_pd[j*m_iColCount + iCol] );

	return vReturn;
}

void CCMMatrix::setColumn( int iCol, CCMVector& aV )
{
	if ( (iCol < 0) || (iCol > m_iColCount - 1) || (aV.getDimension() > m_iColCount))
		 throw CEADSyException("CCMMatrix::setColumn: Index out of bounds");

	for ( int j = 0; j < aV.getDimension(); j++ ) m_pd[j*m_iColCount + iCol] = aV.getElement( j );
}

void CCMMatrix::setRow(int iRow, CCMVector& aV)
{
    if ((iRow < 0) || (iRow > m_iRowCount - 1) || (aV.getDimension() != m_iColCount))
        throw CEADSyException("CCMMatrix::setRow: Index out of bounds");

    for (int j = 0; j < aV.getDimension(); j++) m_pd[iRow*m_iColCount + j] = aV.getElement(j);
}

void CCMMatrix::getRow( CCMVector& aV, int iRow )
{
	if ( (iRow < 0) || (iRow > m_iRowCount - 1) ) throw CEADSyException("CCMMatrix::getRow: Index out of bounds");
	for ( int j = 0; j < m_iColCount; j++ ) aV.setElement( j, m_pd[iRow*m_iColCount + j] );
}

void CCMMatrix::getCol( CCMVector& aV, int iCol )
{
	if ( (iCol < 0) || (iCol > m_iColCount - 1) ) throw CEADSyException("CCMMatrix::getCol: Index out of bounds");
	for ( int j = 0; j < m_iRowCount; j++ ) aV.setElement( j, m_pd[j*m_iColCount + iCol] );
}

CCMMatrix CCMMatrix::getSubMatrix( int aStartRow, int anEndRow, 
		                           int aStartCol, int anEndCol)
{
	if ( (aStartRow < 0) || (aStartRow > m_iRowCount - 2) || (anEndRow < 1) || (anEndRow > m_iRowCount - 1) || 
		 (aStartCol < 0) || (aStartCol > m_iColCount - 2) || (anEndCol < 1) || (anEndCol > m_iColCount - 1) )
		throw CEADSyException("CCMMatrix::getSubmatrix: Index out of bounds");

	int iRowDiff = anEndRow - aStartRow + 1;
	int iColDiff = anEndCol - aStartCol + 1;

	if ( (iRowDiff < 2) || (iColDiff < 2) ) throw CEADSyException("CCMMatrix::getCow: Index out of bounds");;
	
	CCMMatrix M = CCMMatrix( iRowDiff, iColDiff );
	for ( int iRow = 0; iRow < iRowDiff; iRow++ )
		for ( int iCol = 0; iCol < iColDiff; iCol++ )
			M.m_pd[iRow*iColDiff + iCol] = m_pd[(aStartRow + iRow)*m_iColCount + aStartCol + iCol];

	return M;
}

CCMMatrix CCMMatrix::getSubMatrix(int aRow,int aCol)
{
	if ((aRow < 0) || (aRow > m_iRowCount - 1) || (aCol < 0) || (aCol > m_iColCount - 1))
		throw CEADSyException("CCMMatrix::getSubMatrix: Index out of bounds");

	CCMMatrix M = CCMMatrix(m_iRowCount - 1, m_iColCount - 1);
	int r = 0;
	for (int iRow = 0; iRow < m_iRowCount; iRow++)
	{
		if (iRow != aRow)
		{
			int c = 0;
			for (int iCol = 0; iCol < m_iColCount; iCol++)
				if (iCol != aCol)
				{
					M.setElement(r, c, getElement(iRow, iCol));
					c++;
				}
			r++;
		}
	}

	return M;
}

bool CCMMatrix::checkDiagonal( double aMaxAbsValue )
{
	int iEnd = m_iColCount;
	if ( m_iRowCount < iEnd ) iEnd = m_iRowCount;

	for ( int j = 0; j < iEnd; j++ )
		if ( fabs(m_pd[j*m_iColCount + j]) < aMaxAbsValue ) return false;
	return true;
}

bool CCMMatrix::isSymmetric( double aMaxAbsDiff )
{
	if ( m_iColCount != m_iRowCount ) return false;

	for ( int i = 0; i < m_iRowCount; i++ )
		for ( int j = i + 1; j < m_iRowCount; j++ )
		if ( fabs( m_pd[j*m_iColCount + i] - m_pd[i*m_iColCount + j] ) > aMaxAbsDiff ) return false;
	return true;
}

double CCMMatrix::trace()
{
	int i = 0;
	double dResult = 0.0;

	while ( (i < m_iColCount) && (i < m_iRowCount) )
	{
		dResult+=m_pd[i*m_iColCount + i];
		i++;
	}

	return dResult;
}
	
void CCMMatrix::add( CCMMatrix& aM, double aFactor )
{
	if ( (aM.m_iRowCount != getRowCount()) || (aM.m_iColCount != getColCount()) )
		throw CEADSyException("CCMMatrix::add: Matrix size mismatch");

	int iEnd = m_iColCount*m_iRowCount;
	for ( int i = 0; i < iEnd; i++ ) m_pd[i] = m_pd[i] + aFactor*aM.m_pd[i];
}

void CCMMatrix::mul( double aFactor )
{
	int iEnd = m_iColCount*m_iRowCount;
	for ( int i = 0; i < iEnd; i++ ) m_pd[i] = aFactor*m_pd[i];
}

void CCMMatrix::set( CCMVector& u, CCMVector& v )
{
	if ( (v.getDimension() != getColCount()) || (u.getDimension() != getRowCount()) )
		throw CEADSyException("CCMMatrix::set(CCMVector & u, CCMVector & v): Dimensions of vectors must be equal");

	for ( int i = 0; i < m_iRowCount; i++ )
		for ( int j = 0; j < m_iColCount; j++ )
			m_pd[i*m_iColCount + j] = u.getElement( i )*v.getElement( j );
}

void CCMMatrix::set( CCMMatrix& aM )
{
	if ( (aM.getColCount() != getColCount()) || (aM.getRowCount() != getRowCount()) )
		throw CEADSyException("CCMMatrix::set(CCMMatrix & aM): Dimensions of matrixes must be equal");

	for ( int i = 0; i < m_iRowCount; i++ )
		for ( int j = 0; j < m_iColCount; j++ )
			m_pd[i*m_iColCount + j] = aM.m_pd[i*m_iColCount + j];
}

double CCMMatrix::getFrobeniusNorm()
{
	double dSum = 0.0;
	for ( int i = 0; i < m_iRowCount; i++ )
		for ( int j = 0; j < m_iColCount; j++ )
			dSum+=(pow( m_pd[i*m_iColCount + j], 2.0 )); 
	return sqrt( dSum );
}

double CCMMatrix::rowTimesVector(int iRow, CCMVector& x, int iColExcluded)
{
    double dSum = 0.0;
    for (int j = 0; j < m_iColCount; j++)
        if (j != iColExcluded) dSum += m_pd[iRow*m_iColCount + j]*x.getElement(j);
    return dSum;
}
