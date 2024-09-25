import numpy
import matplotlib.pyplot as plt
import matplotlib.markers as mrk

ref = numpy.loadtxt(open("../../data/Hamiltonian4D/Contopoulos2.DAT_DERIVED"), delimiter=' ', skiprows=1)
dbi = numpy.loadtxt(open("../../data/Hamiltonian4D/SKIP_SCAN/DIFFERENTIATION_BY_INTEGRATION/DERIV_Contopoulos2_18.prep_dat"), delimiter=' ', skiprows=1)

plt.plot(ref[0:4000,0], ref[0:4000,4], color = "black")
plt.scatter(dbi[:,0], dbi[:,4], color = "black")
plt.show()

print("End program")
