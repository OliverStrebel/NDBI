// CLeaveOneOutEstimator.h: Schnittstelle für die Klasse CLeaveOneOutEstimator.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CLEAVEONEOUTESTIMATOR_H)
#define CLEAVEONEOUTESTIMATOR_H


#ifndef __VECTOR__
#include <vector>
#endif


#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif


#ifndef CENTROPYMINCALCULATOR_H
#include "pipeline/estimation/CEntropyMinCalculator.h"
#endif


class CCMVector;
class CDEData;
class CCMMatrix;
class CPipelineData;
class CResultData;
class CEquationSystem;

class CLeaveOneOutEstimator : public CPipelineElement
{
public:
	CLeaveOneOutEstimator(CPipelineElement* pPE, bool useEffect = true);
	virtual ~CLeaveOneOutEstimator();

	virtual CPipelineData* next();

private:
    //1st index: ode component. 2nd index: index of subsample vector
    typedef std::vector<std::vector<CCMVector>> SubSamples;
    
    std::vector<std::vector<CCMVector>> allocateSubSamples(CDEData& aDEDState, CEquationSystem& aEQS);
    void setData(CPipelineData* pPD, CCMMatrix& A, CCMVector& b, int iComp, int iPosSkipped, CEquationSystem& aEQS);
	void solve(CCMMatrix& A, CCMVector& b, SubSamples& s, int iComp);
    void storeResult(CResultData& aRD, SubSamples& s, int iComp);

    CEntropyMinCalculator m_EMC;
};

#endif //CLEAVEONEOUTESTIMATOR_H