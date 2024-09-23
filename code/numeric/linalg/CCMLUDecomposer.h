// CCMLUDecomposer.h: Schnittstelle für die Klasse CCMLUDecomposer.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CCMLUDECOMPOSER_H 
#define CCMLUDECOMPOSER_H

#ifndef CCMDATAMANAGER_H
#include "numeric/linalg/CCMDataManager.h"
#endif

class CCMMatrix;
class CCMVector;

class CCMLUDecomposer : public CCMDataManager
{
public:
	static void decompose( CCMMatrix& aM );

private:
	CCMLUDecomposer(){};
	virtual ~CCMLUDecomposer(){};
};

#endif //CCMLUDECOMPOSER_H