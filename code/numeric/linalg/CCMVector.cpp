//CCMVector.cpp: Implementierung der Klasse CCMVector.
//
//////////////////////////////////////////////////////////////////////

#include "CCMVector.h"

#include "util/CEADSyException.h"
#include "CCMMatrix.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCMVector::CCMVector()
{
	m_pd = 0; 
	m_iDimension = 0;
}

CCMVector::CCMVector( int iDimension )
{
	m_iDimension = iDimension;

	m_pd = new double[m_iDimension];	
	for ( int i = 0; i < m_iDimension; i++ )
		m_pd[i] = 0.0;
}

CCMVector::CCMVector( const CCMVector& aV )
{
	m_iDimension = aV.m_iDimension;

	m_pd = new double[m_iDimension];	
	for ( int i = 0; i < m_iDimension; i++ )
		m_pd[i] = aV.m_pd[i];
}

CCMVector::CCMVector( double* pd, int iDimension )
{
	m_iDimension = iDimension;

	m_pd = new double[m_iDimension];	
	for ( int i = 0; i < m_iDimension; i++ )
		m_pd[i] = pd[i];
}

CCMVector::~CCMVector()
{
	if ( m_pd != 0 )
		delete []m_pd;
	m_pd = 0;
}

CCMVector CCMVector::operator=(const CCMVector& aVector )
{
	if ( this == &aVector ) return *this;

	if ( m_pd != 0 ) delete[m_iDimension] m_pd;

	m_iDimension = aVector.m_iDimension;
	m_pd = new double[m_iDimension];
	for ( int i = 0; i < m_iDimension; i++ )
		m_pd[i] = aVector.m_pd[i];

	return *this;
}

CCMVector CCMVector::operator+( const CCMVector& aV )
{
	if ( (aV.m_iDimension != getDimension()) ) throw CEADSyException("CCMVector::operator+:  dimension mismatch");
	CCMVector aVR = CCMVector( aV.m_iDimension );
	for ( int i = 0; i < m_iDimension; i++ ) aVR.m_pd[i] = m_pd[i] + aV.m_pd[i];
	return aVR;
}

CCMVector CCMVector::operator-( const CCMVector& aV )
{
	if ( (aV.m_iDimension != getDimension()) ) throw CEADSyException("CCMVector::operator-:  dimension mismatch");
	CCMVector aVR = CCMVector( aV.m_iDimension );
	for ( int i = 0; i < m_iDimension; i++ ) aVR.m_pd[i] = m_pd[i] - aV.m_pd[i];
	return aVR;
}

double CCMVector::operator*( const CCMVector& aV )
{
	if ( (aV.m_iDimension != getDimension()) ) throw CEADSyException("CCMVector::operator*(CCMVector): dimension mismatch");
	double dRet = 0.0;
	for ( int i = 0; i < m_iDimension; i++ ) dRet+=(m_pd[i]*aV.m_pd[i]);
	return dRet;
}

CCMVector CCMVector::operator*( double dS )
{
	CCMVector aVR = CCMVector( m_iDimension );
	for ( int i = 0; i < m_iDimension; i++ ) aVR.m_pd[i] = dS*m_pd[i];
	return aVR;
}

CCMVector CCMVector::operator/(const CCMVector& aV)
{
	CCMVector aVR = CCMVector(m_iDimension);
	for (int i = 0; i < m_iDimension; i++) aVR.m_pd[i] = m_pd[i]/aV.m_pd[i];
	return aVR;
}


void CCMVector::normalize()
{
	double dNorm = getNorm();
	for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = m_pd[i]/dNorm;
}

double CCMVector::getNorm()
{
	double dResult = 0.0;
	for ( int i = 0; i < m_iDimension; i++ ) dResult+=(m_pd[i]*m_pd[i]);
	return sqrt( dResult );
}

double CCMVector::getMaxNorm()
{
	double dResult = 1.0E-300;

	for ( int i = 0; i < m_iDimension; i++ )
		if ( fabs( m_pd[i] ) > dResult ) dResult = fabs( m_pd[i] );

	return dResult;
}

double CCMVector::getSquaredNorm()
{
	double dResult = 0.0;
	for ( int i = 0; i < m_iDimension; i++ ) dResult+=(m_pd[i]*m_pd[i]);
	return dResult;
}

double CCMVector::getElement( int iPos )
{
	if ( (iPos < 0) || (iPos > m_iDimension - 1) ) throw CEADSyException("CCMVector::getElement:  index out of bounds");

	return m_pd[iPos];
}
	
void CCMVector::setElement( int iPos, double dValue )
{
	if ( (iPos < 0) || (iPos > m_iDimension - 1) ) throw CEADSyException("CCMVector::setElement:  index out of bounds");

	m_pd[iPos] = dValue;
}

void CCMVector::addValue( int iPos, double dValue )
{
	if ( (iPos < 0) || (iPos > m_iDimension - 1) ) throw CEADSyException("CCMVector::addValue:  index out of bounds");
	m_pd[iPos]+=dValue;
}

void CCMVector::saxpy( const double aS, const CCMVector& aV, bool bMultiplyMe )
{
	if ( (aV.m_iDimension != getDimension()) ) throw CEADSyException("CCMVector::saxpy: vector dimension mismatch");;

	if ( !bMultiplyMe ) for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = m_pd[i] + aS*aV.m_pd[i];
	else for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = aS*m_pd[i] + aV.m_pd[i];
}

void CCMVector::setZero()
{
	for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = 0.0;
}

void CCMVector::setZero( int aStart, int anEnd )
{
	if ( !(aStart < getDimension()) || !(anEnd < getDimension()) ) throw CEADSyException("CCMVector::getElement:  index out of bounds");

	for ( int i = aStart; i <= anEnd; i++ ) m_pd[i] = 0.0;
}

void CCMVector::setOne()
{
	for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = 1.0;
}

void CCMVector::set( CCMMatrix& aM, CCMVector& aV, double aFactor )
{
	if ((aV.getDimension() != aM.getColCount()) || (getDimension() != aM.getRowCount()))
		throw CEADSyException("CCMVector::set:  matrix vector dimension mismatch");

	double* pM = getData( aM );
	setZero();
	for ( int i = 0; i < aM.getRowCount(); i++ )
		for ( int j = 0; j < aM.getColCount(); j++ )
			m_pd[i]+= pM[i*aM.getColCount() + j]*aV.m_pd[j]*aFactor;
}

void CCMVector::set( CCMVector& aV, double aS )
{
	if (aV.getDimension() != this->getDimension()) throw CEADSyException("CCMVector::set: vector dimension mismatch");

	for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = aV.m_pd[i]*aS;
}

void CCMVector::set( CCMVector& aV )
{
	if (aV.getDimension() != this->getDimension()) throw CEADSyException("CCMVector::set:  vector dimension mismatch");

	for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = aV.m_pd[i];
}

void CCMVector::mul( double aS )
{
	for ( int i = 0; i < m_iDimension; i++ ) m_pd[i] = m_pd[i]*aS;
}
