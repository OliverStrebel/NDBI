#include "CEADSyConfig.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "util/CLog.h"
#include "util/CEADSyException.h"
#include "util/CFileReader.h"

#include "CSubDirFileFilter.h"

#pragma warning( disable : 26495) //Non initialized member variable


CEADSyConfig* CEADSyConfig::m_pEADSyConfig = 0;


CEADSyConfig::CEADSyConfig()
: m_cfgFileName("data/Configuration4All.cfg")
{
}

CEADSyConfig::~CEADSyConfig()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//static interface
void CEADSyConfig::init(std::string aCfgFileName)
{
	if (m_pEADSyConfig == 0)
	{
		m_pEADSyConfig = new CEADSyConfig();
		if (aCfgFileName.size() > 0) m_pEADSyConfig->m_cfgFileName = aCfgFileName;
		m_pEADSyConfig->doInit();
	}
}

void CEADSyConfig::close()
{
	if (m_pEADSyConfig != 0)
	{
		delete m_pEADSyConfig;
		m_pEADSyConfig = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//private helpers
void CEADSyConfig::doInit()
{
	std::map<std::string, std::string> lines = getLinesFromFile();


    m_OperationMode = initString(lines, "OperationMode");
    m_dSigmaStart = initDouble(lines, "sigmaStart");
    m_dSigmaDelta = initDouble(lines, "sigmaDelta");
    m_iSigmaSteps = initInteger(lines, "sigmaSteps");
    m_iMaxSkipRecords = initInteger(lines, "maxSkipRecords"); 
    m_iSkipIncrement = initInteger(lines, "skipIncrement");


    m_DataSourceType = initString(lines, "DataSourceType");
    if (lines.find("data") != lines.end()) m_FileNames = readFileNames(lines["data"]); else throw CEADSyException(getExceptMsg("data"));
    m_iNumberOfLinesLimit = initInteger(lines, "numberOfLinesLimit");


    m_DifferentiationMethod = initString(lines, "DifferentiationMethod");
    //Low Pass Filter
    m_iEpsSpectralSteps = initInteger(lines, "epsSpectralSteps");
	m_dEpsSpectralStart = initDouble(lines, "epsSpectralStart");
	m_dEpsSpectralDelta = initDouble(lines, "epsSpectralDelta");
	m_dWindowEffectEliminationPercent = initDouble(lines, "windowEffectEliminationPercent");
    m_dKneeMultiplier = initDouble(lines, "kneeMultiplier");
    //Friedrichs Mollifier 
	m_iHalfIntervalInDataPoints = initInteger(lines, "halfIntervalInDataPoints");
	m_iIntegrationIntersteps = initInteger(lines, "integrationInterSteps");
    m_dDiffMultiplier = initDouble(lines, "diffMultiplier");
    //Integrated
    m_iDifPolynomialDegree = initInteger(lines, "difPolynomialDegree");
    m_iNumRegressions = initInteger(lines, "numRegressions");
    m_iRegressionColMultiplier = initInteger(lines, "regressionColMultiplier");


    m_EstimationMethod = initString(lines, "EstimationMethod");
    m_dSigmaScale = initDouble(lines, "sigmaScale");
    m_iSampleSize = initInteger(lines, "sampleSize");
    m_iSampleCount = initInteger(lines, "sampleCount");
    m_iPolynomialOrder = initInteger(lines, "polynomialOrder");


    m_AlternativeOutputPath = initString(lines, "alternative_output_path");
}

int CEADSyConfig::initInteger(std::map<std::string, std::string>& lines, std::string fieldName)
{
	if (lines.find(fieldName) != lines.end()) return std::stoi(lines[fieldName]);
	else throw CEADSyException(getExceptMsg(fieldName));
}

double CEADSyConfig::initDouble(std::map<std::string, std::string>& lines, std::string fieldName)
{
	if (lines.find(fieldName) != lines.end()) return std::stod(lines[fieldName]);
	else throw CEADSyException(getExceptMsg(fieldName));
}

std::string CEADSyConfig::initString(std::map<std::string, std::string>& lines, std::string fieldName)
{
    if (lines.find(fieldName) != lines.end()) return lines[fieldName];
    else throw CEADSyException(getExceptMsg(fieldName));
}

std::string CEADSyConfig::getExceptMsg(std::string var)
{
	return std::string("Fix entry for ") + var + std::string(" in configuration file ") + m_cfgFileName;
}

std::map<std::string, std::string>  CEADSyConfig::getLinesFromFile()
{
	CFileReader r;
	std::stringstream sData = r.getFileData(m_cfgFileName);

	std::string item;
	std::vector<std::string> lines;
	while (std::getline(sData, item))
	{
		item.erase(std::remove_if(item.begin(), item.end(), std::isspace), item.end());
		lines.push_back(item);
	}
	if (lines.size() < 1) throw CEADSyException(std::string("Configuration File is ") + m_cfgFileName + std::string(" is empty!"));

	std::map<std::string, std::string> lineMap;
	for (unsigned int i = 0; i < lines.size(); i++)
	    if (lines[i].size() != 0)
		    if (lines[i][0] != '#')
			{
				size_t iEnd = lines[i].find("=");
				lineMap[lines[i].substr(0, iEnd)] = lines[i].substr(iEnd+1, lines[i].size());
			}
	return lineMap;
}

std::vector<std::string> CEADSyConfig::readFileNames(std::string aPath)
{
	if (aPath[aPath.size()-1] == '/') return CSubDirFileFilter::getFileNames(aPath, ".dat");
	std::vector<std::string> fileName;
	fileName.push_back(aPath);
	return fileName;
}
