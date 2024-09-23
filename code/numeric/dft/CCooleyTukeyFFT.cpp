// CCooleyTukeyFFT.cpp: Implementierung der Klasse CCooleyTukeyFFT.
//
//////////////////////////////////////////////////////////////////////

#include "CCooleyTukeyFFT.h"

#include <complex>
#include <iostream>
#include <valarray>

const double PI = 3.141592653589793238;

//Recursive Cooley Tukey Algo
CCooleyTukeyFFT::CCooleyTukeyFFT()
{
}

CCooleyTukeyFFT::~CCooleyTukeyFFT()
{
}

/*
This computes an in-place complex-to-complex FFT
dir = 1 gives forward transform
dir = -1 gives reverse transform
*/
void CCooleyTukeyFFT::fft(short int dir, long m, double* x, double* y)
{
     size_t iSize = 1;
     for (int k = 0; k < m; k++) iSize*=2;
     CComplexVector z(iSize);
     for (size_t i = 0; i < iSize; i++) z[i] = CComplex(x[i], y[i]);
     
     if (dir == 1) fft(z);
     else ifft(z);

     for (size_t i = 0; i < iSize; i++)
     {
         x[i] = z[i].real();
         y[i] = z[i].imag();
     }
}

void CCooleyTukeyFFT::fft(CComplexVector& z)
{
    const size_t size = z.size();
    if (size <= 1) return;

    CComplexVector z_even = z[std::slice(0, size/2, 2)];
    CComplexVector z_odd = z[std::slice(1, size/2, 2)];
    fft(z_even);
    fft(z_odd);

    for (size_t k = 0; k < size / 2; ++k)
    {
        CComplex t = std::polar(1.0, -2*PI*k/size)*z_odd[k];
        z[k] = z_even[k] + t;
        z[k + size/2] = z_even[k] - t;
    }
}

void CCooleyTukeyFFT::ifft(CComplexVector& z)
{
    z = z.apply(std::conj);
    fft(z);
    z = z.apply(std::conj);
    CComplex c(1.0*z.size(), 0.0);
    z /= c;
}

