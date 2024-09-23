// CRealRiemannSumFT.cpp: Implementierung der Klasse CRealRiemannSumFT.
//
//////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES

#include "CRealRiemannSumFT.h"

#include "util/CLog.h"
#include "util/CEADSyException.h"

#include <cmath>

/*
    In order to cope also with non-equidistant data points
    that arise frequently with experimental data
    a simple approximation of the fourier integral is used.
*/
CRealRiemannSumFT::CRealRiemannSumFT()
{
}

CRealRiemannSumFT::~CRealRiemannSumFT()
{
}

CDEData* CRealRiemannSumFT::filter(CDEData& data, double dEps, int iDerivative, int iSobolevExponent)
{
    if (iDerivative >= 2) throw CEADSyException("CRealRiemannSumFT::filter: derivatives higher than first order not yet implemented!");
    if (iDerivative == 0) return filterState(data, dEps, iSobolevExponent);
    else return filterFirstDerivative(data, dEps, iSobolevExponent);
}

/*
* Returns a vector with 2 CDEData-pointers to the real and 
* imaginary part of the discrete fourier transform.
* 
* For an even number of data points 
* the last data point is dropped for the calculation
*/
std::vector<CDEData*>* CRealRiemannSumFT::ftReal(CDEData& d)
{
    std::vector<CDEData*>* pVD = newComplexData(d);
    forward(d, *(pVD->at(0)), true);
    forward(d, *(pVD->at(1)), false);
    return pVD;
}

/*
* Accepts a vector with 2 CDEData-pointers to the real and
* imaginary part of some fourier transformed data
* and returns a pointer to the real part of the 
* inverse fourier transformed input
*/
CDEData* CRealRiemannSumFT::ift(std::vector<CDEData*>* pComplex, CDEData& orig)
{
    CDEData* pDED = createEmptyData(orig, true);
    backward(*pComplex, orig, *pDED);
    return pDED;
}

//################################################################################################################
//private helpers
std::vector<CDEData*>* CRealRiemannSumFT::newComplexData(CDEData& d)
{
    std::vector<CDEData*>* pD = new std::vector<CDEData*>();
    pD->push_back(createEmptyData(d));
    pD->push_back(createEmptyData(d));
    return pD;
}

CDEData* CRealRiemannSumFT::createEmptyData(CDEData& d, bool isBackward)
{
    int iRowCount = getRowCount(d, isBackward);
    std::vector<double*>* pd = new std::vector<double*>();
    for (int j = 0; j < d.getColCount(); j++) pd->push_back(new double[iRowCount]);
    return new CDEData(iRowCount, d.getDependendVariableOffset(), pd);
}

int CRealRiemannSumFT::getRowCount(CDEData& d, bool isBackward)
{
    if (isBackward)
    { 
        if (d.getRowCount()%2 != 0) return d.getRowCount();
        else return d.getRowCount() - 1;
    }

    if (d.getRowCount()%2 != 0) return d.getRowCount()/2 + 1;
    else return (d.getRowCount()-1)/2 + 1;
}

void CRealRiemannSumFT::forward(CDEData& orig, CDEData& transformed, bool isCosinus)
{
    double dPreFactor = 1.0/(sqrt(2.0*M_PI));
    setFrequencies(orig, transformed);
    for (int j = orig.getDependendVariableOffset(); j < orig.getColCount(); j++)
        for (int i = 0; i < transformed.getRowCount(); i++)
        {
            double dSum = 0.0;
            double omega_i = transformed.get(0, i);
            int iLast = orig.getRowCount() - 1;
            if (orig.getRowCount()%2 == 0) iLast = iLast-1;
            for (int k = 0; k < iLast; k++)
            {
                double tk = orig.get(0, k+1) - orig.get(0, k);
                if (isCosinus) dSum+=tk*orig.get(j,k)*cos(omega_i*orig.get(0, k));
                else dSum+=tk*(-1.0)*orig.get(j, k)*sin(omega_i*orig.get(0, k));
            }
            transformed.set(j, i, dSum*dPreFactor);
        }
}

