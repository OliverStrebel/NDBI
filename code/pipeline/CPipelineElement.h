// CPipelineElement.h: Schnittstelle für die Klasse CPipelineElement.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CPIPELINEELEMENT_H)
#define CPIPELINEELEMENT_H

#ifndef CEADSYCONFIG_H
#include "util/CEADSyConfig.h"
#endif

class CPipelineData;

class CPipelineElement
{
public:
	CPipelineElement(CPipelineElement* pPE);
	virtual ~CPipelineElement();

	virtual CPipelineData* next( ) = 0;
	CPipelineElement* getPrevious(){return m_pPrevious;};

protected:
	CEADSyConfig& cfg() { return CEADSyConfig::instance(); };
	CPipelineElement(){ m_pPrevious = 0;};

private:
	CPipelineElement* m_pPrevious;
};

#endif //CPIPELINEELEMENT_H