// CTangentRegression.h: Schnittstelle für die Klasse CTangentRegression.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CTANGENTREGRESSION_H)
#define CTANGENTREGRESSION_H


#ifndef __VECTOR__
#include <vector>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

class CPipelineData;
class CDEData;

class CTangentRegression : public CPipelineElement
{
public:
	CTangentRegression(CPipelineElement* pPE);
	virtual ~CTangentRegression();

	virtual CPipelineData* next();

protected:
	CTangentRegression(){};

private:
	void calculatePreparedData(CPipelineData* pPD);
	void createPreparedDEData(CPipelineData* pPD);
	void prepareDataPoint(CPipelineData* pPD, int iCoordinate, int iPreparedRow);
};

#endif //CTANGENTREGRESSION_H