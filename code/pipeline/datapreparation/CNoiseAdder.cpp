//CNoiseAdder.cpp: Implementierung der Klasse CNoiseAdder.
///////////////////////////////////////////////////////////////////////////////
#include "CNoiseAdder.h"

#include <float.h>

#include "numeric/stochastic/randomnumbers/CBoxMillerRNG.h"
#include "numeric/stochastic/randomnumbers/CEcuyer40014RNG.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"

#include <limits>


CNoiseAdder::CNoiseAdder(CPipelineElement* pPE, double aNoisePercent)
: CPipelineElement(pPE), m_dNoisePercent(aNoisePercent)
{
}

CNoiseAdder::~CNoiseAdder()
{
}

CPipelineData* CNoiseAdder::next()
{
    CPipelineData* pD = getPrevious()->next();
    addNoise(pD->getDEData());
    pD->setNoiseAdded(m_dNoisePercent);
    return pD;
}

void CNoiseAdder::addNoise(CDEData& aDED)
{
    for (int i = aDED.getDependendVariableOffset(); i < aDED.getColCount(); i++)
    {
        double dElongation = getElongation(aDED, i);
        CBoxMillerRNG rng(m_dNoisePercent*dElongation, new CEcuyer40014RNG(1234567890), new CEcuyer40014RNG(987654321));
        for (int j = 0; j < aDED.getRowCount(); j++) aDED.set(i, j, aDED.get(i, j) + rng.G());
    }
}

double CNoiseAdder::getElongation(CDEData& aDED, int iCol)
{
    double dMax = -std::numeric_limits<double>::max(), dMin = std::numeric_limits<double>::max();
    for (int i = 0; i < aDED.getRowCount(); i++)
    {
        double dTest = aDED.get(iCol, i);
        if (dTest < dMin) dMin = dTest;
        if (dTest > dMax) dMax = dTest;
    }
    return dMax - dMin;
}

