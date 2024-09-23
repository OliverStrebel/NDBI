// CCooleyTukeyFilter.cpp: Implementierung der Klasse CCooleyTukeyFilter.
//
//////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES

#include "CCooleyTukeyFilter.h"

#include "util/CEADSyException.h"

#include "numeric/dft/CCooleyTukeyFFT.h"

#include <cmath>

CCooleyTukeyFilter::CCooleyTukeyFilter()
{
}

CCooleyTukeyFilter::~CCooleyTukeyFilter()
{
}

CDEData* CCooleyTukeyFilter::filter(CDEData& orig, double dEps, int iDerivative, int iSobolevExponent)
{
    if (iDerivative >= 2) throw CEADSyException("CCooleyTukeyFilter::filter: derivatives higher than first order not yet implemented!");
    if (!checkPowerOf2(orig.getRowCount())) throw CEADSyException("CCooleyTukeyFilter::filter: length of data must be a power of 2!");
    if (iDerivative == 0) return filterState(orig, dEps, iSobolevExponent);
    else return filterFirstDerivative(orig, dEps, iSobolevExponent);
}

/*
* Returns a vector with 2 CDEData-pointers to the real and
* imaginary part of the discrete fourier transform.
*/
std::vector<CDEData*>* CCooleyTukeyFilter::ftReal(CDEData& orig)
{
    std::vector<CDEData*>* pVD = new std::vector<CDEData*>();
    std::vector<double*>* pRe = new std::vector<double*>();
    std::vector<double*>* pIm = new std::vector<double*>();
    pRe->push_back(getFrequencyData(orig));
    pIm->push_back(getFrequencyData(orig));

    forward( orig, pRe, pIm);

    pVD->push_back(new CDEData(orig.getRowCount(), orig.getDependendVariableOffset(), pRe));
    pVD->push_back(new CDEData(orig.getRowCount(), orig.getDependendVariableOffset(), pIm));
    return pVD;
}

/*
* Accepts a vector with 2 CDEData-pointers to the real and
* imaginary part of some fourier transformed data
* and returns a pointer to the real part of the
* inverse fourier transformed input
*/
CDEData* CCooleyTukeyFilter::ift(std::vector<CDEData*>* pComplex, CDEData& orig)
{
    std::vector<double*>* pRe = new std::vector<double*>();
    pRe->push_back(createTimeData(orig));
    backward(*pComplex, orig, pRe);
    return new CDEData(orig.getRowCount(), orig.getDependendVariableOffset(), pRe);
}

//################################################################################################################
//private helpers

//Frequencies like scipy.fft.fftfreq
//See also Press, Teukolsky, Vetterling and Flannery, Numerical Receipes in Fortran 2nd ed. (1990) Fig. 12.2.2
double* CCooleyTukeyFilter::getFrequencyData(CDEData& orig)
{
    int iRowCount = orig.getRowCount();
    double* pd = new double[iRowCount];
    double tau = (orig.get(0, 1) - orig.get(0, 0));
    for (int n = 0; n < iRowCount/2; n++) pd[n] = (1.0*n)/(tau*iRowCount);
    pd[iRowCount/2] = 1.0/(2.0*tau);//Nyquist frequency
    int iPos = iRowCount/2 + 1;
    for (int n = iRowCount/2 - 1; n > 0; n--)
    {
        pd[iPos] = (-1.0*n)/(tau*iRowCount);
        iPos++;
    }

    return pd;
}

void CCooleyTukeyFilter::forward(CDEData& orig, std::vector<double*>* pRe, std::vector<double*>* pIm)
{
    CCooleyTukeyFFT ct;
    int iRowCount = orig.getRowCount();
    for (int j = orig.getDependendVariableOffset(); j < orig.getColCount(); j++)
    {
        double* pdRe = new double[iRowCount];
        double* pdIm = new double[iRowCount];
        for (int i = 0; i < iRowCount; i++)
        {
            pdRe[i] = orig.get(j, i);
            pdIm[i] = 0.0;
        }
        ct.fft(1, getPowerBase2(iRowCount), pdRe, pdIm);
        pRe->push_back(pdRe);
        pIm->push_back(pdIm);
    }
}

