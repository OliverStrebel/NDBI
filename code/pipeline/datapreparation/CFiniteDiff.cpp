//CFiniteDiff.cpp: Implementierung der Klasse CFiniteDiff.
///////////////////////////////////////////////////////////////////////////////
#include "CFiniteDiff.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"

#include "numeric/linalg/CCMVector.h"
#include "numeric/linalg/CCMMatrix.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"


CFiniteDiff::CFiniteDiff(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CFiniteDiff::~CFiniteDiff()
{
}

CPipelineData* CFiniteDiff::next()
{
    CPipelineData* pPD = getPrevious()->next();
	calculatePreparedData(pPD);
    return pPD;
}

void CFiniteDiff::calculatePreparedData(CPipelineData* pPD)
{
	createPreparedDEData(pPD);
	int iStart = pPD->getDEData().getDependendVariableOffset();
	int iEnd = pPD->getDEData().getColCount();
	for (int iCoordinate = iStart; iCoordinate < iEnd; iCoordinate++)
        for (int iRow = 0; iRow < pPD->getPreparedDEData(0).getRowCount(); iRow++ ) 
		    prepareDataPoint(pPD, iCoordinate, iRow );
}

void CFiniteDiff::createPreparedDEData(CPipelineData* pPD)
{
    CDEData& aDED = pPD->getDEData();
	int iSize = aDED.getRowCount();
	for (int i = 0; i <= 1; i++)
	{
	    std::vector<double*>* pD = new std::vector<double*>();
	    for (int j = 0; j < aDED.getColCount(); j++) pD->push_back(new double[iSize]);
	    CDEData* pDED = new CDEData(iSize, aDED.getDependendVariableOffset(), pD);		
	    for (int k = 0; k < aDED.getDependendVariableOffset(); k++)
		    for (int j = 0; j < iSize; j++) pDED->set(k, j, aDED.get(k, j));
		
	pPD->addPreparedDEData(pDED);
	}
}

void CFiniteDiff::prepareDataPoint(CPipelineData* pPD, int iCol, int iRow)
{
     CDEData& aDED = pPD->getDEData();
	 CDEData& aDEDState = pPD->getPreparedDEData(0);
	 CDEData& aDEDDeriv = pPD->getPreparedDEData(1);
	 aDEDState.set(iCol, iRow, aDED.get(iCol, iRow));

	 double dDeriv = 0.0;
	 int iRC = aDED.getRowCount();
	 if (iRow == 0)  dDeriv = (aDED.get(iCol, 1) - aDED.get(iCol, 0))/(aDED.get(0, 1) - aDED.get(0, 0));
	 else if (iRow == iRC-1) dDeriv = (aDED.get(iCol, iRC -2) - aDED.get(iCol, iRC-1))/(aDED.get(0, iRC-1) - aDED.get(0, iRC-2));
	 else dDeriv = (aDED.get(iCol, iRow+1) - aDED.get(iCol, iRow-1))/(aDED.get(0, iRow+1) - aDED.get(0, iRow-1));

	 aDEDDeriv.set(iCol, iRow, dDeriv);
}
