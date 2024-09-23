//CPullEnd.cpp: Implementierung der Klasse CPullEnd.
///////////////////////////////////////////////////////////////////////////////
#include "CPullEnd.h"

#include "pipeline/datastructures/CPipelineData.h"


CPullEnd::CPullEnd(CPipelineElement* pPE)
: CPipelineElement(pPE)
{
}

CPullEnd::~CPullEnd()
{
}

CPipelineData* CPullEnd::next()
{
    CPipelineData* pPD = getPrevious()->next();

    return pPD;
}
