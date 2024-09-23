// CNewtonEquationSolver.h: Schnittstelle für die Klasse CNewtonEquationSolver.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CNEWTONEQUATIONSOLVER_H)
#define CNEWTONEQUATIONSOLVER_H

class CCMMatrix;
class CCMVector;
class CNewtonEquationInterface;
class CLinearEquationSolver;

class CNewtonEquationSolver
{
public:
	CNewtonEquationSolver( bool bLog = false );
	virtual ~CNewtonEquationSolver();

	virtual bool solve( CNewtonEquationInterface& aI, CLinearEquationSolver& aS) = 0;

	void setLog( bool bLog ){m_bLog = bLog;};
	bool getLog(){return m_bLog;};

protected:
	void doLog( int iCompNo, int iIter, double dNorm );
	void checkNorm( double dNorm );

private:
	bool m_bLog;
};

#endif // !defined(CNEWTONEQUATIONSOLVER_H)
