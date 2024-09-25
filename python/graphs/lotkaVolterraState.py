import numpy
import matplotlib.pyplot as plt

from constants import Constants as C


def saveFig(ad, x_pre, x_bock, x_preLine, x_bockLine, out='graphs'):
    t = ad.t
    data = ad.d
    plt.xlim(1899, 1921)
    plt.xticks([1900, 1904, 1908, 1912, 1916, 1920])
    plt.xlabel("$year$")
        
    plt.ylabel("Thousand of hares")
    plt.scatter(t,
                data[:,0],
                marker='o',
                color='black',
                facecolors='black',
                label="data")
    plt.plot(x_preLine[:,0],
             x_preLine[:,1],
             linestyle='dotted',
             color = 'black',
             label="Prepocessing")
    plt.plot(x_bockLine[:,0],
             x_bockLine[:,1],
             color = 'black',
             label="Bock refinement")
    plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
               ncol=3, fancybox=False, shadow=False)

    plt.savefig(out + '/figHaresState.jpeg')
    plt.close()
 


    plt.xlim(1899, 1921)
    plt.xticks([1900, 1904, 1908, 1912, 1916, 1920])
    plt.xlabel("$year$")
        
    plt.ylabel("Thousand of lynx")
    plt.scatter(t,
                data[:,1],
                marker='o',
                color='black',
                facecolors='black',
                label="data")
    plt.plot(x_preLine[:,0],
             x_preLine[:,2],
             linestyle='dotted',
             color = 'black',
             label="Prepocessing")
    plt.plot(x_bockLine[:,0],
             x_bockLine[:,2],
             color = 'black',
             label="Bock refinement")
    plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
               ncol=3, fancybox=False, shadow=False)

    plt.savefig(out + '/figLynxState.jpeg')
    plt.close()
