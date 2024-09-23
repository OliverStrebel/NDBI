//CFriedrichsMollifierWeak.cpp: Implementierung der Klasse CFriedrichsMollifierWeak.
///////////////////////////////////////////////////////////////////////////////
#include "CFriedrichsMollifierWeak.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"

#include "util/CLog.h"

CFriedrichsMollifierWeak::CFriedrichsMollifierWeak(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CFriedrichsMollifierWeak::~CFriedrichsMollifierWeak()
{
}

CPipelineData* CFriedrichsMollifierWeak::next()
{
	if (cfg().getIntegrationIntersteps() < 0) throw CEADSyException("Use an appropriate number of integration steps for the integration with the Friedrich mollifier");
    CLog::debug(cfg().getHalfIntervalInDataPoints()); CLog::debug(" "); CLog::debugEndl(cfg().getIntegrationIntersteps());
    CPipelineData* pPD = getPrevious()->next();
	calculatePreparedData(pPD);
    return pPD;
}

void CFriedrichsMollifierWeak::calculatePreparedData(CPipelineData* pPD)
{
	createPreparedDEData(pPD);
	std::vector<double> res(2, 0.0);
	int iStartCol = pPD->getDEData().getDependendVariableOffset(), iEndCol = pPD->getDEData().getColCount();
	int iStartRow = cfg().getHalfIntervalInDataPoints(), iEndRow = pPD->getDEData().getRowCount() - cfg().getHalfIntervalInDataPoints();
	for (int iCol = iStartCol; iCol < iEndCol; iCol++)
	{
	    int iPos = 0;
		for (int iRow = iStartRow; iRow < iEndRow; iRow++) 
		{
		    prepareDataPoint(pPD, iRow, iCol, res, iPos);
			iPos++;
		}
	}
}

void CFriedrichsMollifierWeak::createPreparedDEData(CPipelineData* pPD)
{
	CDEData& aDED = pPD->getDEData();
	int iSize = aDED.getRowCount() - 2*cfg().getHalfIntervalInDataPoints();
	for (int i = 0; i <= 1; i++)
	{
		CDEData* pDED = new CDEData(iSize, aDED.getColCount(), aDED.getDependendVariableOffset());
		for (int k = 0; k < aDED.getDependendVariableOffset(); k++)
			for (int j = 0; j < iSize; j++) pDED->set(k, j, aDED.get(k, j + cfg().getHalfIntervalInDataPoints()));
		pPD->addPreparedDEData(pDED);
	}
}

void CFriedrichsMollifierWeak::prepareDataPoint(CPipelineData* pPD, int iRow, int iCol, std::vector<double>& res, int iPos)
{
	CDEData& aDED = pPD->getDEData();
	CDEData& aDEDState = pPD->getPreparedDEData(0);
	CDEData& aDEDDeriv = pPD->getPreparedDEData(1);
    integrate(aDED, iRow, iCol, res);
	aDEDState.set(iCol, iPos, res[0]);
	aDEDDeriv.set(iCol, iPos, res[1]);
}

//Works with nonconstant time steps
void CFriedrichsMollifierWeak::integrate(CDEData& aDED, int iRow, int iCol, std::vector<double>& res)
{
    double dState = 0.0, dDiff = 0.0, dNorm = 0.0, dLeft = 0.0, dRight = 0.0, dh = get_h(aDED, iRow);
	int iStart = iRow - cfg().getHalfIntervalInDataPoints() + 1, iEnd = iRow + cfg().getHalfIntervalInDataPoints(), iE = cfg().getIntegrationIntersteps() + 1;
	double dRow = aDED.get(0, iRow), dEps = 0.000000001;
	for (int i = iStart; i <= iEnd; i++)
	{
		double dS = aDED.get(0, i - 1), dE = aDED.get(0, i);
		double dt = (dE - dS)/(1.0*iE);
		for (int k = 1; k <= iE; k++)
		{
		    double dD_i = aDED.get(iCol, i), dD_i_1 = aDED.get(iCol, i - 1);
			double dLower = dD_i_1 + (dD_i - dD_i_1)*(dt*(k-1))/(dE - dS);//linear interpolated data value
			double dUpper = dD_i_1 + (dD_i - dD_i_1)*(dt*k)/(dE - dS);//linear interpolated data value
            double dLeft = (dS + dt*(k-1) - dRow)/dh;
            if (fabs(dLeft + 1.0) < dEps) dLeft = (dS + dt*(k-1) + dt*0.99 - dRow)/dh;
            double dRight = (dS + dt*k - dRow)/dh;
            if (fabs(dRight - 1.0) < dEps) dRight= (dS + dt*k - dt*0.99 - dRow)/dh;

			dState += 0.5*dt*(fmf(dLeft)*dLower + fmf(dRight)*dUpper);//Trapezoid rule
			dDiff += 0.5*dt*(fmfd(dLeft)*dLower + fmfd(dRight)*dUpper);//Trapezoid rule
			dNorm += 0.5*dt*(fmf(dLeft) + fmf(dRight));//Trapezoid rule
		}
	}
	res[0] = dState/dNorm;
	res[1] = dDiff*(2.0/(aDED.get(0, iEnd) - aDED.get(0, iStart)))/dNorm;
}

double CFriedrichsMollifierWeak::get_h(CDEData& aDED, int iPos)
{
	double dh = aDED.get(0, iPos + cfg().getHalfIntervalInDataPoints()) - aDED.get(0, iPos);
	if (dh < aDED.get(0, iPos) - aDED.get(0, iPos - cfg().getHalfIntervalInDataPoints())) dh = aDED.get(0, iPos) - aDED.get(0, iPos - cfg().getHalfIntervalInDataPoints());
	return dh;
}
