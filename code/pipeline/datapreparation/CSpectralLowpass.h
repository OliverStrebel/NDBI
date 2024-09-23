// CSpectralLowpass.h: Schnittstelle für die Klasse CSpectralLowpass.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CSPECTRALLOWPASS_H)
#define CSPECTRALLOWPASS_H


#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

class CPipelineData;
class CDEData;

class CSpectralLowpass : public CPipelineElement
{
public:
	CSpectralLowpass(CPipelineElement* pPE);
	virtual ~CSpectralLowpass();

	virtual CPipelineData* next();

protected:
	CSpectralLowpass(){};

private:
	void filterData(CPipelineData* pPD);

	std::vector<double> getFilterParameter(CPipelineData* pPD);
	CDEData* getData(CPipelineData* pPD);
	double getDistance(CDEData& aDED, std::pair<double*, double*>& cx, int iComp);
	double getEpsTurningPoint(std::vector<double>& eps, std::vector<double>& dist);
	double getEpsWithKneeMultiplier(std::vector<double>& eps, std::vector<double>& dist);
	void filterComponent(CDEData& aState, CDEData& aDeriv, double dEps, int iComp);
	CDEData* eliminateWindowEffect(CDEData* pDED);

	void ifft(std::vector<CDEData*>&, std::pair<double*, double*>& cx, double dEps, int iComp);

	void cleanUp(CDEData* pDED, std::vector<CDEData*>* pvD = 0, CDEData* pDEDOther = 0);
};

#endif //CSPECTRALLOWPASS_H