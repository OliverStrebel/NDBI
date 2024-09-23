//CDEData.cpp: Implementierung der Klasse CDEData.
///////////////////////////////////////////////////////////////////////////////
#include "CDEData.h"

#include <vector>

#include "util/CEADSyException.h"
#include "util/CLog.h"

CDEData::CDEData(int iRowCount, int iColCount, int dependenVariableOffset)
: m_iRowCount(iRowCount), m_iDependendVariableOffset(dependenVariableOffset)
{
    m_pData = new std::vector<double*>;
    for (int j = 0; j < iColCount; j++) m_pData->push_back(new double[iRowCount]);
}

CDEData::CDEData(int iRowCount, int dependenVariableOffset, std::vector<double*>* pData)
: m_iRowCount(iRowCount), m_iDependendVariableOffset(dependenVariableOffset), m_pData(pData)
{
}

CDEData::~CDEData()
{
    if( m_pData != 0)
    {
        for (unsigned int i = 0; i < m_pData->size(); i++)
            if ((*m_pData)[i] != 0)
            {
                delete (*m_pData)[i];
                (*m_pData)[i] = 0;
            }
        delete m_pData;
        m_pData = 0;
    }
}
