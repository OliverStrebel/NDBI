// CFourierFilter.h: Schnittstelle für die Klasse CFourierFilter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CFOURIERFILTER_H)
#define CFOURIERFILTER_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CDEDATA_H
#include "pipeline/datastructures/CDEData.h"
#endif

class CFourierFilter
{
public:
	CFourierFilter();	
	virtual ~CFourierFilter();

    virtual CDEData* filter(CDEData& data, double dEps, int iDerivative = 0, int iSobolevExponent = 0) = 0;
    virtual std::vector<CDEData*>* ftReal(CDEData& orig) = 0;
    virtual CDEData* ift(std::vector<CDEData*>* pComplex, CDEData& orig) = 0;

    void cleanUp(CDEData* pDED, std::vector<CDEData*>* pvD = 0, CDEData* pDEDOther = 0);
};

#endif //CFOURIERFILTER_H

