import sys
import numpy
import scipy.signal as sig

import matplotlib.pyplot as plt

noisy_data = numpy.loadtxt(open("../../../data/Jerks/SKIP_SCAN/NOISY_DATA_USED/Jerk_2_20.used"), delimiter=' ', skiprows=1)
ref = numpy.loadtxt(open("../../../data/Jerks/Jerk_2.DAT_DERIVED"), delimiter=' ', skiprows=1)
dbi = numpy.loadtxt(open("../../../data/Jerks/SKIP_SCAN/DIFFERENTIATION_BY_INTEGRATION/DERIV_Jerk_2_20.prep_dat"), delimiter=' ', skiprows=1)

windowMin = 0
orderMin = 0
minNorm = sys.float_info.max

for iWindow in range(2, 30):
    for iOrder in range(1, iWindow-1): 
        sgd = sig.savgol_filter(noisy_data[:,1], iWindow, iOrder, 1, 0.21, mode = 'nearest')
        dist = numpy.linalg.norm(dbi[:,1] - sgd)
        if dist < minNorm:
            minNorm = dist
            windowMin = iWindow
            orderMin = iOrder
print(windowMin)
print(orderMin)

denoised = sig.savgol_filter(noisy_data[:,1], windowMin, orderMin, 2, 0.21, mode = 'nearest')

plt.xlabel("time")
plt.ylabel("$d^2 x_1/dt^2$")
plt.scatter(dbi[0:97,0], denoised[0:97], color = 'black', label = "Savitsky-Golay")
plt.plot(ref[0:2000,0], ref[0:2000,2],  color = 'black', label = "theoretical derivative")
plt.legend(loc='upper center'
           , bbox_to_anchor=(0.5, 1.1)
           , ncol=2
           , fancybox=False
           , shadow=False)
plt.savefig('fig_second_deriv.jpeg')
plt.show()
plt.close()

denoised = sig.savgol_filter(noisy_data[:,1], windowMin, orderMin, 3, 0.21, mode = 'nearest')

plt.xlabel("time")
plt.ylabel("$d^3 x_1 / dt^3$")
plt.scatter(dbi[0:97,0], denoised[0:97], color = 'black', label = "Savitsky-Golay")
plt.plot(ref[0:2000,0], ref[0:2000,3],  color = 'black', label = "theoretical derivative")
plt.legend(loc='upper center'
           , bbox_to_anchor=(0.5, 1.1)
           , ncol=2
           , fancybox=False
           , shadow=False)
plt.savefig('fig_third_deriv.jpeg')
plt.show()
plt.show()
plt.close()


print("End program")
