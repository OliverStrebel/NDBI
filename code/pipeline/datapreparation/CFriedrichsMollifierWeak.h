// CFriedrichsMollifierWeak.h: Schnittstelle für die Klasse CFriedrichsMollifierWeak.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CFRIEDRICHSMOLLIFIERWEAK_H)
#define CFRIEDRICHSMOLLIFIERWEAK_H


#ifndef __VECTOR__
#include <vector>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

#include <cmath>

class CPipelineData;
class CDEData;

class CFriedrichsMollifierWeak : public CPipelineElement
{
public:
	CFriedrichsMollifierWeak(CPipelineElement* pPE);
	virtual ~CFriedrichsMollifierWeak();

	virtual CPipelineData* next();

protected:
	CFriedrichsMollifierWeak() {};

private:
	void calculatePreparedData(CPipelineData* pPD);
	void createPreparedDEData(CPipelineData* pPD);
	void prepareDataPoint(CPipelineData* pPD, int iRow, int iCol, std::vector<double>& res, int iPos);

	double fmf(double x){return std::exp(-1.0/(1.0 - x*x));};
	//Differentiation generates 3 minus signs which are canceled by the minus from the integration by parts formula
	double fmfd(double x){return std::exp(-1.0/(1.0 - x*x))*(2.0*x/pow(1.0 - x*x, 2.0));};

	void integrate(CDEData& aDED, int iPos, int iCol, std::vector<double>& res);
	double get_h(CDEData& aDED, int iPos);
};

#endif //CFRIEDRICHSMOLLIFIERWEAK_H