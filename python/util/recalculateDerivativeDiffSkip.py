import numpy

#Parametrization

#comparison
def getAverageDiffPerFile(res: numpy.array, ref: numpy.array, iComp: list):
    diff = 0.0
    for j in range(len(iComp)):
        iTimeRes = 0
        diffComp = 0.0
        for iTimeRef in range(len(ref[:,0])):
            if iTimeRes < len(res[:,0]):
                if (abs(ref[iTimeRef,0] - res[iTimeRes,0]) < 1.0E-10):
                    diffComp = diffComp + abs((ref[iTimeRef,iComp[j]] - res[iTimeRes,iComp[j]])/ref[iTimeRef,iComp[j]])
                    iTimeRes = iTimeRes + 1
        diff = diff + diffComp/(1.0*iTimeRes)
    return diff/(1.0*len(iComp))


N=4000 #max rows
diffMethod="DIFFERENTIATION_BY_INTEGRATION"
scanMethod="SKIP_SCAN"
scan_par_ident = "_06"
files = ["Jerk_1", "Jerk_2", "Jerk_3", "Jerk_4", "Jerk_5", "Jerk_6", "Jerk_7"]
iComp = [1, 2, 3]

relDiff = 0.0
for i in range(len(files)):
    refFileName = "../../data/Jerks/" + files[i] + ".DAT_DERIVED"
    resFileName = "../../data/Jerks/" + scanMethod + "/" + diffMethod + "/DERIV_" + files[i] + scan_par_ident + ".prep_dat"
    print(resFileName)
    ref = numpy.loadtxt(open(refFileName), delimiter=' ', skiprows=1, max_rows=N)
    res = numpy.loadtxt(open(resFileName), delimiter=' ', skiprows=1)
    relDiff = relDiff + getAverageDiffPerFile(res, ref, iComp)
        
print("Average diff: " + str(relDiff/(1.0*len(files))))
print("End program")
