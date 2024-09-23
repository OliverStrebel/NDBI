// CDE.cpp: Implementierung der Klasse CDE.
//
//////////////////////////////////////////////////////////////////////

#include "CDE.h"

#include "numeric/linalg/CCMVector.h"
#include "numeric/linalg/CCMMatrix.h"

#include "pipeline/datastructures/CDEData.h"
#include "pipeline/datastructures/CPipelineData.h"

#include "util/CEADSyConfig.h"
#include "util/CEADSyException.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CDE::CDE(std::vector<CDETerm*>* pTerms, int iEquationNum)
: m_pTerms(pTerms), m_iEquationNum(iEquationNum)
{
}

CDE::~CDE()
{
    if ( m_pTerms != 0)
	{
		for (int i = 0; i < getAvailableParameterCount(); i++)
			//Jeder Term kommt mit der Anzahl seiner Parameter vor
			if ((*m_pTerms)[i] != 0)
			{
				delete (*m_pTerms)[i];
				(*m_pTerms)[i] = 0;
			}
		delete m_pTerms;
		m_pTerms = 0;
	}
}

int CDE::getParameterCount()
{
	int iCount = 0;
	CDETerm* pT = 0;
	for (int i = 0; i < (int) m_pTerms->size(); i++)
	{
		//Jeder Term kommt mit der Anzahl seiner Parameter vor
		if ((*m_pTerms)[i]->isActive() && (pT != (*m_pTerms)[i])) iCount += ((*m_pTerms)[i]->getParameterCount());
		pT = (*m_pTerms)[i];
	}
	return iCount;
}

//linear in parameters olnly
void CDE::setRHS(int iPos, int iComp, CCMVector& b, CPipelineData* pPD, int iSamplePos)
{
    CDEData& aDED = pPD->getPreparedDEData(1);
	b.setElement(iPos, aDED.get(iComp + aDED.getDependendVariableOffset(), iSamplePos));
}

//linear in parameters olnly
void CDE::setMatrix(int iPos, CCMMatrix& T, CPipelineData* pPD, int iSamplePos)
{
	CDEData& aDED = pPD->getPreparedDEData(0);
	int iCol = 0;
	CCMVector x(2);
	x.setOne();

	for (int i = 0; i < (int) m_pTerms->size(); i++)
	{
		//Jeder Term kommt mit der Anzahl seiner Parameter vor
		CDETerm* pTC = m_pTerms->at(i);
		if (pTC->isActive())
		{
			T.setElement( iPos, iCol, pTC->f(iSamplePos, pPD, x, 0, -1));
			iCol++;
		}
	}
}

std::vector<int> CDE::getParameterNumbersForTerm(int iPos)
{
	CDETerm* pT = (*m_pTerms)[iPos];
	std::vector<int> termParameterNumbers;
	for (int i = 0; i < (int) m_pTerms->size(); i++)
		if (pT == (*m_pTerms)[i]) termParameterNumbers.push_back(i);
	return termParameterNumbers;
}

std::string CDE::latex()
{
	std::string tex = "";
	for (unsigned int i = 0; i < m_pTerms->size(); i++)
	{
		if (tex.size() > 0) tex.append(std::string(" + "));
		tex.append(m_pTerms->at(i)->latex());
	}
	return tex;
}

