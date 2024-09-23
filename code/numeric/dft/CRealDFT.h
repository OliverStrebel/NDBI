// CRealDFT.h: Schnittstelle für die Klasse CRealDFT.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CRealDFT_H)
#define CRealDFT_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CDEDATA_H
#include "pipeline/datastructures/CDEData.h"
#endif

#ifndef CFOURIERFILTER_H
#include "numeric/dft/CFourierFilter.h"
#endif


class CRealDFT : public CFourierFilter
{
public:
	CRealDFT();	
	virtual ~CRealDFT();

    virtual CDEData* filter(CDEData& orig, double dEps, int iDerivative = 0, int iSobolevExponent = 0);
    virtual std::vector<CDEData*>* ftReal(CDEData& orig);
    virtual CDEData* ift(std::vector<CDEData*>* pComplex, CDEData& orig);

private:
    std::vector<CDEData*>* newComplexData(CDEData& orig);
    CDEData* createEmptyFrequencyData(CDEData& orig);
    CDEData* createEmptyTimeData(CDEData& d);

    void forward(CDEData& orig, CDEData& transformed, bool isCosinus);
    void backward(std::vector<CDEData*>& complex, CDEData& orig, CDEData& transformed);

    CDEData* filterState(CDEData& data, double dEps, int iSobolevExponent = 0);
    CDEData* filterFirstDerivative(CDEData& data, double dEps, int iSobolevExponent = 0);
};

#endif //CRealDFT_H

