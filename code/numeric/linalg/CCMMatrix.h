// CCMMatrix.h: Schnittstelle für die Klasse CCMMatrix.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CCMMATRIX_H)
#define CCMMATRIX_H


class CCMVector;

class CCMMatrix
{
	friend class CCMDataManager;

public:
	CCMMatrix() { m_iRowCount = 0; m_iColCount = 0; m_pd = 0; };
	CCMMatrix( int iRowCount, int iColCount );
	CCMMatrix( const CCMMatrix& aM );
	~CCMMatrix();

	CCMMatrix& operator=( const CCMMatrix& aMatrix );

	CCMMatrix operator+( const CCMMatrix& aMatrix );
	CCMMatrix operator*( const CCMMatrix& aMatrix );
	CCMVector operator*( CCMVector& aVector );
	CCMMatrix operator*( double dScalar );

	double getElement( int iRow, int iCol );
	void setElement( int iRow, int iCol, double dValue );

	int getRowCount(){return m_iRowCount;};
	int getColCount(){return m_iColCount;};
	CCMMatrix getTransposed();
	void transpose();
	void setZero();
	void setUnit();
	CCMMatrix getSubMatrix( int aStartRow, int anEndRow, 
		                    int aStartCol, int anEndCol);
	CCMMatrix getSubMatrix(int aRow, int aCol);
	CCMVector getColumn( int iCol );
	void setColumn( int iCol, CCMVector& aV );
    void setRow(int iRow, CCMVector& aV);
	void getRow( CCMVector& aV, int iRow );
	void getCol( CCMVector& aV, int iCol );
	bool checkDiagonal( double aMaxAbsValue );
	bool isSymmetric( double aMaxAbsDiff );
	double trace();

	void add( CCMMatrix& aM, double aFactor = 1.0 );
	void mul( double aFactor );
	void set( CCMVector& u, CCMVector& v );
	void set( CCMMatrix& aM );

	double getFrobeniusNorm();
    double rowTimesVector(int iRow, CCMVector& x, int iColExcluded = -1);

private:
	CCMMatrix( double* pd, int aRowCount, int aColCount );
	double* m_pd;

	int m_iColCount;
	int m_iRowCount;
};

#endif //CCMMATRIX_H