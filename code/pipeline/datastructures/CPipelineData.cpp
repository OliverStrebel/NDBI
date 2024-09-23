//CPipelineData.cpp: Implementierung der Klasse CPipelineData.
///////////////////////////////////////////////////////////////////////////////
#include "CPipelineData.h"

#include "CDEData.h"
#include "CResultData.h"

#include "pipeline/de/CDEFactory.h"
#include "pipeline/de/CDE.h"

#include "util/CEADSyException.h"
#include "util/CEADSyConfig.h"
#include "util/CLog.h"


CPipelineData::CPipelineData(std::string& aFileName, CDEData* pDEData, double dNoiseAdded)
: m_FileName(aFileName), m_pDEData(pDEData), m_dNoiseAdded(dNoiseAdded)
{
    int iOffset = m_pDEData->getDependendVariableOffset();
    int iNumVariables = m_pDEData->getColCount() - iOffset;
    CDEFactory f;
    std::vector<CDE*> des;
    for (int i = 0; i < iNumVariables; i++) 
        des.push_back(f.createPolynomialDE(iNumVariables, cfg().getPolynomialOrder(), i));
    m_pResultData = new CResultData(des);
}

CPipelineData::~CPipelineData()
{
    if (m_pDEData !=0)
    {
        delete m_pDEData;
        m_pDEData = 0; 
    }

    if (m_pResultData != 0)
    {
        delete m_pResultData;
        m_pResultData = 0;
    }

    for ( unsigned int i = 0; i < m_PreparedDEData.size(); i++)
        if (m_PreparedDEData[i] != 0)
        {
            delete m_PreparedDEData[i];
            m_PreparedDEData[i] = 0;
        }
}