void CRealRiemannSumFT::setFrequencies(CDEData& orig, CDEData& transformed)
{
    int N = transformed.getRowCount();
    int iLast = orig.getRowCount() - 1;
    if (orig.getRowCount()%2 == 0) iLast = iLast-1;//drop the last data point for even numbers of rows
    double T = (orig.get(0, iLast) - orig.get(0, 0));
    for (int n = 0; n < N; n++) transformed.set(0, n, 2.0*M_PI*n/T);
}

void CRealRiemannSumFT::backward(std::vector<CDEData*>& complex, CDEData& orig, CDEData& transformed)
{
    double dPreFactor = 1.0/(sqrt(2.0*M_PI));
    setIndependendVars(orig, transformed);
    CDEData& re = *complex[0];
    CDEData& im = *complex[1];

    for (int j = orig.getDependendVariableOffset(); j < orig.getColCount(); j++)
        for (int i = 0; i < transformed.getRowCount(); i++)
        {
            double dSum = 0.0;
            double t_i = transformed.get(0, i);
            double dLowerRe = re.get(j, 0)*cos(t_i*re.get(0, 0)), dLowerIm = im.get(j, 0)*sin(t_i*im.get(0, 0)), dUpperRe = 0.0, dUpperIm = 0.0;
            for (int k = 0; k < re.getRowCount() - 1; k++)
            {
                double dOmk = re.get(0, k + 1) - re.get(0, k);
                //trapezoid rule
                dUpperRe = re.get(j, k+1)*cos(t_i*re.get(0, k+1));
                dUpperIm = im.get(j, k+1)*sin(t_i*im.get(0, k+1));
                dSum += dOmk*(dLowerRe + dUpperRe - dUpperIm - dLowerIm);
                dLowerRe = dUpperRe;//saves costly evaluytion of trigonometric functions
                dLowerIm = dUpperIm;//saves costly evaluytion of trigonometric functions
            }
            transformed.set(j, i, dSum*dPreFactor);
        }
}

void CRealRiemannSumFT::setIndependendVars(CDEData& orig, CDEData& transformed)
{
    for (int i = 0; i < transformed.getRowCount(); i++) 
        transformed.set(0, i, orig.get(0,i));
}

CDEData* CRealRiemannSumFT::filterState(CDEData& data, double dEps, int iSob)
{
    std::vector<CDEData*>* pVD = ftReal(data);
    for (int i = 0; i < pVD->at(0)->getRowCount(); i++)
        for (int j = pVD->at(0)->getDependendVariableOffset(); j < pVD->at(0)->getColCount(); j++)
        {
            double dOm = pVD->at(0)->get(0, i);
            pVD->at(0)->set(j, i, pVD->at(0)->get(j, i)/(1.0 + dEps*pow(1.0 + dOm*dOm, 1.0*iSob)*dOm*dOm));
            pVD->at(1)->set(j, i, pVD->at(1)->get(j, i)/(1.0 + dEps*pow(1.0 + dOm*dOm, 1.0*iSob)*dOm*dOm));
        }
    CDEData* pDEDF = ift(pVD, data);
    cleanUp(0, pVD);
    return pDEDF;
}

CDEData* CRealRiemannSumFT::filterFirstDerivative(CDEData& data, double dEps, int iSob)
{
    std::vector<CDEData*>* pVD = ftReal(data);
    for (int i = 0; i < pVD->at(0)->getRowCount(); i++)
        for (int j = pVD->at(0)->getDependendVariableOffset(); j < pVD->at(0)->getColCount(); j++)
        {
            double dOm = pVD->at(0)->get(0, i);
            double dImNew = pVD->at(0)->get(j, i) / (1.0 + dEps * pow(1.0 + dOm*dOm, 1.0*iSob)*dOm*dOm);
            double dReNew = pVD->at(1)->get(j, i) / (1.0 + dEps * pow(1.0 + dOm*dOm, 1.0*iSob)*dOm*dOm);
            pVD->at(0)->set(j, i, -1.0*dReNew*dOm);
            pVD->at(1)->set(j, i, dImNew*dOm);
        }
    CDEData* pDEDF = ift(pVD, data);
    cleanUp(0, pVD);
    return pDEDF;
}
