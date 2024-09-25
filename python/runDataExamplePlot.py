import numpy
import matplotlib.pyplot as plt
import matplotlib.markers as mrk

noisy = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/NOISY_DATA_USED/Jerk_2_20.used"), delimiter=' ', skiprows=1)
ref = numpy.loadtxt(open("../data/Jerks/Jerk_2.dat"), delimiter=' ', skiprows=1)
dbi = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/DIFFERENTIATION_BY_INTEGRATION/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)
fd = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/FINITE_DIFF/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)
sl = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/SPECTRAL_LOWPASS/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)
fmw = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/FRIEDRICHS_MOLLIFIER_WEAK/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)
fmn = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/FRIEDRICHS_MOLLIFIER_NUMERIC/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)
tangent = numpy.loadtxt(open("../data/Jerks/SKIP_SCAN/TANGENT_REGRESSION/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)



plt.xlabel("time")
plt.ylabel("$x_1$")
plt.plot(ref[0:2000,0]
         , ref[0:2000,1]
         , color = "black"
         , label="numerical data")
plt.scatter(noisy[0:97,0]
            , noisy[0:97,1]
            , color = "black"
            , label="noisy data points used")
plt.legend(loc='upper center'
           , bbox_to_anchor=(0.5, 1.1)
           , ncol=2
           , fancybox=False
           , shadow=False)
plt.savefig('graphs/figDataExample.jpeg')
plt.show()
plt.close()



plt.xlabel("time")
plt.ylabel("$d x_1/dt$")
plt.scatter(fd[0:97,0]
            , fd[0:97,1]
            , color = "grey"
            , label = "finite differences"
            , marker=mrk.MarkerStyle('o', fillstyle='none'))
plt.scatter(sl[0:89,0]
            , sl[0:89,1]
            , color = "grey"
            , label = "spectral lowpass"
            , marker=mrk.MarkerStyle('s', fillstyle='none'))
plt.plot(ref[0:2000,0]
         , ref[0:2000,2]
         , label = "theoretical derivative"
         , color = "black")
plt.scatter(dbi[0:97,0]
            , dbi[0:97,1]
            , label = "NDBI"
            , color = "black" )
plt.legend(loc='upper center'
           , bbox_to_anchor=(0.5, 1.15)
           , ncol=2
           , fancybox=False
           , shadow=False)
plt.savefig('graphs/figDerivativeExample_1.jpeg')
plt.show()
plt.close()


plt.xlabel("time")
plt.ylabel("$d x_1/dt$")
plt.scatter(fmw[0:95,0]
            , fmw[0:95,1]
            , color = "grey"
            , label = "Friedrichs mollifier weak"
            , marker=mrk.MarkerStyle('o', fillstyle='none'))
plt.scatter(fmn[0:95,0]
            , fmn[0:95,1]
            , color = "grey"
            , label = "Friedrichs mollifier numeric"
            , marker=mrk.MarkerStyle('^', fillstyle='none'))
plt.scatter(tangent[0:95,0]
            , tangent[0:95,1]
            , color = "grey"
            , label = "tangent regression"
            , marker=mrk.MarkerStyle('s', fillstyle='none'))
plt.plot(ref[0:2000,0]
         , ref[0:2000,2]
         , label = "theoretical derivative"
         , color = "black")
plt.scatter(dbi[0:97,0]
            , dbi[0:97,1]
            , label = "NDBI"
            , color = "black" )
plt.legend(loc='upper center'
           , bbox_to_anchor=(0.5, 1.15)
           , ncol=3
           , fancybox=False
           , shadow=False)
plt.savefig('graphs/figDerivativeExample_2.jpeg')
plt.show()
plt.close()

print("End program")
