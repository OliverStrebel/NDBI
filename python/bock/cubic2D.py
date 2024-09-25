import numpy

from constants import Constants as C

class Cubic2D:
    def __init__(_):
        i = 4
        
    def f(_, v, adParAct, pars):
        res = 0.0
        for i in range(0, len(adParAct)):
            if (adParAct[i] < 0): continue
            if (adParAct[i] == 0): res = res + pars[i]
            if (adParAct[i] == 1): res = res + pars[i]*v[0]
            if (adParAct[i] == 2): res = res + pars[i]*v[1]
            if (adParAct[i] == 3): res = res + pars[i]*pow(v[0], 2.0)
            if (adParAct[i] == 4): res = res + pars[i]*v[0]*v[1]
            if (adParAct[i] == 5): res = res + pars[i]*pow(v[1], 2.0)
            if (adParAct[i] == 6): res = res + pars[i]*pow(v[0], 3.0)
            if (adParAct[i] == 7): res = res + pars[i]*pow(v[0], 2.0)*v[1]
            if (adParAct[i] == 8): res = res + pars[i]*v[0]*pow(v[1], 2.0)
            if (adParAct[i] == 9): res = res + pars[i]*pow(v[1], 3.0)
                       
        return res 

