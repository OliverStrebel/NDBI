// CRealDFT.cpp: Implementierung der Klasse CRealDFT.
//
//////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES

#include "CRealDFT.h"

#include "util/CLog.h"
#include "util/CEADSyException.h"

#include <cmath>

CRealDFT::CRealDFT()
{
}

CRealDFT::~CRealDFT()
{
}

CDEData* CRealDFT::filter(CDEData& orig, double dEps, int iDerivative, int iSobolevExponent)
{
    if (iDerivative >= 2) throw CEADSyException("CRealDFT::filter: derivatives higher than first order not yet implemented!");
    if (iDerivative == 0) return filterState(orig, dEps, iSobolevExponent);
    else return filterFirstDerivative(orig, dEps, iSobolevExponent);
}

/*
* Returns a vector with 2 CDEData-pointers to the real and
* imaginary part of the discrete fourier transform.
*/
std::vector<CDEData*>* CRealDFT::ftReal(CDEData& orig)
{
    std::vector<CDEData*>* pVD = newComplexData(orig);
    forward(orig, *(pVD->at(0)), true);
    forward(orig, *(pVD->at(1)), false);
    return pVD;
}

/*
* Accepts a vector with 2 CDEData-pointers to the real and
* imaginary part of some fourier transformed data
* and returns a pointer to the real part of the
* inverse fourier transformed input
*/
CDEData* CRealDFT::ift(std::vector<CDEData*>* pComplex, CDEData& orig)
{
    CDEData* pDED = createEmptyTimeData(orig);
    backward(*pComplex, orig, *pDED);
    return pDED;
}

//################################################################################################################
//private helpers
std::vector<CDEData*>* CRealDFT::newComplexData(CDEData& orig)
{
    std::vector<CDEData*>* pD = new std::vector<CDEData*>();
    pD->push_back(createEmptyFrequencyData(orig));
    pD->push_back(createEmptyFrequencyData(orig));
    return pD;
}

//Frequencies like scipy rfft
CDEData* CRealDFT::createEmptyFrequencyData(CDEData& orig)
{
    int iRowCount = orig.getRowCount()/2 + 1;
    if (orig.getRowCount()%2 == 0) iRowCount = (orig.getRowCount() - 1)/2 + 1;
    std::vector<double*>* pd = new std::vector<double*>();
    for (int j = 0; j < orig.getColCount(); j++) pd->push_back(new double[iRowCount]);
    int N = orig.getRowCount();
    if (orig.getRowCount() % 2 == 0) N = orig.getRowCount() - 1;
    double tau = (orig.get(0, 1) - orig.get(0, 0));
    for (int n = 0; n < iRowCount; n++) pd->at(0)[n] = (1.0*n)/(tau*N);

    return new CDEData(iRowCount, orig.getDependendVariableOffset(), pd);
}

void CRealDFT::forward(CDEData& orig, CDEData& transformed, bool isCosinus)
{
    int iRowCount = orig.getRowCount();
    if (orig.getRowCount()%2 == 0) iRowCount = orig.getRowCount() - 1;

    for (int j = orig.getDependendVariableOffset(); j < orig.getColCount(); j++)
        for (int i = 0; i < transformed.getRowCount(); i++)
        {
            double dSum = 0.0, omega_i = 2.0*M_PI*transformed.get(0, i);
            for (int k = 0; k < iRowCount; k++)
                if (isCosinus) dSum += orig.get(j, k)*cos(omega_i*orig.get(0, k));
                else dSum += -1.0*orig.get(j, k)*sin(omega_i*orig.get(0, k));
            transformed.set(j, i, dSum);
        }
}

CDEData* CRealDFT::createEmptyTimeData(CDEData& orig)
{
    int iRowCount = orig.getRowCount();
    if (orig.getRowCount()%2 == 0) iRowCount = orig.getRowCount() - 1;
    std::vector<double*>* pd = new std::vector<double*>();
    for (int j = 0; j < orig.getColCount(); j++) pd->push_back(new double[iRowCount]);
    for (int n = 0; n < iRowCount; n++) pd->at(0)[n] = orig.get(0, n);

    return new CDEData(iRowCount, orig.getDependendVariableOffset(), pd);
}

void CRealDFT::backward(std::vector<CDEData*>& complex, CDEData& orig, CDEData& transformed)
{
    int N = transformed.getRowCount();
    CDEData& re = *complex[0];
    CDEData& im = *complex[1];

    for (int j = orig.getDependendVariableOffset(); j < orig.getColCount(); j++)
        for (int i = 0; i < transformed.getRowCount(); i++)
        {
            double t_i = t_i = 2.0*M_PI*orig.get(0, i);
            double dSum = re.get(j, 0)*cos(t_i * re.get(0, 0));
            for (int k = 1; k < re.getRowCount(); k++)
                dSum += 2.0*re.get(j, k)*cos(t_i*re.get(0, k)) - 2.0*im.get(j, k)*sin(t_i*im.get(0, k));
            transformed.set(j, i, dSum/N);
        }
}

CDEData* CRealDFT::filterState(CDEData& orig, double dEps, int iSob)
{
    std::vector<CDEData*>* pVD = ftReal(orig);
    for (int i = 0; i < pVD->at(0)->getRowCount(); i++)
        for (int j = pVD->at(0)->getDependendVariableOffset(); j < pVD->at(0)->getColCount(); j++)
        {
            double dOm = pVD->at(0)->get(0, i);
            double dRegFactor = (1.0 + dEps * pow(1.0 + dOm * dOm, 1.0 * iSob) * dOm * dOm);
            pVD->at(0)->set(j, i, pVD->at(0)->get(j, i)/dRegFactor);
            pVD->at(1)->set(j, i, pVD->at(1)->get(j, i)/dRegFactor);
        }
    CDEData* pDEDF = ift(pVD, orig);
    cleanUp(0, pVD);
    return pDEDF;
}

CDEData* CRealDFT::filterFirstDerivative(CDEData& orig, double dEps, int iSob)
{
    std::vector<CDEData*>* pVD = ftReal(orig);
    for (int i = 0; i < pVD->at(0)->getRowCount(); i++)
        for (int j = pVD->at(0)->getDependendVariableOffset(); j < pVD->at(0)->getColCount(); j++)
        {
            double dOm = 2.0*M_PI*pVD->at(0)->get(0, i);
            double dRegFactor = (1.0 + dEps*pow(1.0 + dOm*dOm, 1.0*iSob)*dOm*dOm);
            double dImNew = pVD->at(0)->get(j, i)/dRegFactor;
            double dReNew = pVD->at(1)->get(j, i)/dRegFactor;
            pVD->at(0)->set(j, i, -1.0*dReNew*dOm);
            pVD->at(1)->set(j, i, dImNew*dOm);
        }
    CDEData* pDEDF = ift(pVD, orig);
    cleanUp(0, pVD);
    return pDEDF;
}
