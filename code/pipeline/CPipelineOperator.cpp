// CPipelineOperator: Uses CEADSyConfig, creates and operates the pipeline
//
///////////////////////////////////////////////////////////////////////////////
#include "CPipelineOperator.h"

#include "CPipelineElement.h"

#include "datastructures/CPipelineData.h"

#include "pipeline/datapreparation/CNoiseAdder.h"

#include "pipeline/datapreparation/CTangentRegression.h"
#include "pipeline/datapreparation/CFiniteDiff.h"
#include "pipeline/datapreparation/CSpectralLowpass.h"
#include "pipeline/datapreparation/CFriedrichsMollifierNumeric.h"
#include "pipeline/datapreparation/CFriedrichsMollifierWeak.h"

#include "pipeline/datapreparation/integration/CDifferentiationByIntegration.h"

#include "pipeline/io/CDataSource.h"
#include "pipeline/io/CDataSink.h"

#include "pipeline/estimation/CStepDownSubsampler.h"
#include "pipeline/estimation/parameter/CLeaveOneOutEstimator.h"

#include "pipeline/CPullEnd.h"

#include "util/CEADSyConfig.h"
#include "util/CEADSyException.h"
#include "util/CLog.h"

#include <iostream>


CPipelineOperator::CPipelineOperator()
{
}

CPipelineOperator::~CPipelineOperator()
{
}

void CPipelineOperator::run()
{
    std::vector<std::string> fileNames = cfg().getFileNames();
    CDataSource* pSource = 0;
    CNoiseAdder* pNoiseAdder = 0;
    CPullEnd pullEnd(createPipelineFromConfig(&pSource, &pNoiseAdder));

    for (int k = 0; k < fileNames.size(); k++)
    {
        std::cout << "Processing " << fileNames[k] << std::endl;
        pSource->setSourceFile(fileNames[k]);
        if (cfg().getOperationMode() == cfg().m_OperationModesAvailable.SKIP_SCAN) runSkipScan(pSource, pNoiseAdder, &pullEnd);
        else if (cfg().getOperationMode() == cfg().m_OperationModesAvailable.NOISE_SCAN) runNoiseScan(pSource, pNoiseAdder, &pullEnd);
        else if (cfg().getOperationMode() == cfg().m_OperationModesAvailable.AS_IS) delete pullEnd.next();
        else throw CEADSyException(std::string("Invalid Operation Mode: ") + cfg().getOperationMode());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//createPipeline
CPipelineElement* CPipelineOperator::createPipelineFromConfig(CDataSource** ppS, CNoiseAdder** ppNAD)
{
    CPipelineElement* pLastPE = *ppS = new CDataSource(0);
    pLastPE = createNoiseAdder(pLastPE, ppNAD);
    pLastPE = createDifferentiationMethod(pLastPE);
    pLastPE = createEstimationMethod(pLastPE);
    return new CDataSink(pLastPE);
}

CPipelineElement* CPipelineOperator::createNoiseAdder(CPipelineElement* pLastPE, CNoiseAdder** ppNAD)
{
    if ((cfg().getOperationMode() == cfg().m_OperationModesAvailable.NOISE_SCAN)
             || (cfg().getOperationMode() == cfg().m_OperationModesAvailable.SKIP_SCAN))
    {
        *ppNAD = new CNoiseAdder(pLastPE, 0.00);
        pLastPE = *ppNAD;
    }
    else if (cfg().getOperationMode() == cfg().m_OperationModesAvailable.AS_IS);
    else throw CEADSyException(std::string("Invalid Operation Mode: ") + cfg().getOperationMode());
    return pLastPE;
}

CPipelineElement* CPipelineOperator::createDifferentiationMethod(CPipelineElement* pLastPE)
{
    if (cfg().getDifferentiationMethod() == cfg().m_DifferentiationMethodsAvailable.FINITE_DIFF)
        pLastPE = new CFiniteDiff(pLastPE);
    else if (cfg().getDifferentiationMethod() == cfg().m_DifferentiationMethodsAvailable.FRIEDRICHS_MOLLIFIER_NUMERIC)
        pLastPE = new CFriedrichsMollifierNumeric(pLastPE);
    else if (cfg().getDifferentiationMethod() == cfg().m_DifferentiationMethodsAvailable.FRIEDRICHS_MOLLIFIER_WEAK)
        pLastPE = new CFriedrichsMollifierWeak(pLastPE);
    else if (cfg().getDifferentiationMethod() == cfg().m_DifferentiationMethodsAvailable.SPECTRAL_LOWPASS)
        pLastPE = new CSpectralLowpass(pLastPE);
    else if (cfg().getDifferentiationMethod() == cfg().m_DifferentiationMethodsAvailable.TANGENT_REGRESSION)
        pLastPE = new CTangentRegression(pLastPE);
    else if (cfg().getDifferentiationMethod() == cfg().m_DifferentiationMethodsAvailable.DIFFERENTIATION_BY_INTEGRATION)
        pLastPE = new CDifferentiationByIntegration(pLastPE);
    else throw CEADSyException(std::string("Invalid Differentiation Method: ") + cfg().getDifferentiationMethod());
    return pLastPE;
}

CPipelineElement* CPipelineOperator::createEstimationMethod(CPipelineElement* pLastPE)
{
    if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.MODEL_STEPDOWN_EFFECT)
        pLastPE = new CStepDownSubsampler(pLastPE);
    else if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.MODEL_STEPDOWN_ENTROPY)
        pLastPE = new CStepDownSubsampler(pLastPE,false);
    else if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.PARAMETER_LEAVE_ONE_OUT)
        pLastPE = new CLeaveOneOutEstimator(pLastPE);
    else if (cfg().getEstimationMethod() == cfg().m_EstimationMethodsAvailable.NONE)
        pLastPE = pLastPE;
    else throw CEADSyException(std::string("Invalid Estimation Method: ") + cfg().getEstimationMethod());

    return pLastPE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//run Scans
void CPipelineOperator::runSkipScan(CDataSource* pS, CNoiseAdder* pNAD, CPipelineElement* pPull)
{
    pNAD->setNoiseLevel(cfg().getSigmaStart());
    for (int i = 0; i <= cfg().getMaxSkipRecords(); i+=cfg().getSkipIncrement())
    {
        pS->setSkipRecords(i);
        std::cout << "    skipped data records: " << i << std::endl;
        delete pPull->next();
    }
}

void CPipelineOperator::runNoiseScan(CDataSource* pS, CNoiseAdder* pNAD, CPipelineElement* pPull)
{
    for (int i = 0; i < cfg().getSigmaSteps(); i++)
    {
        double dNoiseLevel = cfg().getSigmaStart() + cfg().getSigmaDelta() * i;
        pNAD->setNoiseLevel(dNoiseLevel);
        std::cout << "    noise level: " << dNoiseLevel << std::endl;
        delete pPull->next();
    }
}
