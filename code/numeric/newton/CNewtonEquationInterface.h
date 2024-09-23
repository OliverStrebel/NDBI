// CNewtonEquationInterface.h: Schnittstelle für die Klasse CNewtonEquationInterface.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CNEWTONEQUATIONINTERFACE_H)
#define CNEWTONEQUATIONINTERFACE_H

class CCMMatrix;
class CCMVector;

class CNewtonEquationInterface
{
public:
	CNewtonEquationInterface();
	virtual ~CNewtonEquationInterface();

	virtual void initNewton( CCMMatrix& A, CCMVector& x, CCMVector& b ) = 0;
	virtual void set( CCMMatrix& A, CCMVector& x, CCMVector& b ) = 0;
	virtual void transferResult( CCMMatrix& A, CCMVector& x, CCMVector& b ) = 0;

	virtual int getMaxIterations() = 0;
	virtual double getPrecision() = 0;
	virtual int getRowCount() = 0;
	virtual int getColCount() = 0;
	
	virtual int getComponentNumber(){return -1;};
};

#endif // !defined(CNEWTONEQUATIONINTERFACE_H)
