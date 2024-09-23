// CDE.h: Schnittstelle.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CDE_H)
#define CDE_H


#ifndef __VECTOR__
#include <vector>
#endif

#ifndef __STRING__
#include <string>
#endif


#ifndef CDETERM_H
#include "CDETerm.h"
#endif


class CCMVector;
class CCMMatrix;
class CDEData;

class CDE
{
public:
	CDE(std::vector<CDETerm*>* m_pTerms, int iEquationNum);
	virtual ~CDE();

	void setRHS(int iPos, int iComp, CCMVector& b, CPipelineData* pPD, int iSamplePos);
	void setMatrix(int iPos, CCMMatrix& A, CPipelineData* pPD, int iSamplePos);
	bool isEstimable(){return (m_pTerms->size() > 0);};

	int getAvailableParameterCount(){return (int) m_pTerms->size();};//Jeder Term kommt mit der Anzahl seiner Parameter vor
	int getParameterCount();
	bool isActive(int i){return m_pTerms->at(i)->isActive();};//Jeder Term kommt mit der Anzahl seiner Parameter vor
	void setActivity(int iPos, bool bActive){m_pTerms->at(iPos)->setActivity(bActive); };
	std::vector<int> getParameterNumbersForTerm(int iPos);
	int getEquationNum() {return m_iEquationNum;};

	std::string latex();

protected:
	CDE(): m_iEquationNum(-1), m_pTerms(0){};

private:
	std::vector<CDETerm*>* m_pTerms;
	int m_iEquationNum;
};

#endif //CDE_H
