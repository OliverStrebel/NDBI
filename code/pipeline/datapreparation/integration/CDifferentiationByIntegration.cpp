//CDifferentiationByIntegration.cpp: Implementierung der Klasse CDifferentiationByIntegration.
///////////////////////////////////////////////////////////////////////////////
#include "CDifferentiationByIntegration.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"
#include "pipeline/datapreparation/integration/CMultivariateHermiteSeries.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"
#include "numeric/linalg/solver/CGivensNES.h"

#include "util/CLog.h"
#include "util/CFileWriter.h"


CDifferentiationByIntegration::CDifferentiationByIntegration(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CDifferentiationByIntegration::~CDifferentiationByIntegration()
{
}

CPipelineData* CDifferentiationByIntegration::next()
{
    CPipelineData* pPD = getPrevious()->next();
    CDEData& aDED = pPD->getDEData();
    CMultivariateHermiteSeries s(cfg().getDifPolynomialDegree(), aDED.getCompCount());
    CCMMatrix Aint(aDED.getRowCount()-1, s.getLength());
    setIntegrationSnippetMatrix(Aint, aDED, s);
   
    std::vector<CCMVector> coefficients;
    for (int iComp = 0; iComp < aDED.getCompCount(); iComp++) coefficients.push_back(regression(Aint, aDED, iComp));
    pPD->addPreparedDEData(calculateState(Aint, aDED, s, coefficients));
    pPD->addPreparedDEData(calculateDeriv(Aint, pPD->getPreparedDEData(0), s, coefficients));

	return pPD;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//next() helpers
void CDifferentiationByIntegration::setIntegrationSnippetMatrix(CCMMatrix& Aint, CDEData& aDED, CMultivariateHermiteSeries& s)
{
    CCMMatrix A(aDED.getRowCount(), Aint.getColCount());
    std::vector<double> data(aDED.getCompCount(), 0.0);
    for (int i = 0; i < A.getRowCount(); i++)
    {
        for (int iComp = 0; iComp < aDED.getCompCount(); iComp++) 
            data[iComp] = aDED.get(iComp + aDED.getDependendVariableOffset(), i);
        for (int iMonome = 0; iMonome < Aint.getColCount(); iMonome++)
            A.setElement(i, iMonome, s.evaluateSummand(iMonome, data));
    }

     for( int i = 0; i < Aint.getRowCount(); i++)
         for (int j = 0; j < Aint.getColCount(); j++)
             Aint.setElement(i, j, 0.5*(aDED.get(0, i+1) - aDED.get(0, i))*(A.getElement(i,j) + A.getElement(i+1,j)) );//Trapezoid rule
}

CCMVector CDifferentiationByIntegration::regression(CCMMatrix& Aint, CDEData& aDED, int iComp)
{
    CGivensNES nes;
    CCMMatrix A(Aint.getRowCount(), Aint.getColCount());
    CCMVector b(A.getRowCount()), x(A.getColCount()), row(A.getColCount());
    std::vector<CCMVector> samples;
    int iStep = (Aint.getRowCount() - 1) / cfg().getNumRegressions(), iStart = 0, iCol = iComp + aDED.getDependendVariableOffset();
    for (int i = 0; i < cfg().getNumRegressions(); i++)
    {
        row.setZero();
        for (int k = iStart; k < A.getRowCount(); k++)
        {
            addRow(Aint, row, k);
            A.setRow(k, row);
            b.setElement(k, aDED.get(iCol, k + 1) - aDED.get(iCol, iStart));
        }
        row.setZero();
        for (int k = iStart - 1; k >= 0; k--)
        {
            addRow(Aint, row, k, -1.0);
            A.setRow(k, row);
            b.setElement(k, aDED.get(iCol, k) - aDED.get(iCol, iStart));
        }
        nes.solve(A, b, x);
        if (checkVector(x)) samples.push_back(x);
        iStart += iStep;
    }
    return calculateMean(samples);
}

CDEData* CDifferentiationByIntegration::calculateState(CCMMatrix& Aint, CDEData& aDED, CMultivariateHermiteSeries& s, std::vector<CCMVector>& coefficients)
{
    CDEData* pDEDRet = new CDEData(aDED.getRowCount(), aDED.getColCount(), aDED.getDependendVariableOffset());
    int iInterval = cfg().getHalfIntervalInDataPoints(), iDependendPos = aDED.getDependendVariableOffset();;
    for (int i_x0 = 0; i_x0 < aDED.getRowCount(); i_x0++)
    {
        std::vector<double> averages;
        for (int iComp = 0; iComp < aDED.getCompCount(); iComp++)
        {
            double dAverage = aDED.get(iComp + iDependendPos, i_x0);
            int iAverageCounter = 0, iOffset = iInterval;
            if (i_x0 + iOffset > aDED.getRowCount() - 1) iOffset = aDED.getRowCount() - 1 - i_x0;//correct the offset at the end of the datafile
            //Integration from the left
            for (int iDataPointsUsed = 1; iDataPointsUsed <= iOffset; iDataPointsUsed++)
            {
                double dx_t = aDED.get(iComp + iDependendPos, i_x0 + iDataPointsUsed);//Starting point for integration
                for (int k = 0; k < iDataPointsUsed; k++) 
                    for (int j = 0; j < coefficients[0].getDimension(); j++) dx_t -= coefficients[0].getElement(j) * Aint.getElement(i_x0 + k, j);
                dAverage += dx_t;
                iAverageCounter++;
            }
            //Integration from the left
            iOffset = -iInterval;
            if (i_x0 + iOffset <= 0) iOffset = -i_x0;//correct the offset at the beginning of the datafile
            for (int iDataPointsUsed = iOffset; iDataPointsUsed < 0; iDataPointsUsed++)
            {
                double dx_t = aDED.get(iComp+ iDependendPos, i_x0 + iDataPointsUsed);//Starting point for integration
                for (int k = iDataPointsUsed; k < 0; k++) 
                    for (int j = 0; j < coefficients[0].getDimension(); j++) dx_t += coefficients[0].getElement(j)*Aint.getElement(i_x0 + k, j);
                dAverage += dx_t;
                iAverageCounter++;
            }
            averages.push_back(dAverage/(1.0 * iAverageCounter + 1.0));
        }
        for (int iCompPos = 0; iCompPos < iDependendPos; iCompPos++) pDEDRet->set(iCompPos, i_x0, aDED.get(iCompPos, i_x0));
        for (int iCompPos = iDependendPos; iCompPos < aDED.getColCount(); iCompPos++) pDEDRet->set(iCompPos, i_x0, averages[iCompPos - iDependendPos]);
    }
    return pDEDRet;
}

CDEData* CDifferentiationByIntegration::calculateDeriv(CCMMatrix& Aint, CDEData& aDEDState, CMultivariateHermiteSeries& s, std::vector<CCMVector>& coefficients)
{
    CDEData* pDEDRet = new CDEData(aDEDState.getRowCount(), aDEDState.getColCount(), aDEDState.getDependendVariableOffset());
    std::vector<double> data(aDEDState.getCompCount(), 0.0);
    for (int i_x0 = 0; i_x0 < aDEDState.getRowCount(); i_x0++)
    {
        for (int iComp = 0; iComp < aDEDState.getCompCount(); iComp++)
            data[iComp] = aDEDState.get(iComp + aDEDState.getDependendVariableOffset(), i_x0);

        for (int iComp = 0; iComp < aDEDState.getCompCount(); iComp++)
        {
            double dDeriv = 0.0;
            for (int iMonome = 0; iMonome < s.getLength(); iMonome++) 
                dDeriv = dDeriv + (coefficients[iComp].getElement(iMonome)*s.evaluateSummand(iMonome, data));
            pDEDRet->set(iComp + aDEDState.getDependendVariableOffset(), i_x0, dDeriv);
        }
        for (int iAx = 0; iAx < aDEDState.getDependendVariableOffset(); iAx++) pDEDRet->set(iAx, i_x0, aDEDState.get(iAx, i_x0));
    }
    return pDEDRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//regression() helpers
bool CDifferentiationByIntegration::checkVector(CCMVector& x)
{
    for (int j = 0; j < x.getDimension(); j++)
        if (_isnan(x.getElement(j)) != 0) return false;
    return true;
}

void CDifferentiationByIntegration::addRow(CCMMatrix& Aint, CCMVector& row, int k, double dir)
{
    for (int iCol = 0; iCol < Aint.getColCount(); iCol++) 
        row.setElement(iCol, row.getElement(iCol) + dir*Aint.getElement(k, iCol));
}

CCMVector CDifferentiationByIntegration::calculateMean(std::vector<CCMVector>& v)
{
    CCMVector mean(v[0].getDimension());
    for (size_t k = 0; k < v.size(); k++) mean = mean + v[k];
    mean = mean*(1.0/(1.0*v.size()));
    return mean;
}
