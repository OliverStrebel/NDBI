import numpy

from bock.cubic2D import Cubic2D

class AlgoData:
    def __init__(_
                 , t_: numpy.ndarray
                 , data: numpy.ndarray
                 , param: numpy.ndarray):
        _.t = t_
        _.d = data
        _.parActive = _._getParameterActivity(param, data)
        _.parValPre = _._getParameterValues(param, data) #from preprocessing
        _.parValCur = _._getParameterValues(param, data) #current from Newton iteration
        _.rhs = _._getOdeRightHandSide(data)
        _.interSteps = 999
        _.targetEuclideanDistance = 1.0E-014
        _.maxIter = 1000
        _.minIncrementNorm = 1.0E-009
        _.numRowsNewton = 200
        _.epsGradient = 0.00001 #works better than Dennis-Schnabel macheps for optimization
        _.activeParameterCount = _._getActiveParameterCount()       
        
    def _getParameterActivity(_, par: numpy.ndarray, data: numpy.ndarray):
        parAct = []
        for i in range(len(data[0,:])):
            parAct.append([-1]*_._getParameterCount(data))
        for i in range(len(par[:,0])):
            compActivity = parAct[int(par[i,0])]
            compActivity[int(par[i,1])] = int(par[i,1])
        return parAct


    def _getParameterValues(_, par: numpy.ndarray, data: numpy.ndarray):
        parVal = numpy.zeros((_._getParameterCount(data), len(data[0,:])))
        for i in range(len(par[:,0])):
            parVal[int(par[i,1]), int(par[i,0])] = par[i, 2]
        return parVal


    def _getOdeRightHandSide(_, data: numpy.ndarray):
        fun = 0
        if (len(data[0,:]) == 2): fun = Cubic2D()
        else: raise Exception("AlgoData._getOdeRightHandSide: Only two dimensional Problems implemented!") 
        return fun


    def _getParameterCount(_, data: numpy.ndarray):
        parameterCount = 0 #number of monomial termes
        if (len(data[0,:]) == 2): parameterCount = 10
        else: raise Exception("AlgoData._getParameterActivity: Only two dimensional Problems implemented!") 
        return parameterCount

    def _getActiveParameterCount(_):
        iCount = 0
        for i in range(len(_.parActive)):
            for j in range(len(_.parActive[i])):
                if _.parActive[i][j] > -1: iCount = iCount+1
        return iCount
