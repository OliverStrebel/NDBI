// CStepDownSubsampler.h: Schnittstelle für die Klasse CStepDownSubsampler.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CSTEPDOWNSUBSAMPLER_H)
#define CSTEPDOWNSUBSAMPLER_H


#ifndef __VECTOR__
#include <vector>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

#ifndef CENTROPYMINCALCULATOR_H
#include "pipeline/estimation/CEntropyMinCalculator.h"
#endif

#ifndef CMAPLMINCALCULATOR_H
#include "pipeline/estimation/CMaxELLHCalculator.h"
#endif


class CPipelineData;
class CCMVector;
class CEcuyer40014RNG;

class CStepDownSubsampler : public CPipelineElement
{
public:
	CStepDownSubsampler(CPipelineElement* pPE, bool useEffect = true);
	virtual ~CStepDownSubsampler();

	virtual CPipelineData* next();

protected:
	CStepDownSubsampler(): m_EMC(false), m_MMC(false), m_bUseEffect(false) {};

private:
	//1st index: ode component. 2nd index: elimination step. 3rd index: index of subsample vector
    typedef std::vector<std::vector<std::vector<CCMVector>>> SubSamples;

	std::vector<bool> init(CPipelineData* pPD, SubSamples& s);
	void prepareStep(CPipelineData* pPD, SubSamples& s, int iComp);
	void calculateSamples(CPipelineData* pPD, SubSamples& s, int iComp);
	void solve(CPipelineData* pPD, SubSamples& s, int iComp, CEcuyer40014RNG& aRNG);
	bool evaluateSamples(CPipelineData* pPD, SubSamples& s, int iComp);
	void storeResults(CPipelineData* pPD, SubSamples& s, int iMin, int iComp, CCMVector& means, CCMVector& sd);

    CEntropyMinCalculator m_EMC;
    CMaxELLHCalculator m_MMC;
    bool m_bUseEffect;
};

#endif //CSTEPDOWNSUBSAMPLER_H