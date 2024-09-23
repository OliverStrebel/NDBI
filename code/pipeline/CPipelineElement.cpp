//CPipelineElement.cpp: Implementierung der Klasse CPipelineElement.
///////////////////////////////////////////////////////////////////////////////
#include "CPipelineElement.h"

#include "util/CEADSyException.h"
#include "util/CLog.h"


CPipelineElement::CPipelineElement(CPipelineElement* pPE)
: m_pPrevious(pPE)
{
}

CPipelineElement::~CPipelineElement()
{
    if (m_pPrevious != 0)
    {
        delete m_pPrevious;
        m_pPrevious = 0;
    }
}
