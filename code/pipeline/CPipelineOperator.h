// CPipelineOperator:Uses CEADSyConfig, creates and operates the pipeline
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CPIPELINEOPERATOR_H)
#define CPIPELINEOPERATOR_H

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif

class CPipelineElement;
class CDataSource;
class CNoiseAdder;

class CPipelineOperator
{
public:
	CPipelineOperator();
	virtual ~CPipelineOperator();

	static void run();

private:
    static CPipelineElement* createPipelineFromConfig(CDataSource** ppS, CNoiseAdder** ppNAD);
    static CPipelineElement* createNoiseAdder(CPipelineElement* pLastPE, CNoiseAdder** ppNAD);
    static CPipelineElement* createDifferentiationMethod(CPipelineElement* pLastPE);
    static CPipelineElement* createEstimationMethod(CPipelineElement* pLastPE);

    static void runSkipScan(CDataSource* pS, CNoiseAdder* pNAD, CPipelineElement* pPull);
    static void runNoiseScan(CDataSource* pS, CNoiseAdder* pNAD, CPipelineElement* pPull);

    static CEADSyConfig& cfg() {return CEADSyConfig::instance();}
};

#endif //CPIPELINEOPERATOR_H