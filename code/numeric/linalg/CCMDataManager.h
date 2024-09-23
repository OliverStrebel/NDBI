// CCMDataManager.h: Schnittstelle für die Klasse CCMDataManager.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CCMDATAMANAGER_H)
#define CCMDATAMANAGER_H

class CCMMatrix;
class CCMVector;

class CCMDataManager 
{
	friend class CLog;
protected:
	double* getData( CCMMatrix& aM );
	double* getData( CCMVector& aV );
	CCMMatrix createCMMatrix( double* pd, int aRowCount, int aColCount );
	CCMVector createCMVector( double* pd, int aDimension );
	CCMMatrix createCMSquareUnitMatrix( int aRowCount );

	static void check( double dNum, char* pc, double dLevel = 1.0E-100 );
	static void checkIter( int i, int iMax, char* pc );
};

#endif //CCMDATAMANAGER_H