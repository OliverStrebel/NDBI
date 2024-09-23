// CCMVector.h: Schnittstelle für die Klasse CCMVector.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CCMVECTOR_H)
#define CCMVECTOR_H

#ifndef CCMDATAMANAGER_H
#include "CCMDataManager.h"
#endif

class CCMMatrix;

class CCMVector : public CCMDataManager
{
friend class CCMDataManager;

public:
	CCMVector();
	CCMVector( int iDimension );
	CCMVector( const CCMVector& aVector );
	~CCMVector();

	CCMVector operator=( const CCMVector& aVector );
	CCMVector operator+( const CCMVector& aVector );
	CCMVector operator-( const CCMVector& aVector );

	double operator*( const CCMVector& pVector );
	CCMVector operator*( double dScalar );
	CCMVector operator/(const CCMVector& pVector);

	double getElement( int iPos );
	void setElement( int iPos, double dValue );
	void addValue( int iPos, double dValue );

	int getDimension(){return m_iDimension;};

	double getNorm();
	double getMaxNorm();
	double getSquaredNorm();
	
	void normalize();
	void setZero();
	void setZero( int aStart, int anEnd );
	void setOne();
	void saxpy( const double aS, const CCMVector& aV, bool bMultiplyMe = false );
	void set( CCMMatrix& aM, CCMVector& aV, double aS = 1.0 );
	void set( CCMVector& aV, double aS );
	void set( CCMVector& aV );
	void mul( double aS );

private:
	CCMVector( double* pd, int aDimension );
	double* m_pd;

	int m_iDimension;
};

#endif //CCMVECTOR_H
