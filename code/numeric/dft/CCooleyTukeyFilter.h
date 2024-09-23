// CCooleyTukeyFilter.h: Schnittstelle für die Klasse CCooleyTukeyFilter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CCOOLEYTUKEYFILTER_H)
#define CCOOLEYTUKEYFILTER_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CDEDATA_H
#include "pipeline/datastructures/CDEData.h"
#endif

#ifndef CFOURIERFILTER_H
#include "numeric/dft/CFourierFilter.h"
#endif


class CCooleyTukeyFilter : public CFourierFilter
{
public:
	CCooleyTukeyFilter();	
	virtual ~CCooleyTukeyFilter();

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

    bool checkPowerOf2(int iRowCount);
    int getPowerBase2(int iRowCount);
};

#endif //CCOOLEYTUKEYFILTER_H

