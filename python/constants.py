class Constants:
##############################################################################
# Parametrisations of EADSy
    class OperationMode:
        AS_IS = "AS_IS"
        NOISE_SCAN = "NOISE_SCAN"
        SKIP_SCAN = "SKIP_SCAN"

    class DifferentiationMethod:
        FINITE_DIFF = "FINITE_DIFF"
        TANGENT_REGRESSION = "TANGENT_REGRESSION"
        FRIEDRICHS_MOLLIFIER_WEAK = "FRIEDRICHS_MOLLIFIER_WEAK"
        FRIEDRICHS_MOLLIFIER_NUMERIC = "FRIEDRICHS_MOLLIFIER_NUMERIC"
        SPECTRAL_LOWPASS = "SPECTRAL_LOWPASS"
        DIFFERENTIATION_BY_INTEGRATION = "DIFFERENTIATION_BY_INTEGRATION"
        

##################################################################################################
#Parametrizations used by the scripts
    algos = [DifferentiationMethod.FINITE_DIFF,
             DifferentiationMethod.TANGENT_REGRESSION,
             DifferentiationMethod.FRIEDRICHS_MOLLIFIER_WEAK,
             DifferentiationMethod.FRIEDRICHS_MOLLIFIER_NUMERIC,
             DifferentiationMethod.SPECTRAL_LOWPASS,
             DifferentiationMethod.DIFFERENTIATION_BY_INTEGRATION]
    algo_shortcut =["FD", "TR", "FMW", "FMN", "SL", "NDBI"]
    dirs = ["Hamiltonian4D/","Jerks/", "3D/", "2D/", "Driven3D/"]
    #dirs = ["Hamiltonian4D/"]    

    root_dir = "../data/"
    noise_as_int=10000.0
    time_as_int=10000.0

