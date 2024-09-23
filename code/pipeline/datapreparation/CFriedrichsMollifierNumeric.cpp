//CFriedrichsMollifierNumeric.cpp: Implementierung der Klasse CFriedrichsMollifierNumeric.
///////////////////////////////////////////////////////////////////////////////
#include "CFriedrichsMollifierNumeric.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"

#include "util/CLog.h"

CFriedrichsMollifierNumeric::CFriedrichsMollifierNumeric(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CFriedrichsMollifierNumeric::~CFriedrichsMollifierNumeric()
{
}

/*
Parameteroptimierung
    h
    Schrittweite der Integration
	Schrittweite bei der Bildung finiter Differenzen
*/
CPipelineData* CFriedrichsMollifierNumeric::next()
{
	if (cfg().getIntegrationIntersteps() < 0) throw CEADSyException("Use an appropriate number of integration steps for the convolution with the Friedrich mollifier");
    CPipelineData* pPD = getPrevious()->next();
	calculatePreparedData(pPD);
    return pPD;
}

void CFriedrichsMollifierNumeric::calculatePreparedData(CPipelineData* pPD)
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

void CFriedrichsMollifierNumeric::createPreparedDEData(CPipelineData* pPD)
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

void CFriedrichsMollifierNumeric::prepareDataPoint(CPipelineData* pPD, int iRow, int iCol, std::vector<double>& res, int iPos)
{
	CDEData& aDED = pPD->getDEData();
	CDEData& aDEDState = pPD->getPreparedDEData(0);
	CDEData& aDEDDeriv = pPD->getPreparedDEData(1);
    convolute(aDED, iRow, iCol, res);
	aDEDState.set(iCol, iPos, res[0]);
	aDEDDeriv.set(iCol, iPos, res[1]);
}

//Works with nonconstant time steps
void CFriedrichsMollifierNumeric::convolute(CDEData& aDED, int iRow, int iCol, std::vector<double>& res)
{
    double dConv = 0.0, dNorm = 0.0, dLeft = 0.0, dRight = 0.0, dh = get_h(aDED, iRow), 
           dDiff = cfg().getDiffMultiplier()*(aDED.get(0, iRow - 1) - aDED.get(0, iRow));
	int iStart = iRow - cfg().getHalfIntervalInDataPoints() + 1, iEnd = iRow + cfg().getHalfIntervalInDataPoints(), iE = cfg().getIntegrationIntersteps() + 1;
	double dRow = aDED.get(0, iRow);
	for (int i = iStart; i <= iEnd; i++)
	{
		double dS = aDED.get(0, i - 1), dE = aDED.get(0, i);
		double dt = (dE - dS)/(1.0*iE);
		for (int k = 1; k <= iE; k++)
		{
		        double d_i = aDED.get(iCol, i), d_i_1 = aDED.get(iCol, i - 1);
			    double dLower = d_i_1 + (d_i - d_i_1)*(dt*(k-1))/(dE - dS);//linear interpolated data value
				double dUpper = d_i_1 + (d_i - d_i_1)*(dt*k)/(dE - dS);//linear interpolated data value
				double dLeftLower = d_i_1 + (d_i - d_i_1)*(dt*(k-1) - dDiff)/(dE - dS);
				double dLeftUpper = d_i_1 + (d_i - d_i_1)*(dt*k - dDiff)/(dE - dS);
				double dRightLower = d_i_1 + (d_i - d_i_1) * (dt*(k - 1) + dDiff) / (dE - dS);
				double dRightUpper = d_i_1 + (d_i - d_i_1) * (dt*k + dDiff) / (dE - dS);
				//The shift dDiff drops out in dS - dMid.
				dConv += 0.5*dt*(fmf((dRow - dS - dt*(k-1))/dh)*dLower + fmf((dRow - dS - dt*k)/dh)*dUpper);//Trapezoid rule
				dNorm += 0.5*dt*(fmf((dRow - dS - dt*(k - 1))/dh) + fmf((dRow - dS - dt*k)/dh));//Trapezoid rule
				dRight += 0.5*dt*(fmf((dRow - dS - dt*(k - 1))/dh)*dRightLower + fmf((dRow - dS - dt*k)/dh)*dRightUpper);
				dLeft += 0.5*dt*(fmf((dRow - dS - dt*(k - 1))/dh)*dLeftLower + fmf((dRow - dS - dt*k)/dh)*dLeftUpper);
		}
	}
	res[0] = dConv/dNorm;
	res[1] = (dRight - dLeft)/(2.0*dDiff*dNorm);
}

double CFriedrichsMollifierNumeric::get_h(CDEData& aDED, int iPos)
{
	double dh = aDED.get(0, iPos + cfg().getHalfIntervalInDataPoints()) - aDED.get(0, iPos);
	if (dh < aDED.get(0, iPos) - aDED.get(0, iPos - cfg().getHalfIntervalInDataPoints())) dh = aDED.get(0, iPos) - aDED.get(0, iPos - cfg().getHalfIntervalInDataPoints());
	return dh;
}
