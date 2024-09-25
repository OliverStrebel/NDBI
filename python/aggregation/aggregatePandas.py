import pandas
import numpy

from aggregation.AggregatedResults import AggregatedResults
import aggregation.resAsDataFrame as res

def get(resDF, refDF):
    resDFEqualTimes = _ensureEqualPointsInTimeForAllAlgorithms(resDF)
    return _aggregate(resDFEqualTimes, refDF)
    

#####################################################################################################################################
#private helpers
# pro ode, comp, scanpar
# ÜBER ALLE ALGOS GEMEINSAME zEITPUNKTE BESTIMMEN
def _ensureEqualPointsInTimeForAllAlgorithms(resDF):
    print("Removing points in time that are not estimated by all algos")
    df = res.emptyDF()
    odes = resDF['ode'].drop_duplicates()
    scanPars = resDF['scan_param'].drop_duplicates()
    for ode in odes:
        print("    " + str(ode))
        for scanPar in scanPars:
            resDFOde = resDF[(resDF['ode'] == ode) & (resDF['scan_param'] == scanPar)]
            tMin, tMax = _getTimeMinMax(resDFOde)
            resT = resDFOde[(resDFOde['time'] >= tMin) & (resDFOde['time'] <= tMax)]
            df = pandas.concat([df, resT])
    return df


def _getTimeMinMax(resDFOde):
    algos = list(resDFOde['algo'].drop_duplicates())
    timePoints = pandas.DataFrame(resDFOde[resDFOde['algo'] == algos[0]]['time'].drop_duplicates())
    for i in range(1, len(algos)):
        timePt = pandas.DataFrame(resDFOde[resDFOde['algo'] == algos[i]]['time'].drop_duplicates())
        timePoints = timePoints.merge(timePt, how = 'inner')
    return timePoints['time'].min(), timePoints['time'].max()    


def _aggregate(resDF, refDF):
    print("Aggregating")
    ar = AggregatedResults(resDF['algo'].unique(), resDF['scan_param'].unique()) 
    for jScanPar in range(len(ar.scan_param)):
        print('.', end=' ')
        for kAlgo in range(len(ar.algos)):
            resDFComp = resDF[(resDF["scan_param"] == ar.scan_param[jScanPar]) & (resDF["algo"] == ar.algos[kAlgo])]
            comparisonDF = pandas.merge(resDFComp, refDF, on=['ode', 'comp_no', 'time'], suffixes=('_res','_ref'))
            comparisonDF['rel_diff'] = (comparisonDF['derivative_res'] - comparisonDF['derivative_ref']).abs().div(comparisonDF['derivative_ref'].abs())
            #if ar.scan_param[jScanPar] == 18: _investigate(comparisonDF)
            ar.avg_abs_diff[kAlgo, jScanPar] = comparisonDF['rel_diff'].mean()
    print('')
    return ar


def _investigate(compDF):
    maxVal = compDF['rel_diff'].max()
    print(maxVal)
    if maxVal > 165000.0:
        maxRec = compDF[compDF["rel_diff"] > 44800.0]
        print(maxRec['algo'])
        print(maxRec['ode'])
        print(maxRec['scan_param'])
        print(maxRec['comp_no'])
        print(maxRec['time'])
        print(maxRec['derivative_ref'])
        print(maxRec['derivative_res'])
        print(maxRec['rel_diff'])
