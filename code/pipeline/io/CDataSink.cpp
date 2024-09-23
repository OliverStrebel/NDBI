//CDataSink.cpp: Implementierung der Klasse CDataSink.
///////////////////////////////////////////////////////////////////////////////
#include "CDataSink.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <string>

#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CResultData.h"

#include "pipeline/de/CDE.h"

#include "util/CEADSyException.h"
#include "util/CFileWriter.h"


CDataSink::CDataSink(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CDataSink::~CDataSink()
{
}

CPipelineData* CDataSink::next()
{ 
    CPipelineData* pPD = getPrevious()->next();
    std::vector<std::string> sinkFilePath = getPaths(pPD);
    if (cfg().getEstimationMethod() != cfg().m_EstimationMethodsAvailable.NONE) writeResultFile(pPD->getResultData(), getName( pPD, sinkFilePath[1], ".res"));
    if (cfg().getOperationMode() == cfg().m_OperationModesAvailable.SKIP_SCAN) writeDEDataFile(pPD->getDEData(), getName(pPD, sinkFilePath[0], ".used"));
    if (pPD->getPreparedDEDataSize() > 0) writeDEDataFile(pPD->getPreparedDEData(0), getName(pPD, sinkFilePath[1], "STATE"));
    if (pPD->getPreparedDEDataSize() > 1) writeDEDataFile(pPD->getPreparedDEData(1), getName(pPD, sinkFilePath[1], "DERIV"));
    return pPD;
}

void CDataSink::writeResultFile(CResultData& aRD, std::string aFileName)
{
    std::ofstream aFile;
    aFile.open(aFileName, std::ios::trunc);
    if (!aFile.is_open()) throw CEADSyException(std::string("File cannot be opened: ") + aFileName);
 
    if (!aRD.hasData()) return;

    for (int iComp = 0; iComp < aRD.getDECount(); iComp++)
    {
        std::vector<std::string> res = aRD.getLastResult(iComp);
        for (int j = 0; j < (int)res.size(); j++) aFile << res[j] << std::endl;
    }
    aFile.flush();
    aFile.close();
}

void CDataSink::writeDEDataFile(CDEData& aDED, std::string aFileName)
{
    CFileWriter fw;
    fw.writeDEData(aDED, aFileName.c_str());
}


std::vector<std::string> CDataSink::getPaths(CPipelineData* pPD)
{
    int iPrecision = 10;
    std::vector<std::string> paths;
    std::string sourceFileName = pPD->getFileName();
    size_t iPos = sourceFileName.rfind('/');
    std::string path = sourceFileName.substr(0, iPos + 1);

    std::string sinkFilePath = path.append(cfg().getOperationMode().append("/"));
    paths.push_back(sinkFilePath + std::string("NOISY_DATA_USED/"));
    paths.push_back(sinkFilePath + cfg().getDifferentiationMethod() + std::string("/"));
    return paths;
}

std::string CDataSink::getName(CPipelineData* pPD, std::string sinkFilePath, std::string ident)
{
    int iPrecision = 10;
    std::string sourceFileName = pPD->getFileName();
    size_t iPos = sourceFileName.rfind('/');

    std::string fullname = sourceFileName.substr(iPos+1, sourceFileName.size());
    int iTypeSeparatorPos = (int) fullname.find('.');
    std::string name = fullname.substr(0, iTypeSeparatorPos);
    std::string sinkFileIdent = getIdentifierNoise(pPD, iPrecision);
    if (cfg().isSkipMode()) sinkFileIdent = getIdentifierSkip(pPD);

    std::string sinkFileName = sinkFilePath + getPrefix(ident) + name.append("_").append(sinkFileIdent) + getSuffix(ident);
    return sinkFileName;
}

std::string CDataSink::getIdentifierSkip(CPipelineData* pPD)
{
    std::ostringstream ostr;
    ostr << pPD->getSkipRecords();
    std::string skipRecs = ostr.str();
    if (skipRecs.size() == 1) skipRecs = std::string("0").append(skipRecs);
    return skipRecs;
}

std::string CDataSink::getIdentifierNoise(CPipelineData* pPD, int iPrecision)
{
    std::ostringstream ostr;
    ostr << std::setprecision(iPrecision);
    ostr << pPD->getNoiseAdded();
    std::string noiseAdded = ostr.str();
    for (int i = (int) noiseAdded.size(); i < iPrecision; i++) noiseAdded.append("0");
    return noiseAdded;
}

std::string CDataSink::getPrefix(std::string ident)
{
    if (ident == std::string(".res"))
    {
        if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.MODEL_STEPDOWN_EFFECT) return "MOD_EF_";
        else if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.MODEL_STEPDOWN_ENTROPY) return "MOD_EN_";
        else if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.PARAMETER_LEAVE_ONE_OUT) return "PAR_LOO_";
        else throw CEADSyException(std::string("Invalid EstimationMethod: ") + cfg().getEstimationMethod());
    }
    if (ident == std::string("DERIV")) return ident + std::string("_");
    if (ident == std::string("STATE")) return ident + std::string("_");
    if (ident == std::string(".used")) return std::string("");
    else throw CEADSyException(std::string("Invalid file identifier: " ) + ident);
}

std::string CDataSink::getSuffix(std::string ident)
{
    if (ident == std::string(".res") || ident == std::string(".used")) return ident;
    if (ident == std::string("DERIV")) return ".prep_dat";
    if (ident == std::string("STATE")) return ".prep_dat";
    else throw CEADSyException(std::string("Invalid file identifier: ") + ident);
}
