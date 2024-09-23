// CPullEnd.h: Schnittstelle für die Klasse CPullEnd.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CPULLEND_H)
#define CPULLEND_H

#ifndef CPIPELINEELEMENT_H
#include "pipeline/CPipelineElement.h"
#endif

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif

class CPipelineData;

class CPullEnd : public CPipelineElement
{
public:
	CPullEnd(CPipelineElement* pPE);
	virtual ~CPullEnd();

	virtual CPipelineData* next();
};

#endif //CPULLEND_H