// CLog.h: Schnittstelle für die Klasse CLog.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CLOG_H)
#define CLOG_H

#ifndef __VECTOR__
#include <vector>
#endif


class CCMMatrix;
// class CSparseMatrix;
class CCMVector;

class CLog 
{
public:
	static void debug( const char* pMsg, bool bLog = true );
	static void debug( int& value, bool bLog = true );
	static void debug( long long& value, bool bLog = true );
	static void debug( double value, bool bLog = true );
	static void debug( CCMMatrix& value, bool bLog = true );
	//static void debug( CSparseMatrix& value, bool bLog = true );
	static void debug( CCMVector& value, bool bLog = true );
	static void debug( CCMVector& vR, CCMVector& vI, bool bLog = true );
	static void debug( std::vector<double>& value, bool bLog = true );

	static void debugEndl( const char* pMsg, bool bLog = true );
	static void debugEndl( int& value, bool bLog = true );
	static void debugEndl( long long& value, bool bLog = true );
	static void debugEndl( double value, bool bLog = true );

	static void newline();

	static void initialize( const char* pFile );
	static void cleanup();

private:
	CLog(){};
	CLog( const char* pFile );
	~CLog();

	static CLog* m_pI;
};

#endif //CLOG_H