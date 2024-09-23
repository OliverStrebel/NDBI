// CFiniteDiff.h: Schnittstelle für die Klasse CFiniteDiff.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CFINITEDIFF_H)
#define CFINITEDIFF_H


#ifndef __VECTOR__
#include <vector>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

class CPipelineData;
class CDEData;

class CFiniteDiff : public CPipelineElement
{
public:
	CFiniteDiff(CPipelineElement* pPE);
	virtual ~CFiniteDiff();

	virtual CPipelineData* next();

protected:
	CFiniteDiff(){};

private:
	void calculatePreparedData(CPipelineData* pPD);
	void createPreparedDEData(CPipelineData* pPD);
	void prepareDataPoint(CPipelineData* pPD, int iCoordinate, int iPreparedRow);
};

#endif //CFINITEDIFF_H