import pandas
import numpy
import os
import glob

from constants import Constants as C

# laenge der multiplier ist anzahl der Gruppierungsparameter
def readAllFiles(dirs: list):
    res_dirs = []
    for i in range(len(dirs)):
            res_dirs.append(C.root_dir + dirs[i] + "/")

    df = _emptyDF()

    for curDir in res_dirs:
        print("Reading references in dir: " + curDir)
        resultFileSelection = curDir + "*.DAT_DERIVED"
        for resultFileName in sorted(glob.glob(resultFileSelection)):
            resultFileName = resultFileName.replace("\\", "/")
            sfDF = _readSingleDF(resultFileName)
            df = pandas.concat([df, sfDF])
        
    return df

######################################################################################################
#private helpers for data
def _readSingleDF(fileName):
    rawFile = open(fileName)
    raw = pandas.read_csv(fileName, sep=" ", header=None, skiprows=1)
    rawFile.close()
    
    threeCols = _concatRaw(raw) 
    keys = pandas.DataFrame(_getODEFromFileName(fileName))
    df = keys.merge(threeCols, how='cross')
    df.columns = _emptyDF().columns
    return df


def _emptyDF():
    return pandas.DataFrame({'ode': pandas.Series(dtype='string'),
                       'comp_no': pandas.Series(dtype='int'),
                       'time': pandas.Series(dtype='int'),
                       'derivative': pandas.Series(dtype='float')})


def _getODEFromFileName(fileName):
    fileName = fileName.replace("\\", "/")
    myDir, myFile = os.path.split(fileName)
    base, ext = os.path.splitext(myFile)
    return [base]


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
