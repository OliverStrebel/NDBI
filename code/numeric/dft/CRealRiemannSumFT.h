// CRealRiemannSumFT.h: Schnittstelle für die Klasse CRealRiemannSumFT.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CREALRIEMANNSUMFT_H)
#define CREALRIEMANNSUMFT_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CDEDATA_H
#include "pipeline/datastructures/CDEData.h"
#endif

#ifndef CFOURIERFILTER_H
#include "numeric/dft/CFourierFilter.h"
#endif


class CRealRiemannSumFT: public CFourierFilter
{
public:
	CRealRiemannSumFT();	
	virtual ~CRealRiemannSumFT();

    virtual CDEData* filter(CDEData& data, double dEps, int iDerivative = 0, int iSobolevExponent = 0);
    virtual std::vector<CDEData*>* ftReal(CDEData& orig);
    virtual CDEData* ift(std::vector<CDEData*>* pComplex, CDEData& orig);

private:
    std::vector<CDEData*>* newComplexData(CDEData& d);
    CDEData* createEmptyData(CDEData& d, bool isBackward = false);
    int getRowCount(CDEData& d, bool isBackward);

    void forward(CDEData& orig, CDEData& transformed, bool isCosinus);
    void backward(std::vector<CDEData*>& complex, CDEData& orig, CDEData& transformed);
    void setFrequencies(CDEData& orig, CDEData& transformed);
    void setIndependendVars(CDEData& orig, CDEData& transformed);

    CDEData* filterState(CDEData& data, double dEps, int iSobolevExponent = 0);
    CDEData* filterFirstDerivative(CDEData& data, double dEps, int iSobolevExponent = 0);
};

#endif //CREALRIEMANNSUMFT_H

