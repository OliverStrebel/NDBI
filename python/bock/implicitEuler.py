import numpy

from constants import Constants as C
from bock.algoData import AlgoData

class ImplicitEuler:
    def __init__(_):
        i = 4
        
    def run(_, ad: AlgoData):
        x, xLine = _._initIntegration(ad)
        for i in range(1, len(ad.t)): _._fixPointInteration(i, ad, x, xLine)
        return x, xLine


    def _initIntegration(_, ad):
        x = numpy.zeros((len(ad.d[:,0]), len(ad.d[0, :])))
        x[0,:] = ad.d[0,:]

        iColCount = (len(ad.d[:,0])-1)*(ad.interSteps + 1) + 1
        xLine = numpy.zeros((iColCount, len(ad.d[0, :]) + 1))
        for i in range(len(ad.d[0,:])): xLine[0,i+1] = ad.d[0,i]
        xLine[0,0] = ad.t[0]
        
        return x, xLine


    def _fixPointInteration(_, iPos, ad, x, xLine):
        v, ov, nv = _._initFPIteration(iPos, ad.d)
        iPosLine = (iPos-1)*(ad.interSteps +1) + 1
        if (iPos > 1): v, ov, nv = _._initFPIteration(iPos, x)
        iSize = len(x[0,:])
        dt = (ad.t[iPos] -ad.t[iPos-1])/(1.0 + ad.interSteps)
        for iStep in range(0,ad.interSteps+1):
            it = 0
            error = 1.0
            while ((error > ad.targetEuclideanDistance) and (it < ad.maxIter)):
                for i in range(0, iSize):
                    nv[i] = ov[i] + dt*ad.rhs.f(v, ad.parActive[i], ad.parValCur[:,i])
                error = numpy.linalg.norm(numpy.subtract(v, nv))
                v[:] = nv[:]
                it = it+1
            ov[:] = nv[:]
            for k in range(iSize): xLine[iPosLine, k+1] = nv[k]
            xLine[iPosLine, 0] = xLine[iPosLine-1, 0] + dt
            iPosLine = iPosLine+1
        x[iPos, :] = nv
            

    def _initFPIteration(_, iPos, x):
        return numpy.copy(x[iPos-1,:]), numpy.copy(x[iPos-1,:]), numpy.copy(x[iPos-1,:]) 

