// CLotkaVolterra.h: Schnittstelle
//
//////////////////////////////////////////////////////////////////////

#if !defined(CLOTKAVOLTERRA_H)
#define CLOTKAVOLTERRA_H


#ifndef CEQUATIONSYSTEM_H
#include "CEquationSystem.h"
#endif

class CLotkaVolterra : public CEquationSystem
{
public:
	CLotkaVolterra();
	virtual ~CLotkaVolterra();

	virtual void setRegressionMatrix(CCMMatrix& A, int iRow, int iComp, int iDataRow, CPipelineData* pPD);
    virtual int getParameterCount(int iComp);
    virtual int getComponentCount(){return 2;};
};

#endif //CLOTKAVOLTERRA_H
