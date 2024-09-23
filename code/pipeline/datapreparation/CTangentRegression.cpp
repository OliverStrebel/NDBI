//CTangentRegression.cpp: Implementierung der Klasse CTangentRegression.
///////////////////////////////////////////////////////////////////////////////
#include "CTangentRegression.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"

#include "numeric/linalg/CCMVector.h"
#include "numeric/linalg/CCMMatrix.h"
#include "numeric/linalg/solver/CCholeskySolver.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"


CTangentRegression::CTangentRegression(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CTangentRegression::~CTangentRegression()
{
}

CPipelineData* CTangentRegression::next()
{
    CPipelineData* pPD = getPrevious()->next();
	calculatePreparedData(pPD);
    return pPD;
}

void CTangentRegression::calculatePreparedData(CPipelineData* pPD)
{
	createPreparedDEData(pPD);
	int iStart = pPD->getDEData().getDependendVariableOffset();
	int iEnd = pPD->getDEData().getColCount();
	for (int iCoordinate = iStart; iCoordinate < iEnd; iCoordinate++)
        for (int iRow = 0; iRow < pPD->getPreparedDEData(0).getRowCount(); iRow++ ) 
		    prepareDataPoint(pPD, iCoordinate, iRow );
}

void CTangentRegression::createPreparedDEData(CPipelineData* pPD)
{
    int iTangentRegressionSize = cfg().getHalfIntervalInDataPoints();
    CDEData& aDED = pPD->getDEData();
	int iSize = aDED.getRowCount() - 2*iTangentRegressionSize;
	for (int i = 0; i <= 1; i++)
	{
		std::vector<double*>* pD = new std::vector<double*>();
		for (int j = 0; j < aDED.getColCount(); j++) pD->push_back(new double[iSize]);
		CDEData* pDEDP = new CDEData(iSize, aDED.getDependendVariableOffset(), pD);
		
		for (int k = 0; k < aDED.getDependendVariableOffset(); k++)
			for (int j = 0; j < pDEDP->getRowCount(); j++) pDEDP->set(k, j, aDED.get(k, j + iTangentRegressionSize));
		
		pPD->addPreparedDEData(pDEDP);
	}
}

void CTangentRegression::prepareDataPoint(CPipelineData* pPD, int iCol, int iRow)
{
    int iSize = 2;//Nur erste Ableitung
	int iTangentRegressionSize = cfg().getHalfIntervalInDataPoints();
	int iNumTangentPoints = 2*iTangentRegressionSize + 1;
	CCMMatrix A(iNumTangentPoints, iSize);
	CCMVector b(iNumTangentPoints);
	CDEData& aDED = pPD->getDEData();

	for (int i = - iTangentRegressionSize; i <= iTangentRegressionSize; i++)
	{
		b.setElement(i + iTangentRegressionSize, aDED.get(iCol, i + iRow + iTangentRegressionSize));
		double dt = aDED.get(0, i + iRow + iTangentRegressionSize) - aDED.get(0, iRow + iTangentRegressionSize);
		for (int k = 0; k < iSize; k++) A.setElement(i + iTangentRegressionSize, k, pow(dt, 1.0*k));
	}
	CCMVector x(iSize);
	CCholeskySolver s;
	CCMMatrix AT = A.getTransposed();
	CCMMatrix ATA = AT*A;
	CCMVector ATb = AT*b;
	s.solve(ATA, ATb, x);
	for ( int n = 0; n < iSize; n++) pPD->getPreparedDEData(n).set(iCol, iRow, x.getElement(n));
}
