#include "CMaxELLHCalculator.h"

#include "pipeline/datastructures/CDEData.h"
#include "pipeline/datastructures/CResultData.h"
#include "pipeline/datastructures/CPipelineData.h"

#include "pipeline/de/CDE.h"

#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/CCMVector.h"

#include "util/CLog.h"

#include <numbers>
#include <limits>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////////////////////////////////////////////////


CMaxELLHCalculator::CMaxELLHCalculator(bool bLog)
	: m_bLog(bLog)
{
}

CMaxELLHCalculator::~CMaxELLHCalculator()
{
}

int CMaxELLHCalculator::getMinIndex(std::vector<CCMVector>& samples, CCMVector& means, CPipelineData* pPD, int iComp)
{
	if (means.getDimension() == 1) return -1;

    CCMMatrix A(cfg().getSampleSize(), means.getDimension());
    CCMVector b(cfg().getSampleSize()), maplAfter(A.getColCount());
    double dSigma = setRegressionProblem(A, means, b, pPD, iComp);
    double dOldELLH = getELLH(samples, A, b, dSigma);

    for (int j = 0; j < A.getColCount(); j++) maplAfter.setElement(j, getELLH(samples, A, b, dSigma, j));
    if (hasLargerELLH(dOldELLH, maplAfter)) return getLargestAfterRemoval(maplAfter);

    return -1;
}

bool CMaxELLHCalculator::hasLargerELLH(double dOldMAPL, CCMVector& maplAfter)
{
    for (int i = 0; i < maplAfter.getDimension(); i++)
        if(maplAfter.getElement(i) > dOldMAPL) return true;
    return false;
}

int CMaxELLHCalculator::getLargestAfterRemoval(CCMVector& maplAfter)
{
    double dMaxELLH = -1.0*std::numeric_limits<double>::max();
    int iMax = -1;
    for (int i = 0; i < maplAfter.getDimension(); i++)
        if (maplAfter.getElement(i) > dMaxELLH)
        {
            iMax = i;
            dMaxELLH = maplAfter.getElement(i);
        }
    return iMax;
}

double CMaxELLHCalculator::getELLH(std::vector<CCMVector>& samples, CCMMatrix& A, CCMVector& b, double dSigma, int iExcludedCol)
{
    double dELLH = 0.0;
    for (size_t s = 0; s < samples.size(); s++)
    {
        double dL = 1.0;
        for (int i = 0; i < A.getRowCount(); i++)
        {
            double dExp = 0.5*pow(A.rowTimesVector(i, samples[s], iExcludedCol) - b.getElement(i), 2.0)/pow(dSigma, 2.0);
            dL = dL*exp(-dExp)/(sqrt(2.0 * std::numbers::pi)*dSigma);
        }
        dELLH +=dL;
    }
    double dDimension = 1.0*A.getColCount();
    if (iExcludedCol > -1) dDimension -=1.0;
    return pow(dELLH/samples.size(), 1.0/dDimension);
}

double CMaxELLHCalculator::setRegressionProblem(CCMMatrix& A, CCMVector& x, CCMVector& b, CPipelineData* pPD, int iComp)
{

    int iSamplePos = 0, iStepSize = pPD->getPreparedDEData(0).getRowCount() / cfg().getSampleSize();
    CDE& aDE = pPD->getResultData().getDE(iComp);
    for (int m = 0; m < cfg().getSampleSize(); m++)
    {
        aDE.setRHS(m, iComp, b, pPD, iSamplePos);
        aDE.setMatrix(m, A, pPD, iSamplePos);
        iSamplePos += iStepSize;
    }
    return getStdDev(A, x, b);
}

double CMaxELLHCalculator::getStdDev(CCMMatrix& A, CCMVector& x, CCMVector& b)
{
    CCMVector r(b.getDimension());
    r.set(A, x);
    return sqrt((r - b).getSquaredNorm()/(1.0*r.getDimension()));
}
