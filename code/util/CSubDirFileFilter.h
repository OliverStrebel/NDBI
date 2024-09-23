// CSubDirFileFilter.h: Schnittstelle für die Klasse CSubDirFileFilter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CSUBDIRFILEFILTER_H)
#define CSUBDIRFILEFILTER_H

#ifndef __STRING__
#include <string>
#endif

#ifndef __VECTOR__
#include <vector>
#endif


class CSubDirFileFilter  
{
public:
	CSubDirFileFilter();
	virtual ~CSubDirFileFilter();

	static std::vector<std::string> getFileNames(std::string aPath, std::string anEnding);
};

#endif //CSUBDIRFILEFILTER_H
