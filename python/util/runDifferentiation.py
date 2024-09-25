import glob
import numpy

def differentiate(resultFileName):
    file = open(resultFileName)
    data = numpy.loadtxt(file, delimiter=' ', skiprows=1)
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
    file.close()
    return deriv



def writeDerivative(fileName, deriv):
    fileName = fileName + "_derived"
    file = open(fileName, "w")
    file.write("1\n")
    numpy.savetxt(file, deriv, fmt='%1.10E', delimiter= ' ' )
    file.close()
    


    
rootPath = "../../data/"
data_dirs = [rootPath + "2D/"]

for curDir in data_dirs:
    resultFileSelection = curDir + "*.dat"
    for resultFileName in sorted(glob.glob(resultFileSelection)):
        resultFileName = resultFileName.replace("\\", "/")
        writeDerivative(resultFileName, differentiate(resultFileName))


print("End program")
