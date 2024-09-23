// CFourierFilter.cpp: Implementierung der Klasse CFourierFilter.
//
//////////////////////////////////////////////////////////////////////
#include "CFourierFilter.h"

CFourierFilter::CFourierFilter()
{
}

CFourierFilter::~CFourierFilter()
{
}

void CFourierFilter::cleanUp(CDEData* pDED, std::vector<CDEData*>* pvD, CDEData* pDEDOther)
{
    if (pDED != 0) { delete pDED; pDED = 0; }
    if (pDEDOther != 0) { delete pDEDOther; pDEDOther = 0; }
    if (pvD != 0)
    {
        for (unsigned int i = 0; i < pvD->size(); i++) if (pvD->at(i) != 0) { delete pvD->at(i); pvD->at(i) = 0; }
        delete pvD;
        pvD = 0;
    }
}

