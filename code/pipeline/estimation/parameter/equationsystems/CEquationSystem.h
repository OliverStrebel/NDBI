// CEquationSystem.h: Schnittstelle
//
//////////////////////////////////////////////////////////////////////

#if !defined(CEQUATIONSYSTEM_H)
#define CEQUATIONSYSTEM_H

#ifndef CCMVECTOR_H
#include "numeric/linalg/CCMVector.h"
#endif


class CDEData;
class CCMMatrix;
class CPipelineData;

class CEquationSystem
{
public:
	CEquationSystem();
	virtual ~CEquationSystem();

	virtual void setRegressand(CCMVector& b, int iRow, double d){b.setElement(iRow, d);};
	virtual void setRegressionMatrix(CCMMatrix& A, int iRow, int iComp, int iDataRow, CPipelineData* pPD) = 0;
    virtual int getParameterCount(int iComp) = 0;
    virtual int getComponentCount() = 0;
};

#endif //CEQUATIONSYSTEM_H
