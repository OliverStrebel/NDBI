// CEADSyConfig.h: Schnittstelle für die Klasse CEADSyConfig.h.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CEADSYCONFIG_H)
#define CEADSYCONFIG_H


#ifndef __STRING__
#include <string>
#endif

#ifndef __MAP__
#include <map>
#endif

#ifndef __VECTOR__
#include <vector>
#endif



class CEADSyConfig
{
friend class COptimizationEADSyRunner;

public:
    typedef const std::string FLAG;
    CEADSyConfig();
	~CEADSyConfig();

	static void init(std::string aConfigFileName);
	static void close();
    static CEADSyConfig& instance() { return *m_pEADSyConfig; };
    const std::string m_csvSeparator = " ";

private:
    static CEADSyConfig* m_pEADSyConfig;
    std::string m_cfgFileName;

    void doInit();
    std::map<std::string, std::string> getLinesFromFile();
    std::vector<std::string> readFileNames(std::string aLine);
    int initInteger(std::map<std::string, std::string>& lines, std::string fieldName);
    double initDouble(std::map<std::string, std::string>& lines, std::string fieldName);
    std::string initString(std::map<std::string, std::string>& lines, std::string fieldName);
    std::string getExceptMsg(std::string var);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OperationMode Configuration
public:
    struct OperationMode { FLAG AS_IS = "AS_IS"; FLAG NOISE_SCAN = "NOISE_SCAN"; FLAG SKIP_SCAN = "SKIP_SCAN"; };
    const OperationMode m_OperationModesAvailable;
    std::string getOperationMode() { return m_OperationMode; };

    //Skip Scan
    bool isSkipMode() { return m_OperationMode == m_OperationModesAvailable.SKIP_SCAN;};
    int getMaxSkipRecords() { return m_iMaxSkipRecords; };
    int getSkipIncrement() { return m_iSkipIncrement; };

    //Noise Scan
    double getSigmaStart() { return m_dSigmaStart; };
    int getSigmaSteps() { return m_iSigmaSteps; };
    double getSigmaDelta() { return m_dSigmaDelta; };
private:
    std::string m_OperationMode;
    std::string mode;

    //Skip Scan
    int m_iMaxSkipRecords;
    int m_iSkipIncrement;

    //Noise Scan
    double m_dSigmaStart;
    int m_iSigmaSteps;
    double m_dSigmaDelta;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//InputType Configuration
public:
    struct DataSourceType { FLAG STD_FILE = "STD_FILE"; };//STATE_AND_DERIVATIVE 
    const DataSourceType m_DataSourceTypesAvailable;
    int getNumberOfLinesLimit(){return m_iNumberOfLinesLimit;};
    std::string getDataSourceTypeType() { return m_DataSourceType; };
    std::vector<std::string> getFileNames() { return m_FileNames; };
private:
    int m_iNumberOfLinesLimit;
    std::string m_DataSourceType;
    std::vector<std::string> m_FileNames;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DifferentiationMethod Configuration
public:
    struct DifferentiationMethod {
        FLAG FINITE_DIFF = "FINITE_DIFF"; FLAG TANGENT_REGRESSION = "TANGENT_REGRESSION"; FLAG FRIEDRICHS_MOLLIFIER_WEAK = "FRIEDRICHS_MOLLIFIER_WEAK";
        FLAG FRIEDRICHS_MOLLIFIER_NUMERIC = "FRIEDRICHS_MOLLIFIER_NUMERIC"; FLAG SPECTRAL_LOWPASS = "SPECTRAL_LOWPASS";
        FLAG DIFFERENTIATION_BY_INTEGRATION = "DIFFERENTIATION_BY_INTEGRATION";};
    const DifferentiationMethod m_DifferentiationMethodsAvailable;
    std::string getDifferentiationMethod() { return m_DifferentiationMethod; };

    //Spectral Filter Parameters
    double getEpsSpectralStart() { return m_dEpsSpectralStart; };
    int getEpsSpectralSteps() { return m_iEpsSpectralSteps; };
    double getEpsSpectralDelta() { return m_dEpsSpectralDelta; };
    double getWindowEffectEliminationPercent() { return m_dWindowEffectEliminationPercent; };
    double getKneeMultiplier() { return m_dKneeMultiplier; };

    //Friedrichs Mollifier and Weak Differentiation Parameters
    int getHalfIntervalInDataPoints() { return m_iHalfIntervalInDataPoints; };
    int getIntegrationIntersteps() { return m_iIntegrationIntersteps; };
    double getDiffMultiplier() { return m_dDiffMultiplier; };

    //Differentiation by Integration Parameters
    int getDifPolynomialDegree() { return m_iDifPolynomialDegree; };
    int getNumRegressions() { return m_iNumRegressions; };
    int getRegressionColMultiplier() { return m_iRegressionColMultiplier; };

private:
    std::string m_DifferentiationMethod;

    //Fourier Filter Parameters
    double m_dEpsSpectralStart;
    int m_iEpsSpectralSteps;
    double m_dEpsSpectralDelta;
    double m_dWindowEffectEliminationPercent;
    double m_dKneeMultiplier;

    //Friedrichs Mollifier and Weak Differentiation Parameters
    int m_iHalfIntervalInDataPoints;
    int m_iIntegrationIntersteps;
    double m_dDiffMultiplier;

    //Integrated Parameters
    int m_iDifPolynomialDegree;
    int m_iNumRegressions;
    int m_iRegressionColMultiplier;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EstimationMethod Configuration
public:
    struct EstimationMethod { FLAG MODEL_STEPDOWN_EFFECT = "MODEL_STEPDOWN_EFFECT";
                              FLAG MODEL_STEPDOWN_ENTROPY = "MODEL_STEPDOWN_ENTROPY";
                              FLAG NONE = "NONE";
                              FLAG PARAMETER_LEAVE_ONE_OUT = "PARAMETER_LEAVE_ONE_OUT"; };
    const EstimationMethod m_EstimationMethodsAvailable;
    std::string getEstimationMethod() { return m_EstimationMethod; };
    double getSigmaScale(){return m_dSigmaScale;};
    int getSampleSize(){return m_iSampleSize;};
    int getSampleCount(){return m_iSampleCount;};
    int getPolynomialOrder(){return m_iPolynomialOrder;};
private:
    std::string m_EstimationMethod;
    double m_dSigmaScale;
    int m_iSampleSize;
    int m_iSampleCount;
    int m_iPolynomialOrder;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Sink Configuration
public:
    std::string getAlternativeOutputPath(){return m_AlternativeOutputPath;};
private:
    std::string m_AlternativeOutputPath;
};

#endif //CEADSYCONFIG_H