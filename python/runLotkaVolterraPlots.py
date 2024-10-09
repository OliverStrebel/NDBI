import sys
import os

import matplotlib.pyplot as plt
import numpy

from bock.implicitEuler import ImplicitEuler
from bock.algoData import AlgoData
from bock.simpleBockAlgorithm import SimpleBockAlgorithm

import graphs.lotkaVolterraState as state
import graphs.lotkaVolterraDeriv as deriv


data = numpy.loadtxt(open("../data/experimental/LynxHareHudsonBay_1900_1920.txt"), delimiter=' ', skiprows=1)
par = numpy.loadtxt(open("../data/experimental/AS_IS/DIFFERENTIATION_BY_INTEGRATION/PAR_LOO_LynxHareHudsonBay_1900_1920_0000000000.RES"), delimiter=' ')
ad = AlgoData(data[:,0], data[:,1:3], par)
integrator = ImplicitEuler()
x_pre, x_preLine = integrator.run(ad)

bock = SimpleBockAlgorithm()
x_bock, x_bockLine = bock.run(ad)

state.saveFig(ad, x_pre, x_bock, x_preLine, x_bockLine)
deriv.saveFig(ad, x_bockLine)

print('End program')
