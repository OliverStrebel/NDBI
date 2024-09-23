// CFileWriter.h: Schnittstelle für die Klasse CFileWriter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CFILEWRTITER_H)
#define CFILEWRTITER_H

#ifndef __STRING__
#include <string>
#endif

class CDEData;

class CFileWriter  
{
public:
	CFileWriter();
	virtual ~CFileWriter();

	void writeDEData(CDEData& data, std::string& aFileName);
	void writeDEData(CDEData& data, const char* pchFileName);
};

#endif //CFILEWRTITER_H
