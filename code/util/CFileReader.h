// CFileReader.h: Schnittstelle für die Klasse CFileReader.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CFILEREADER_H)
#define CFILEREADER_H

#ifndef __SSTREAM__
#include <sstream>
#endif


class CFileReader  
{
public:
	CFileReader();
	virtual ~CFileReader();

	std::stringstream getFileData(std::string aFileName);
};

#endif //CFILEREADER_H
