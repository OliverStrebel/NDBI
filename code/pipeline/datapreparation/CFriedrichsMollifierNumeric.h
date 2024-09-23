// CFriedrichsMollifierNumeric.h: Schnittstelle für die Klasse CFriedrichsMollifierNumeric.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CFRIEDRICHSMOLLIFIERNUMERIC_H)
#define CFRIEDRICHSMOLLIFIERNUMERIC_H


#ifndef __VECTOR__
#include <vector>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

#include <cmath>

class CPipelineData;
class CDEData;

class CFriedrichsMollifierNumeric : public CPipelineElement
{
public:
	CFriedrichsMollifierNumeric(CPipelineElement* pPE);
	virtual ~CFriedrichsMollifierNumeric();

	virtual CPipelineData* next();

protected:
	CFriedrichsMollifierNumeric(){};

private:
	void calculatePreparedData(CPipelineData* pPD);
	void createPreparedDEData(CPipelineData* pPD);
	void prepareDataPoint(CPipelineData* pPD, int iRow, int iCol, std::vector<double>& res, int iPos);

	double fmf(double x, double dNorm = 1.0){return dNorm*std::exp(-1.0/(1.0 - x*x));};
	
	void convolute(CDEData& aDED, int iPos, int iCol, std::vector<double>& res);
	double get_h(CDEData& aDED, int iPos);
};

#endif //CFRIEDRICHSMOLLIFIERNUMERIC_H