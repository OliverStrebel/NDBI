import numpy
import matplotlib.pyplot as plt

from aggregation.AggregatedResults import AggregatedResults
from constants import Constants as C


def scanGraphics(ar, scalefactor, out='graphs'):
    x_axis = ar.scan_param*scalefactor

    myMarkers=['x', 'o', 'D', 's', '+', '^']
    myFaceColors=['black', 'none', 'none', 'black', 'black', 'none']

    if scalefactor < 0.9:
        plt.xlim(-0.1, 5.1)
        plt.xticks([0.0, 1.0, 2.0, 3.0, 4.0, 5.0])
        plt.xlabel("$q$")
    else:
        plt.xlim(-0.1, 20.1)
        plt.xticks([0.0, 4.0, 8.0, 12.0, 16.0, 20.0])
        plt.xlabel("$No$ $of$ $skipped$ $data$ p$oints$")
        
    plt.ylabel("ARD")
    plt.yscale("log")    
    i = 0
    for algo in ar.algos:
        y = ar.avg_abs_diff[i,]
        plt.scatter(x_axis,
                    y,
                    marker=myMarkers[i],
                    color='black',
                    facecolors=myFaceColors[i],
                    label=C.algo_shortcut[i])
        plt.plot(x_axis,
                 y,
                 color='black',
                 linestyle='dotted')
        i=i+1

    plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
               ncol=3, fancybox=False, shadow=False)
    if scalefactor < 0.9: plt.savefig(out + '/meanRelativeDeviationPerDataPointNoise.jpeg')
    else: plt.savefig(out + '/meanRelativeDeviationPerDataPointSkip.jpeg')
    plt.close()
 
