// CNoiseAdder.h: Schnittstelle für die Klasse CNoiseAdder.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CNOISEADDER_H)
#define CNOISEADDER_H

#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif


class CPipelineData;
class CDEData;

class CNoiseAdder : public CPipelineElement
{
public:
	CNoiseAdder(CPipelineElement* pPE, double aNoisePercent);
	virtual ~CNoiseAdder();

	virtual CPipelineData* next();
	void setNoiseLevel(double aNoisePercent){m_dNoisePercent = aNoisePercent;};

protected:
	CNoiseAdder(): m_dNoisePercent(0.0){};

private:
    double m_dNoisePercent;

	void addNoise(CDEData& aDED);
	double getElongation(CDEData& aDED, int iCol);
};

#endif //CNOISEADDER_H