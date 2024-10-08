//CSpectralLowpass.cpp: Implementierung der Klasse CSpectralLowpass.
///////////////////////////////////////////////////////////////////////////////
#include "CSpectralLowpass.h"


#include "pipeline/datastructures/CPipelineData.h"
#include "pipeline/datastructures/CDEData.h"

#include "numeric/dft/CBluesteinFFT.h"
#include "numeric/dft/CBluesteinFilter.h"

#include "numeric/linalg/CCMVector.h"
#include "numeric/linalg/CCMMatrix.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"

#include <limits>
#include <cmath>

CSpectralLowpass::CSpectralLowpass(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CSpectralLowpass::~CSpectralLowpass()
{
}


CPipelineData* CSpectralLowpass::next()
{
    CPipelineData* pPD = getPrevious()->next();
	filterData(pPD);
	return pPD;
}

void CSpectralLowpass::filterData(CPipelineData* pPD)
{
	std::vector<double> epsPerComp = getFilterParameter(pPD);
	CDEData* pState = getData(pPD);
	CDEData* pDeriv = getData(pPD);
	for (int iComp = 0; iComp < (int) epsPerComp.size(); iComp++) filterComponent(*pState, *pDeriv, epsPerComp[iComp], iComp);
	pPD->addPreparedDEData(eliminateWindowEffect(pState));
	pPD->addPreparedDEData(eliminateWindowEffect(pDeriv));
	cleanUp(pState, 0, pDeriv);
}

std::vector<double> CSpectralLowpass::getFilterParameter(CPipelineData* pPD)
{
	std::vector<double> epsOptPerComp;
	CDEData* pDEDP2 = getData(pPD);

	CBluesteinFilter f;
	std::vector<CDEData*>* pvF = f.ftReal(*pDEDP2);

	std::pair<double*, double*> cx(new double[pDEDP2->getRowCount()], new double[pDEDP2->getRowCount()]);
	for (int iComp = pDEDP2->getDependendVariableOffset(); iComp < pDEDP2->getColCount(); iComp++)
	{ 
		std::vector<double> eps, dist;
		for (int k = 0; k < cfg().getEpsSpectralSteps(); k++)
		{
			eps.push_back(cfg().getEpsSpectralStart() + cfg().getEpsSpectralDelta()*k);
			ifft(*pvF, cx, eps[eps.size()-1], iComp);
			dist.push_back(getDistance(*pDEDP2, cx, iComp));
		}
		epsOptPerComp.push_back(getEpsTurningPoint(eps, dist));
	}
	cleanUp(pDEDP2, pvF);
	delete cx.first; delete cx.second; cx.first = 0; cx.second = 0;
	return epsOptPerComp;
}

CDEData* CSpectralLowpass::getData(CPipelineData* pPD)
{
	CDEData& aDED = pPD->getDEData();
	int size = aDED.getRowCount();
    if (size%2 != 0) size-=1;//Wegen Filter. Der kann nur gerade, wegen der Frequenzarrays.
	std::vector<double*>* pdv = new std::vector<double*>();
	for (int j = 0; j < aDED.getColCount(); j++)
	{
		double* pd = new double[size];
		for (int i = 0; i < size; i++) pd[i] = aDED.get(j, i);
		pdv->push_back(pd);
	}
	return new CDEData(size, aDED.getDependendVariableOffset(), pdv);
}

void CSpectralLowpass::ifft(std::vector<CDEData*>& vF, std::pair<double*, double*>& cx, double dEps, int iComp)
{
	for (int i = 0; i < vF[0]->getRowCount(); i++)
	{
		double dNu = vF[0]->get(0, i);
		double dRegFactor = 1.0/(1.0 + dEps*dNu*dNu);
		cx.first[i] = vF[0]->get(iComp, i)*dRegFactor;
		cx.second[i] = vF[1]->get(iComp, i)*dRegFactor;
	}
    CBluesteinFFT blue;
	blue.fft(-1, vF[0]->getRowCount(), cx.first, cx.second);
}

double CSpectralLowpass::getDistance(CDEData& aDED, std::pair<double*, double*>& cx, int iComp)
{
	double distance = 0.0;
    int iExcludedPoints = (int) std::round(aDED.getRowCount()*cfg().getWindowEffectEliminationPercent());
	int iStart = iExcludedPoints, iEnd = aDED.getRowCount() - iExcludedPoints;
	for (int i = iStart; i < iEnd; i++) distance+=(pow(aDED.get(iComp, i) - cx.first[i], 2.0));
	return sqrt(distance);
}

double CSpectralLowpass::getEpsTurningPoint(std::vector<double>& eps, std::vector<double>& dist)
{
    double dMin = std::numeric_limits<double>::max();
	size_t kTurn = 0;
	for (size_t k = 1; k < eps.size(); k++)
	{
		if (fabs(dist[k] - dist[k - 1]) < dMin)
		{
		    kTurn = k;
			dMin = fabs(dist[k] - dist[k - 1]);
     	} else return eps[kTurn];//Erster Wendepunkt wird genommen
	}
	return getEpsWithKneeMultiplier(eps, dist);
}


double CSpectralLowpass::getEpsWithKneeMultiplier(std::vector<double>& eps, std::vector<double>& dist)
{
	double dMinCurvature = std::numeric_limits<double>::max();
	size_t kMinCurvature = 0;
	for (size_t k = 1; k < eps.size()-1; k++)
	{
	    double dTest = (dist[k+1] + dist[k-1] - 2.0*dist[k])/((eps[k] - eps[k-1])*(eps[k+1] - eps[k]));
		if (dTest < dMinCurvature)
		{
			kMinCurvature = k;
			dMinCurvature = dTest;
		}
	}
	return cfg().getKneeMultiplier()*eps[kMinCurvature];
}

void CSpectralLowpass::filterComponent(CDEData& aState, CDEData& aDeriv, double dEps, int iComp)
{
    CBluesteinFilter f;
	CDEData* pState = f.filter(aState, dEps, 0);
	CDEData* pDeriv = f.filter(aState, dEps, 1);
	int iOffset = aState.getDependendVariableOffset();
	for (int i = 0; i < aState.getRowCount(); i++)
	{
	    aState.set(iComp + iOffset, i, pState->get(iComp + iOffset, i));
		aDeriv.set(iComp + iOffset, i, pDeriv->get(iComp + iOffset, i));
	}
	cleanUp(pState, 0, pDeriv);
}

CDEData* CSpectralLowpass::eliminateWindowEffect(CDEData* pDED)
{
    int iStart = (int) std::round(pDED->getRowCount()*cfg().getWindowEffectEliminationPercent());
	int iSize = pDED->getRowCount() - 2*iStart;
	std::vector<double*>* pd = new std::vector<double*>();
	for (int j = 0; j < pDED->getColCount(); j++) pd->push_back(new double[iSize]);

	for (int j = 0; j < pDED->getColCount(); j++)
	    for (int i = 0; i < iSize; i++) (*pd)[j][i] = pDED->get(j, i + iStart);
	
	return new CDEData(iSize, pDED->getDependendVariableOffset(), pd);
}

void CSpectralLowpass::cleanUp(CDEData* pDED, std::vector<CDEData*>* pvD, CDEData* pDEDOther)
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
