import numpy
import sys

from scipy.linalg import lstsq
from scipy import linalg

from constants import Constants as C

from bock.implicitEuler import ImplicitEuler
from bock.algoData import AlgoData

# This is a simple version of Bocks algorithm. See:
#    Baake E., Baake M., Bock H., Briggs K.: Fitting ordinary differential
#    equations to chaotic data.
#    Physical Review A (1992).
#    http://dx.doi.org/10.1103/PhysRevA.45.5524
# Only one curve segment for the entire data set is used.
# The start parameters are those obtained by the preprocessing method.

# The Gauss-Newton-Algorithm is used

# Automatic differentiation of algorithms is avoided for simplicity and
# due to the zeroth rule of automatic differentiation of Griewank:
# "Finite differences work too"

# Uses error oriented Newton iteration
#    See: P. Deuflhard, Newton Methods for Nonlinear Problems (2nd printing)
#                       Heidelberg: Springer Series in Computational Mathematics
#                       Vol. 35 (2006)
# The reason for this choice is that the model data can be
# quite far away from the experimental data, so that residual-oriented Newton
# method might have large errors

class SimpleBockAlgorithm:
    def __init__(_):
        i = 4


    def run(_, ad: AlgoData):
        _._newtonIteration(ad)
        return _._integrate(ad)


#############################################################################
# private
    def _newtonIteration(_, ad):
        print("Start parameters before Bock Algo: \n" + str(ad.parValCur))
        inc = sys.float_info.max
        it = 0
        x = _._getParameterAsVector(ad)
        while (inc > ad.minIncrementNorm and it < ad.maxIter):
            A, b = _._setRegressionEquation(ad, x)
            d, res, r, s = lstsq(A, b, lapack_driver='gelsy')#gelsy getestet
            inc  = numpy.linalg.norm(d)
            print("Increment Bock algorithm: " + str(inc))
            x = numpy.add(x, d)
            it = it+1
        _._setVectorAsParCur(ad, x)
        print("Refined Parameters after Bock Algo: \n" + str(ad.parValCur))


    def _setRegressionEquation(_, ad, x):
        return _._get_A(ad, x), _._get_b(ad)


    def _get_b(_, ad):
        b = numpy.zeros(len(ad.d[:,0])*len(ad.d[0,:]))
        intData, xLine = _._integrate(ad)
        for i in range(0, len(b), len(intData[0,:])):
            for iComp in range(0, len(intData[0,:])):
                iPos = int(i/len(intData[0,:]))
                b[i+iComp] = ad.d[iPos, iComp] - intData[iPos, iComp]
        return b


    def _get_A(_, ad, x):
        A = numpy.zeros((len(ad.d[:,0])*len(ad.d[0,:]), ad.activeParameterCount))
        for j in range(len(x)):
            h = ad.epsGradient*x[j]
            x[j] = x[j] + h
            upper = _._integrateVector(ad, x)
            x[j] = x[j] - 2.0*h
            lower = _._integrateVector(ad, x)
            x[j] = x[j] + h
            for i in range(0, len(A[:,0]), len(ad.parActive)):
                for iComp in range(0, len(ad.parActive)):
                    iPos = int(i/len(ad.parActive))
                    A[i+iComp, j] = (upper[iPos,iComp] - lower[iPos,iComp])/(2.0*h) 
        return A
        

    def _getParameterAsVector(_, ad):
        iPos = 0
        x = numpy.zeros(ad.activeParameterCount)
        for i in range(len(ad.parActive)):
            for j in range(len(ad.parActive[i])):
                if ad.parActive[i][j] > -1:
                    x[iPos] = ad.parValCur[j, i]
                    iPos = iPos + 1
        return x


    def _setVectorAsParCur(_, ad, x):
        iPos = 0
        for i in range(len(ad.parActive)):
            for j in range(len(ad.parActive[i])):
                if ad.parActive[i][j] > -1:
                    ad.parValCur[j, i] = x[iPos]
                    iPos = iPos + 1


    def _integrateVector(_, ad, x):
        _._setVectorAsParCur(ad, x)
        points, line = _._integrate(ad)
        return points


    def _integrate(_, ad):
        integrator = ImplicitEuler()
        return integrator.run(ad)

