// CDEMonomeTerm.h: Schnittstelle
//
//////////////////////////////////////////////////////////////////////

#if !defined(CDEMONOMETERM_H)
#define CDEMONOMETERM_H

#ifndef CDETERM_H
#include "CDETerm.h"
#endif

class CCMVector;
class CPipelineData;

class CDEMonomeTerm : public CDETerm
{
public:
	CDEMonomeTerm(std::vector<int>& exponents);
	virtual ~CDEMonomeTerm();

	virtual int getParameterCount(){return 1;};
	virtual double f(int iPos, CPipelineData* pPD, CCMVector& x, int iStart, int iEnd);
	virtual std::string latex();
	virtual std::string getSortString();

protected:
    CDEMonomeTerm(){};

private:
	std::vector<int> m_Exponents;
};

#endif //CDEMONOMETERM_H
