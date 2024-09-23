//CEADSyException.cpp: Implementierung der Klasse 
//CEADSyException.
//
////////////////////////////////////////////////////////////////////////////////

#include "CEADSyException.h"

#include "CLog.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CEADSyException::CEADSyException(std::string aText)
: std::exception(aText.c_str())
{
	CLog::debugEndl( (std::string( "Exception: " ) + what()).c_str() );
}

CEADSyException::CEADSyException( char* pText)
: std::exception(pText)
{
	CLog::debugEndl( (std::string( "Exception: " ) + what()).c_str() );
}

CEADSyException::~CEADSyException()
{

}

