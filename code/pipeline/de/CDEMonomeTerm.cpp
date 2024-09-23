// CDEMonomeTerm.cpp: Implementierung der Klasse CDEMonomeTerm.

#include "CDEMonomeTerm.h"

#include "numeric/linalg/CCMVector.h"

#include "pipeline/datastructures/CDEData.h"
#include "pipeline/datastructures/CPipelineData.h"


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CDEMonomeTerm::CDEMonomeTerm(std::vector<int>& exponents)
: m_Exponents(exponents)
{
}

CDEMonomeTerm::~CDEMonomeTerm()
{
}

double CDEMonomeTerm::f(int iSamplePos, CPipelineData* pPD, CCMVector& x, int iStart, int iEnd)
{
    CDEData& aDED = pPD->getPreparedDEData(0);
    int iOffset = aDED.getDependendVariableOffset();
	double d = 1.0;
	for (unsigned int i = 0; i < m_Exponents.size(); i++)
		d *= (pow(aDED.get(i+iOffset, iSamplePos), 1.0*m_Exponents[i]));
	return d*x.getElement(iStart);
}

std::string CDEMonomeTerm::latex()
{
    std::string tex = "";
    for (unsigned int i = 0; i < m_Exponents.size(); i++)
    { 
        if (m_Exponents[i] > 0)
        {
            if (tex.size() > 0) tex = tex + std::string("*");
            tex = tex + std::string("x_") + std::to_string(i + 1);
            if (m_Exponents[i] > 1)  tex = tex + std::string("^") + std::to_string(m_Exponents[i]);
        }
    }
    if (tex.size() == 0) tex = "1";
	return tex;
}

/*
* Works correctly only up to 999 Variables
*/
std::string CDEMonomeTerm::getSortString()
{
    int iSumOfExponents = 0;
    for (unsigned int i = 0; i < m_Exponents.size(); i++) iSumOfExponents+= m_Exponents[i];
    std::string mySortString = std::to_string(iSumOfExponents);
    for (unsigned int i = 0; i < m_Exponents.size(); i++)
        for (unsigned int j = 0; j < (unsigned int) m_Exponents[i]; j++) 
        {
            std::string num = std::to_string(i + 1);
            if (num.size() == 1) num = std::string("00") + num;
            if (num.size() == 2) num = std::string("0") + num;
            mySortString = mySortString + std::string("x_") + num;
        }

	return mySortString;
}
