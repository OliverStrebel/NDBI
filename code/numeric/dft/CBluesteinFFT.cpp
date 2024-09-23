// CBluesteinFFT.cpp: Implementierung der Klasse CBluesteinFFT.
//
//////////////////////////////////////////////////////////////////////

#include "CBluesteinFFT.h"

#include "CCooleyTukeyFFT.h"

#include "util/CLog.h"

#include <complex>
#include <iostream>
#include <valarray>

const double PI = 3.141592653589793238;

CBluesteinFFT::CBluesteinFFT()
{
}

CBluesteinFFT::~CBluesteinFFT()
{
}

/*
*/
void CBluesteinFFT::fft(short int dir, int iSize, double* x, double* y)
{
     CComplexVector z(getNextPowerOf2(iSize));
     for (size_t i = 0; i < iSize; i++) z[i] = CComplex(x[i], y[i]);

     if (dir == 1) fft(z, iSize);
     else ifft(z, iSize);

     for (size_t i = 0; i < iSize; i++)
     {
         x[i] = z[i].real();
         y[i] = z[i].imag();
     }
}

void CBluesteinFFT::fft(CComplexVector& z, int iSize)
{
    CComplexVector phase = getPhase(z.size(), iSize);
    CComplexVector a = z*phase, b = get_b(z.size(), iSize);
    CCooleyTukeyFFT ct;
    ct.fft(a);
    ct.fft(b);
    CComplexVector c = a*b;
    ct.ifft(c);
    z = phase*c;
}

void CBluesteinFFT::ifft(CComplexVector& z, int iSize)
{
    z = z.apply(std::conj);
    fft(z, iSize);
    z = z.apply(std::conj);
    CComplex c(iSize, 0.0);
    z /= c;
}

size_t CBluesteinFFT::getNextPowerOf2(int iSize)
{
    size_t iPowerOf2 = 2;
    size_t stSize = (size_t) iSize;
    while (iPowerOf2 < (2*stSize-1)) iPowerOf2*=2;
    return iPowerOf2;
}

CComplexVector CBluesteinFFT::getPhase(size_t size, int iSize)
{
    CComplexVector phase(size);
    for (size_t n = 0;  n < iSize; n++) phase[n] = std::polar(1.0, -(PI*n*n)/(1.0* iSize));
    return phase;
}

CComplexVector CBluesteinFFT::get_b(size_t size, int iSize)
{
    CComplexVector b(size);
    b[0] = std::polar(1.0, 0.0);
    size_t M = size;
    for (size_t n = 1; n < iSize; n++)
    {
        b[n] = std::polar(1.0, (PI*n*n)/(1.0*iSize));
        b[M-n] = b[n];
    }
    return b;
}
