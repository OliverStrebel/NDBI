0. Instrument EADSy in method CSpectralLowpass::getFilterParameter after eps and dist are calculated with
	if (iComp == 1)
	{
		CLog::debug(eps[eps.size() - 1]);
		CLog::debug(" ");
		CLog::debugEndl(dist[dist.size() - 1]);
	}
 
1. Run EADSy with the Configuration.cfg in this directory

2. Put the output in EADSy.log into data_distance_vs_eps.txt in this directory 

3. Replace , with . as decimal separator

3. Run plotEps.py in this directory
