//CResultData.cpp: Implementierung der Klasse CResultData.
///////////////////////////////////////////////////////////////////////////////
#include "CResultData.h"

#include "pipeline/de/CDE.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"

CResultData::CResultData(std::vector<CDE*> aDE)
    : m_DE(aDE)
{
    for (unsigned int i = 0; i < aDE.size(); i++)
    {
        std::vector<std::vector<int>> p;
        m_ParameterNumbers.push_back(p);

        std::vector<std::vector<double>> m;
        m_Means.push_back(m);

        std::vector<std::vector<double>> sd;
        m_StdDevs.push_back(sd);

        std::vector<std::string> types;
        m_Types.push_back(types);
    }
}

CResultData::~CResultData()
{ 
    for (unsigned int i = 0; i < m_DE.size(); i++)
        if (m_DE[i] != 0)
        {
            delete m_DE[i];
            m_DE[i] = 0;
        }
}

void CResultData::prepareStep(int iComp, const char* pTypeName)
{
    std::vector<int> p;
    m_ParameterNumbers[iComp].push_back(p);

    std::vector<double> m;
    m_Means[iComp].push_back(m);

    std::vector<double> sd;
    m_StdDevs[iComp].push_back(sd);

    m_Types[iComp].push_back(std::string(pTypeName));
}

void CResultData::addSampleResults(int iComp, int iParameterNumber, double dMean, double dStdDev)
{
    int iLast = (int) m_ParameterNumbers[iComp].size() - 1;
    m_ParameterNumbers[iComp][iLast].push_back(iParameterNumber);
    m_Means[iComp][iLast].push_back(dMean);
    m_StdDevs[iComp][iLast].push_back(dStdDev);
}

void CResultData::storeError(int iComp, int iErrorCode)
{
    int iLast = (int)m_ParameterNumbers[iComp].size() - 1;
    m_ParameterNumbers[iComp][iLast].push_back(iErrorCode);
    m_Means[iComp][iLast].push_back(iErrorCode);
    m_StdDevs[iComp][iLast].push_back(iErrorCode);
}

std::vector<std::string> CResultData::getLastResult(int iComp)
{
    std::vector<std::string> res;
    int iLast = ((int) m_ParameterNumbers[iComp].size() - 1);
    if (!m_DE[iComp]->isEstimable())
    {
        res.push_back( std::to_string(iComp) + std::string(" Not Estimable"));
        return res;
    }

    for (int i = 0; i < (int)m_ParameterNumbers[iComp][iLast].size(); i++)
    {
        std::string line = "";
        line.append(std::to_string(iComp));
        line.append(" ");
        line.append(std::to_string(m_ParameterNumbers[iComp][iLast][i]));
        line.append(" ");
        line.append(std::to_string(m_Means[iComp][iLast][i]));
        line.append(" ");
        line.append(std::to_string(m_StdDevs[iComp][iLast][i]));
        res.push_back(line);
    }

    return res;
}
