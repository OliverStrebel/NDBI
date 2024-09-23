// CCooleyTukeyFFT.h: Schnittstelle für die Klasse CCooleyTukeyFFT.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CCOOLEYTUKEYFFT_H)
#define CCOOLEYTUKEYFFT_H

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


//Recursive Cooley Tukey Algo
class CCooleyTukeyFFT
{
friend class CBluesteinFFT;

public:
	CCooleyTukeyFFT();	
	virtual ~CCooleyTukeyFFT();

    void fft(short int dir, long m, double* x, double* y);

private:
    void fft(CComplexVector& z);
    void ifft(CComplexVector& z);
};

#endif //CCOOLEYTUKEYFFT_H

