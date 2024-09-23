// CBluesteinFFT.h: Schnittstelle für die Klasse CBluesteinFFT.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CBLUESTEINFFT_H)
#define CBLUESTEINFFT_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef __COMPLEX__
#include <complex>
#endif

#ifndef __VALARRAY__
#include <valarray>
#endif

typedef std::complex<double> CComplex;
typedef std::valarray<CComplex> CComplexVector;


class CBluesteinFFT
{
public:
	CBluesteinFFT();	
	virtual ~CBluesteinFFT();

    void fft(short int dir, int iSize, double* x, double* y);

private:
    void fft(CComplexVector& z, int iSize);
    void ifft(CComplexVector& z, int iSize);

    size_t getNextPowerOf2(int iSize);
    CComplexVector getPhase(size_t size, int iSize);
    CComplexVector get_b(size_t size, int iSize);
};

#endif //CBLUESTEINFFT_H

