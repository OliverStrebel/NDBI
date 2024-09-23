//CLeaveOneOutEstimator.cpp: Implementierung der Klasse CLeaveOneOutEstimator.
///////////////////////////////////////////////////////////////////////////////
#include "CLeaveOneOutEstimator.h"

#include <typeinfo>

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "numeric/linalg/solver/CGivensNES.h"

#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CResultData.h"
#include "pipeline/datastructures/CDEData.h"

#include "pipeline/de/CDE.h"

#include "pipeline/estimation/parameter/equationsystems/CEquationSystem.h"
#include "pipeline/estimation/parameter/equationsystems/CLotkaVolterra.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"


CLeaveOneOutEstimator::CLeaveOneOutEstimator(CPipelineElement* pPE, bool useEffect)
: CPipelineElement(pPE), m_EMC(false)
{
}

CLeaveOneOutEstimator::~CLeaveOneOutEstimator()
{
}

CPipelineData* CLeaveOneOutEstimator::next()
{
    CPipelineData* pPD = getPrevious()->next();
    CDEData& aDEDState = pPD->getPreparedDEData(0);

    CLotkaVolterra eqs;
    SubSamples s = allocateSubSamples(aDEDState, eqs);
    for (int iComp = 0; iComp < aDEDState.getCompCount(); iComp++)
    {
        CCMMatrix A(aDEDState.getRowCount() - 1, eqs.getParameterCount(iComp));
        CCMVector b(aDEDState.getRowCount() - 1);
        for (int iPos = 0; iPos < aDEDState.getRowCount(); iPos++)
        {
            setData(pPD, A, b, iComp, iPos, eqs);
            solve(A, b, s, iComp);
        }
        storeResult(pPD->getResultData(), s, iComp);
    }
    return pPD;
}

std::vector<std::vector<CCMVector>> CLeaveOneOutEstimator::allocateSubSamples(CDEData& aDEDState, CEquationSystem& aEQS)
{
    SubSamples s;
    for (int i = 0; i < aEQS.getComponentCount(); i++)
    {
        std::vector<CCMVector> v;
        s.push_back(v);
    }
    return s;
}

void CLeaveOneOutEstimator::setData(CPipelineData* pPD, CCMMatrix& A, CCMVector& b, int iComp, int iPosSkipped, CEquationSystem& aEQS)
{
    CDEData& aDEDState = pPD->getPreparedDEData(0);
    CDEData& aDEDDeriv = pPD->getPreparedDEData(1);
    int iPos = 0, iOffset = aDEDState.getDependendVariableOffset();
    for (int i = 0; i < aDEDState.getRowCount(); i++)
    {
        if (i != iPosSkipped)
        {
            aEQS.setRegressand(b, iPos, aDEDDeriv.get(iOffset + iComp, i));
            aEQS.setRegressionMatrix(A, iPos, iComp, i, pPD);
            iPos++;
        }
    }
}

void CLeaveOneOutEstimator::solve(CCMMatrix& A, CCMVector& b, SubSamples& s, int iComp)
{
    CCMVector d(A.getColCount());
    CGivensNES nes;
    nes.solve(A, b, d);
    for (int p = 0; p < d.getDimension(); p++) if (_isnan(d.getElement(p)) != 0) return;
    s[iComp].push_back(d);
    for (int i = 0; i < d.getDimension(); i++)
    {
        CLog::debug(iComp); CLog::debug(" "); CLog::debug(i); CLog::debug(" "); CLog::debugEndl(d.getElement(i)); 
    }
    CLog::debugEndl("");

}

void CLeaveOneOutEstimator::storeResult(CResultData& aRD, SubSamples& s, int iComp)
{
    CCMVector means = m_EMC.getMean(s[iComp]);
    CCMVector stddevs = m_EMC.getSD(s[iComp], means);
    aRD.prepareStep(iComp, "");
    for ( int j = 0; j < means.getDimension(); j++)
        aRD.addSampleResults(iComp, j, means.getElement(j), stddevs.getElement(j));
}
