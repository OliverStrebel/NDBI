//CStepDownSubsampler.cpp: Implementierung der Klasse CStepDownSubsampler.
///////////////////////////////////////////////////////////////////////////////
#include "CStepDownSubsampler.h"

#include <typeinfo>

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "numeric/linalg/solver/CGivensNES.h"

#include "numeric/stochastic/randomnumbers/CEcuyer40014RNG.h"

#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CResultData.h"
#include "pipeline/datastructures/CDEData.h"

#include "pipeline/de/CDE.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"


CStepDownSubsampler::CStepDownSubsampler(CPipelineElement* pPE, bool useEffect)
: CPipelineElement(pPE), m_EMC(false), m_MMC(false), m_bUseEffect(useEffect)
{
}

CStepDownSubsampler::~CStepDownSubsampler()
{
}

CPipelineData* CStepDownSubsampler::next()
{
    CPipelineData* pPD = getPrevious()->next();
    SubSamples s;
    std::vector<bool> hasSuperfluousParameters  = init(pPD, s);
    for (unsigned int iComp = 0; iComp < hasSuperfluousParameters.size(); iComp++)
        while (hasSuperfluousParameters[iComp])
        {
            prepareStep(pPD, s, iComp);
            calculateSamples(pPD, s, iComp);
            hasSuperfluousParameters[iComp] = evaluateSamples(pPD, s, iComp);
        }
    return pPD;
}

std::vector<bool> CStepDownSubsampler::init(CPipelineData* pPD, SubSamples& s)
{
    std::vector<bool> hasSuperfluousParameters;
    CResultData& aRD = pPD->getResultData();
    for (int i = 0; i < aRD.getDECount(); i++)
    {
        std::vector<std::vector<CCMVector>> v;
        s.push_back(v);
        hasSuperfluousParameters.push_back(aRD.getDE(i).isEstimable());
    }
    return hasSuperfluousParameters;
}

void CStepDownSubsampler::prepareStep(CPipelineData* pPD, SubSamples& s, int iComp)
{
    std::vector<CCMVector> v;
    s[iComp].push_back(v);
    pPD->getResultData().prepareStep(iComp, typeid(this).name());
}

void CStepDownSubsampler::calculateSamples(CPipelineData* pPD, SubSamples& s, int iComp)
{
    CEcuyer40014RNG myRNGTrial(123456789);
    if (pPD->getResultData().getDE(iComp).isEstimable())
        for (int i = 0; i < cfg().getSampleCount(); i++)
        {
            try { solve(pPD, s, iComp, myRNGTrial); }
            catch (...) {};
        }
}

void CStepDownSubsampler::solve(CPipelineData* pPD, SubSamples& s, int iComp, CEcuyer40014RNG& aRNGTrial)
{
    CDE& aDE = pPD->getResultData().getDE(iComp);
    CCMMatrix A(cfg().getSampleSize(), aDE.getParameterCount());
    CCMVector b(cfg().getSampleSize());

    for (int m = 0; m  < cfg().getSampleSize(); m++)
    {
        int iSamplePos = aRNGTrial.next() % pPD->getPreparedDEData(0).getRowCount();
        aDE.setRHS(m, iComp, b, pPD, iSamplePos);
        aDE.setMatrix(m, A, pPD, iSamplePos);
    }

    CCMVector d(A.getColCount());
    CGivensNES nes;
    nes.solve(A, b, d);
    for (int p = 0; p < d.getDimension(); p++) if (_isnan(d.getElement(p)) != 0) return;

    int iLast = (int) s[iComp].size() - 1;
    s[iComp][iLast].push_back(d);
}

bool CStepDownSubsampler::evaluateSamples(CPipelineData* pPD, SubSamples& s, int iComp)
{
    bool bHasSuperfluousParameters = false;
    int iLast = (int) s[iComp].size() - 1;
    CResultData& r = pPD->getResultData();

    if (s[iComp][iLast].size() > 1)
    {
        CCMVector means = m_EMC.getMean(s[iComp][iLast]);
        CCMVector stddevs = m_EMC.getSD(s[iComp][iLast], means);
        int iMin = -1;
        //iMin = m_MMC.getMinIndex(ss[iComp][iLast], means, pPD, iComp);
        if (m_bUseEffect) iMin = m_EMC.getMinIndex(means, stddevs, cfg().getSigmaScale());
        else iMin = m_EMC.getMinIndex(s[iComp][iLast], means, cfg().getSigmaScale());

        if (iMin > -1)
        {
            storeResults(pPD, s, iMin, iComp, means, stddevs);
            bHasSuperfluousParameters = true;
        }
        else if (r.getDE(iComp).getAvailableParameterCount() == r.getDE(iComp).getParameterCount())
            r.storeError(iComp, CResultData::Error::NO_MONOME_ELIMINATED);
        else storeResults(pPD, s, iMin, iComp, means, stddevs);//Elimination was not successful -> last result
    }
    else r.storeError(iComp, CResultData::Error::VARIANCE_UNCALCULATABLE);

    return bHasSuperfluousParameters;
}

//Step down specific
void CStepDownSubsampler::storeResults(
    CPipelineData* pPD
    , SubSamples& s
    , int iMin
    , int iComp
    , CCMVector& means
    , CCMVector& sd)
{
    int k = 0, iLast = (int) s[iComp].size() - 1;
    CResultData& r = pPD->getResultData();
    for (int j = 0; j < r.getDE(iComp).getAvailableParameterCount(); j++)
        if (r.getDE(iComp).isActive(j))
        {
            if (k != iMin) r.addSampleResults(iComp, j, means.getElement(k), sd.getElement(k));
            else r.getDE(iComp).setActivity(j, false);
            k++;
        }

    if (k == 0) r.storeError(iComp,CResultData::Error::NO_MONOME_FOUND);//RHS of equation is empty
}


