import numpy

class  AggregatedResults:
    def __init__(_
                 , algos: list[str]
                 , x_axis_int: list[int] ):
        _.algos = algos
        _.scan_param = x_axis_int
        _.avg_abs_diff = numpy.zeros((len(algos), len(x_axis_int)))



