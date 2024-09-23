// CEADSyException.h: Schnittstelle f�r die Klasse CEADSyException.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CEADSYEXCEPTION_H)
#define CEADSYEXCEPTION_H

#include <string>
#include <exception>


class CEADSyException : public std::exception  
{
public:
	CEADSyException(){};
	CEADSyException( std::string aText );
	CEADSyException( char* pText);
	virtual ~CEADSyException();

};

#endif //CEADSYEXCEPTION_H
