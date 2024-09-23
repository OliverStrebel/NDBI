// CDETerm.h: Schnittstelle
//
//////////////////////////////////////////////////////////////////////

#if !defined(CDETERM_H)
#define CDETERM_H


#ifndef __VECTOR__
#include <vector>
#endif

#ifndef __STRING__
#include <string>
#endif


class CCMVector;
class CPipelineData;

class CDETerm
{
public:
	CDETerm();
	virtual ~CDETerm();

	bool isActive(){return m_bActive;};
	void setActivity(bool bActive){m_bActive = bActive;};

	virtual int getParameterCount() = 0;
	virtual double f(int iPos, CPipelineData* pPD, CCMVector& x, int iStart, int iEnd) = 0;

	virtual std::string latex() = 0;
	virtual std::string getSortString() = 0;

private:
	bool m_bActive;
};

#endif //CDETERM_H
