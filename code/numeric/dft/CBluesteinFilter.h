// CBluesteinFilter.h: Schnittstelle für die Klasse CBluesteinFilter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CBLUESTEINFILTER_H)
#define CBLUESTEINFILTER_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CDEDATA_H
#include "pipeline/datastructures/CDEData.h"
#endif

#ifndef CFOURIERFILTER_H
#include "numeric/dft/CFourierFilter.h"
#endif


class CBluesteinFilter : public CFourierFilter
{
public:
	CBluesteinFilter();	
	virtual ~CBluesteinFilter();

    virtual CDEData* filter(CDEData& orig, double dEps, int iDerivative = 0, int iSobolevExponent = 0);
    virtual std::vector<CDEData*>* ftReal(CDEData& orig); 
    virtual CDEData* ift(std::vector<CDEData*>* pComplex, CDEData& orig);

private:
    double* getFrequencyData(CDEData& orig);
    double* createTimeData(CDEData& d);

    void forward(CDEData& orig, std::vector<double*>* pRe, std::vector<double*>* pIm);
    void backward(std::vector<CDEData*>& complex, CDEData& orig, std::vector<double*>* pRe);

    CDEData* filterState(CDEData& data, double dEps, int iSobolevExponent = 0);
    CDEData* filterFirstDerivative(CDEData& data, double dEps, int iSobolevExponent = 0);

};

#endif //CBLUESTEINFILTER_H

