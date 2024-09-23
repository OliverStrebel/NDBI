// CLotkaVolterra.cpp: Implementierung der Klasse CLotkaVolterra.

#include "CLotkaVolterra.h"

#include "numeric/linalg/CCMMatrix.h"

#include "pipeline/datastructures/CDEData.h"
#include "pipeline/datastructures/CPipelineData.h"

#include "util/CEADSyException.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CLotkaVolterra::CLotkaVolterra()
{
}

CLotkaVolterra::~CLotkaVolterra()
{
}

void CLotkaVolterra::setRegressionMatrix(CCMMatrix& A, int iRow, int iComp, int iDataRow, CPipelineData* pPD)
{
    CDEData& aDEDState = pPD->getPreparedDEData(0);
    CDEData& aDEDDeriv = pPD->getPreparedDEData(1);

    if (iComp == 0)
    {
        A.setElement(iRow, 0, aDEDState.get(1, iDataRow));
        A.setElement(iRow, 1, aDEDState.get(1, iDataRow) * aDEDState.get(1, iDataRow));
        A.setElement(iRow, 2, aDEDState.get(1, iDataRow) * aDEDState.get(2, iDataRow));
    }
    else if (iComp == 1)
    {
        A.setElement(iRow, 0, aDEDState.get(2, iDataRow));
        A.setElement(iRow, 1, aDEDState.get(1, iDataRow) * aDEDState.get(2, iDataRow));
    }
    else throw CEADSyException("CLotkaVolterra::setRegressionMatrix: Invalid component number");
}

int CLotkaVolterra::getParameterCount(int iComp)
{
    if (iComp == 0) return 3;
    else if (iComp == 1) return 2;
    else throw CEADSyException("CLotkaVolterra::getParameterCount: Invalid component number");
}
/*
if (iComp >= 0 && iComp < 2) return 5;

if (iComp == 0)
{
    A.setElement(iRow, 0, aDEDState.get(1, iDataRow));
    A.setElement(iRow, 1, aDEDState.get(1, iDataRow) * aDEDState.get(1, iDataRow));
    A.setElement(iRow, 2, aDEDState.get(1, iDataRow) * aDEDState.get(2, iDataRow));
    A.setElement(iRow, 3, aDEDState.get(1, iDataRow) * aDEDState.get(1, iDataRow) * aDEDState.get(1, iDataRow));
    A.setElement(iRow, 4, aDEDState.get(1, iDataRow) * aDEDDeriv.get(1, iDataRow));
}
else if (iComp == 1)
{
    A.setElement(iRow, 0, aDEDState.get(2, iDataRow));
    A.setElement(iRow, 1, aDEDState.get(1, iDataRow) * aDEDState.get(2, iDataRow));
    A.setElement(iRow, 2, aDEDState.get(2, iDataRow) * aDEDState.get(2, iDataRow));
    A.setElement(iRow, 3, aDEDState.get(2, iDataRow) * aDEDState.get(2, iDataRow) * aDEDState.get(1, iDataRow));
    A.setElement(iRow, 4, aDEDState.get(1, iDataRow) * aDEDDeriv.get(2, iDataRow));
}



if (iComp == 0) return 3;
else if (iComp == 1) return 2;


if (iComp == 0)
{
    A.setElement(iRow, 0, aDEDState.get(1, iDataRow));
    A.setElement(iRow, 1, aDEDState.get(1, iDataRow) * aDEDState.get(1, iDataRow));
    A.setElement(iRow, 2, aDEDState.get(1, iDataRow) * aDEDState.get(2, iDataRow));
}
else if (iComp == 1)
{
    A.setElement(iRow, 0, aDEDState.get(2, iDataRow));
    A.setElement(iRow, 1, aDEDState.get(1, iDataRow) * aDEDState.get(2, iDataRow));
}

*/