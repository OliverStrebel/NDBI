import numpy
import matplotlib.pyplot as plt

from bock.cubic2D import Cubic2D

# Einlesen Deriv Ergebnis
# ausgeben
# mit x_bockLine, ad.parValCur die Ableitung ausrechnen
# ebenfalls ausgeben

def saveFig(ad, x_bockLine, out='graphs'):
    dbiData = numpy.loadtxt(open("../data/experimental/AS_IS/DIFFERENTIATION_BY_INTEGRATION/DERIV_LynxHareHudsonBay_1900_1920_0000000000.PREP_DAT"), delimiter=' ', skiprows=1)
    bockTangent = _bockTangent(ad, x_bockLine)
    numDeriv = _numericalDerivative(x_bockLine)
    _plotHares(bockTangent, dbiData, numDeriv, out)
    _plotLynx(bockTangent, dbiData, numDeriv, out)


def _bockTangent(ad, x_bockLine):
    bockTangent = numpy.copy(x_bockLine)
    iCompCount = len(x_bockLine[0,:])-1
    rhs = Cubic2D()
    data = x_bockLine[:,1:(iCompCount+1)]

    for j in range(iCompCount):
        for i in range(len(bockTangent[:,0])):
            bockTangent[i,j+1] = rhs.f(data[i,:], ad.parActive[j], ad.parValCur[:,j])
    return bockTangent


def _plotHares(bockTangent, dbiData, numDeriv, out='graphs'):
    plt.xlim(1899, 1921)
    plt.xticks([1900, 1904, 1908, 1912, 1916, 1920])
    plt.xlabel("$year$")
        
    plt.ylabel("derivative of hare data")
    plt.scatter(dbiData[:,0],
                dbiData[:,1],
                color = 'black',
                label="NDBI")
    plt.plot(bockTangent[:,0],
             bockTangent[:,1],
             color = 'black',
             label="Bock result")
    plt.plot(numDeriv[:,0],
             numDeriv[:,1],
             color = 'black',
             linestyle='dotted',
             label="num. Derivative")
    plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
               ncol=3, fancybox=False, shadow=False)

    plt.savefig(out + '/figHaresDeriv.jpeg')
    plt.close()


def _plotLynx(bockTangent, dbiData, numDeriv, out='graphs'):
    plt.xlim(1899, 1921)
    plt.xticks([1900, 1904, 1908, 1912, 1916, 1920])
    plt.xlabel("$year$")
        
    plt.ylabel("derivative of lynx data")
    plt.scatter(dbiData[:,0],
                dbiData[:,2],
                color = 'black',
                label="NDBI")
    plt.plot(bockTangent[:,0],
             bockTangent[:,2],
             color = 'black',
             label="Bock result")
    plt.plot(numDeriv[:,0],
             numDeriv[:,2],
             color = 'black',
             linestyle='dotted',
             label="num. Derivative")
    plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
               ncol=3, fancybox=False, shadow=False)

    plt.savefig(out + '/figLynxDeriv.jpeg')
    plt.close()

    
def _numericalDerivative(data):
    deriv = numpy.empty_like(data)
    deriv[:,0] = data[:,0]
    for j in range(1, len(data[0,:])):
        deriv[0,j] = (data[1,j] - data[0,j])/(data[1,0] - data[0,0])
    for i in range(1, len(data[:, 0])-1):
        for j in range(1, len(data[0,:])):
            deriv[i,j] = (data[i+1,j] - data[i-1,j])/(data[i+1,0] - data[i-1,0])
    iLast = len(deriv[:,0]) - 1
    for j in range(1, len(data[0,:])):
        deriv[iLast,j] = (data[iLast,j] - data[iLast-1,j])/(data[iLast,0] - data[iLast-1,0])

    return deriv
