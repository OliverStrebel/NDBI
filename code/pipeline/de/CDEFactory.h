// CDEFactory.h: Schnittstelle für die Klasse CDEFactory.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CDEFACTORY_H)
#define CDEFACTORY_H


#ifndef __STRING__
#include <string>
#endif

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif


class CDE;

class CDEFactory  
{
public:
	CDEFactory();
	virtual ~CDEFactory();

	CDE* createPolynomialDE(int iNumVariables, int iDegree, int iEquationNum);

private:
	CEADSyConfig& cfg() {return CEADSyConfig::instance();};

};

#endif //CDEFACTORY_H
