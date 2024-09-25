import pandas

from constants import Constants as C
import aggregation.resAsDataFrame as res
import aggregation.refAsDataFrame as ref
import aggregation.aggregatePandas as agg
from aggregation.AggregatedResults import AggregatedResults
import graphs.averagedeviation as gad


#Note that refDF is not limited to 4000 records
refDF = ref.readAllFiles(C.dirs)


resNoiseDF = res.readAllFiles(C.dirs,
                              C.algos,
                              C.OperationMode.NOISE_SCAN,
                              [C.noise_as_int])
arNoise = agg.get(resNoiseDF, refDF)
gad.scanGraphics(arNoise, 100.0/C.noise_as_int)


resSkipDF = res.readAllFiles(C.dirs,
                            C.algos,
                            C.OperationMode.SKIP_SCAN,
                            [1.0])
arSkip = agg.get(resSkipDF, refDF)
gad.scanGraphics(arSkip, 1.0)


print("End program")