double* CCooleyTukeyFilter::createTimeData(CDEData& orig)
{
    int iRowCount = orig.getRowCount();
    double * pdt = new double[iRowCount];
    for (int i = 0; i < iRowCount; i++) pdt[i] = orig.get(0, i);
    return pdt;
}

void CCooleyTukeyFilter::backward(std::vector<CDEData*>& complex, CDEData& orig, std::vector<double*>* pRe)
{
    CCooleyTukeyFFT ct;
    int iRowCount = orig.getRowCount();
    CDEData& re = *complex[0];
    CDEData& im = *complex[1];

    for (int j = orig.getDependendVariableOffset(); j < orig.getColCount(); j++)
    { 
        double* pdRe = new double[iRowCount];
        double* pdIm = new double[iRowCount];
        for (int i = 0; i < orig.getRowCount(); i++)
        {
            pdRe[i] = re.get(j, i);
            pdIm[i] = im.get(j, i);
        }
        ct.fft(-1, getPowerBase2(iRowCount), pdRe, pdIm);
        pRe->push_back(pdRe);
        delete []pdIm; pdIm = 0;
    }
}

CDEData* CCooleyTukeyFilter::filterState(CDEData& orig, double dEps, int iSob)
{
    std::vector<CDEData*>* pVD = ftReal(orig);
    for (int i = 0; i < pVD->at(0)->getRowCount(); i++)
        for (int j = pVD->at(0)->getDependendVariableOffset(); j < pVD->at(0)->getColCount(); j++)
        {
            double dNu = pVD->at(0)->get(0, i);
            double dRegFactor = (1.0 + dEps * pow(1.0 + dNu*dNu, 1.0 * iSob)*dNu*dNu);
            pVD->at(0)->set(j, i, pVD->at(0)->get(j, i)/dRegFactor);
            pVD->at(1)->set(j, i, pVD->at(1)->get(j, i)/dRegFactor);
        }
    CDEData* pDEDF = ift(pVD, orig);
    cleanUp(0, pVD);
    return pDEDF;
}

CDEData* CCooleyTukeyFilter::filterFirstDerivative(CDEData& orig, double dEps, int iSob)
{
    std::vector<CDEData*>* pVD = ftReal(orig);
    for (int i = 0; i < pVD->at(0)->getRowCount(); i++)
        for (int j = pVD->at(0)->getDependendVariableOffset(); j < pVD->at(0)->getColCount(); j++)
        {
            double dOm = 2.0*M_PI*pVD->at(0)->get(0, i);
            double dNu = pVD->at(0)->get(0, i);
            double dRegFactor = (1.0 + dEps*pow(1.0 + dNu*dNu, 1.0*iSob)*dNu*dNu);
            double dImNew = pVD->at(0)->get(j, i)/dRegFactor;
            double dReNew = pVD->at(1)->get(j, i)/dRegFactor;
            pVD->at(0)->set(j, i, -1.0*dReNew*dOm);
            pVD->at(1)->set(j, i, dImNew*dOm);
        }
    CDEData* pDEDF = ift(pVD, orig);
    cleanUp(0, pVD);
    return pDEDF;
}

bool CCooleyTukeyFilter::checkPowerOf2(int iRowCount)
{
    int iMaxPower = 30, iTest = 1;
    for (int i = 0; i < iMaxPower; i++)
    {
        iTest*=2;
        if (iRowCount == iTest) return true; 
    };
    return false;
}


int CCooleyTukeyFilter::getPowerBase2(int iRowCount)
{
    int iPower = 0, iRowC = iRowCount;
    while (iRowC > 1)
    {
        iRowC = iRowC/2;
        iPower++;
    };
    return iPower;
}
