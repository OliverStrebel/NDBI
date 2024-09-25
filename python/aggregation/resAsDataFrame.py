import pandas
import numpy
import os
import glob

from constants import Constants as C

# laenge der multiplier ist anzahl der Gruppierungsparameter
def readAllFiles(dirs: list, algos: list, mode, mult: list):
    res_dirs = []
    for i in range(len(dirs)):
        for j in range(len(algos)):
            res_dirs.append(C.root_dir + dirs[i] + "/" + mode + "/" + algos[j] + "/")

    df = emptyDF()

    for curDir in res_dirs:
        print("Reading results in dir: " + curDir)
        resultFileSelection = curDir + "DERIV*.PREP_DAT"
        for resultFileName in sorted(glob.glob(resultFileSelection)):
            resultFileName = resultFileName.replace("\\", "/")
            singleDF = _readSingleDF(resultFileName, mult)
            if 'Driven3D' in curDir:
                singleDF = singleDF[singleDF['comp_no'] != 2]
            df = pandas.concat([df, singleDF])
        
    return df


def emptyDF():
    return pandas.DataFrame({'ode': pandas.Series(dtype='string'),
                       'algo': pandas.Series(dtype='string'),
                       'scan_param': pandas.Series(dtype='int'),
                       'comp_no': pandas.Series(dtype='int'),
                       'time': pandas.Series(dtype='int'),
                       'derivative': pandas.Series(dtype='float')})


######################################################################################################
#private helpers for data
def _readSingleDF(fileName, mult: list):
    rawFile = open(fileName)
    raw = pandas.read_csv(fileName, sep=" ", header=None, skiprows=1)
    rawFile.close()
    threeCols = _concatRaw(raw) 
    keys = pandas.DataFrame(_getFieldsFromFileName(fileName, mult))
    df = keys.merge(threeCols, how='cross')
    df.columns = emptyDF().columns
    return df


def _getFieldsFromFileName(fileName, mult: list):
    fileName = fileName.replace("\\", "/")
    myDir, myFile = os.path.split(fileName)
    subdirs = myDir.split("/")
    
    base, ext = os.path.splitext(myFile)
    base = base[6:len(base)]
    iPos = 0; iEnd = len(base); iHyperPar = 0
    for i in range(len(mult)):
        iPos = base.rfind("_", 0, iEnd)
        iHyperPar = iHyperPar + int(float(base[(iPos+1):iEnd])*mult[len(mult)-i-1])
        iEnd = iPos
    return [[base[:iPos], subdirs[len(subdirs)-1], iHyperPar]]


def _concatRaw(raw: pandas.DataFrame):
    resDF = pandas.DataFrame({'comp_no': pandas.Series(dtype='int'),
                       'time': pandas.Series(dtype='int'),
                       'derivative': pandas.Series(dtype='float')})
    raw[raw.columns[0]] = raw[raw.columns[0]]*C.time_as_int
    raw[raw.columns[0]] = raw[raw.columns[0]].astype('int')
    iEnd = len(raw.columns)
    for i in range(1, iEnd):
        compDF = pandas.DataFrame([i-1])
        compDF[compDF.columns[0]] = compDF[compDF.columns[0]].astype('int')
        raw_ = raw[[0,i]]
        res = compDF.merge(raw_, how='cross')
        res.columns = resDF.columns
        resDF = pandas.concat([resDF, res])
    return resDF
