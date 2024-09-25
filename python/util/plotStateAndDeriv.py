import numpy
import matplotlib.pyplot as plt
import matplotlib.markers as mrk

noisy = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/NOISY_DATA_USED/VanDerPol_20.used"), delimiter=' ', skiprows=1)
ref = numpy.loadtxt(open("../../data/Driven3D/VanDerPol.dat"), delimiter=' ', skiprows=1)
dbi = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/DIFFERENTIATION_BY_INTEGRATION/DERIV_VanDerPol_20.prep_dat"), delimiter=' ', skiprows=1)
fd = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/FINITE_DIFF/DERIV_VanDerPol_20.prep_dat"), delimiter=' ', skiprows=1)
lowpass = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/SPECTRAL_LOWPASS/DERIV_VanDerPol_20.prep_dat"), delimiter=' ', skiprows=1)
fmw = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/FRIEDRICHS_MOLLIFIER_WEAK/DERIV_VanDerPol_20.prep_dat"), delimiter=' ', skiprows=1)
fmn = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/FRIEDRICHS_MOLLIFIER_NUMERIC/DERIV_VanDerPol_20.prep_dat"), delimiter=' ', skiprows=1)
tangent = numpy.loadtxt(open("../../data/Driven3D/SKIP_SCAN/TANGENT_REGRESSION/DERIV_VanDerPol_20.prep_dat"), delimiter=' ', skiprows=1)

plt.plot(ref[0:2050,0], ref[0:2050,1], color = "black")
plt.scatter(noisy[0:97,0], noisy[0:97,1], color = "black")
plt.show()

plt.scatter(fd[0:97,0], fd[0:97,1], color = "grey", marker=mrk.MarkerStyle('o', fillstyle='none'))
plt.scatter(lowpass[0:97,0], lowpass[0:97,1], color = "grey", marker=mrk.MarkerStyle('s', fillstyle='none'))
plt.plot(ref[0:2000,0], ref[0:2000,2], color = "black")
plt.scatter(dbi[0:97,0], dbi[0:97,1], color = "black" )
plt.show()


plt.scatter(fmw[0:97,0], fmw[0:97,1], color = "grey", marker=mrk.MarkerStyle('o', fillstyle='none'))
plt.scatter(fmn[0:97,0], fmn[0:97,1], color = "grey", marker=mrk.MarkerStyle('^', fillstyle='none'))
plt.scatter(tangent[0:97,0], tangent[0:97,1], color = "grey", marker=mrk.MarkerStyle('s', fillstyle='none'))
plt.plot(ref[0:2000,0], ref[0:2000,2], color = "black")
plt.scatter(dbi[0:97,0], dbi[0:97,1], color = "black" )
plt.show()

print("End program")
