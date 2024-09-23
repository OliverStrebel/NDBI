// CDifferentiationByIntegration.h: Schnittstelle für die Klasse CDifferentiationByIntegration.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CDIFFERENTIATIONBYINTEGRATION_H)
#define CDIFFERENTIATIONBYINTEGRATION_H


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif


class CPipelineData;
class CDEData;
class CCMMatrix;
class CCMVector;
class CMultivariateHermiteSeries;

class CDifferentiationByIntegration : public CPipelineElement
{
public:
	CDifferentiationByIntegration(CPipelineElement* pPE);
	virtual ~CDifferentiationByIntegration();

	virtual CPipelineData* next();

protected:
	CDifferentiationByIntegration(){};

private:
	void setIntegrationSnippetMatrix(CCMMatrix& A, CDEData& aDED, CMultivariateHermiteSeries& s);
    CCMVector regression(CCMMatrix& Aint, CDEData& aDED, int iComp);
    bool checkVector(CCMVector& aV);
    void addRow(CCMMatrix& Aint, CCMVector& row, int k, double dir = 1.0);
    CCMVector calculateMean(std::vector<CCMVector>& v);

    CDEData* calculateState(CCMMatrix& A, CDEData& aDED, CMultivariateHermiteSeries& s, std::vector<CCMVector>& coefficients);
    CDEData* calculateDeriv(CCMMatrix& A, CDEData& aDEDState, CMultivariateHermiteSeries& s, std::vector<CCMVector>& coefficients);
};

#endif //CDIFFERENTIATIONBYINTEGRATION_H