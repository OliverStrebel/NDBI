//CLog.cpp: Implementierung der Klasse CLog.
//
//////////////////////////////////////////////////////////////////////
#include "CLog.h"

#include "../numeric/linalg/CCMMatrix.h"
// #include "LinearAlgebra/base/CSparseMatrix.h"
#include "../numeric/linalg/CCMVector.h"

// #include "LinearAlgebra/util/CCMDataManager.h"

#include <iostream>
#include <fstream>

#include <string>

static std::ofstream s_File;

#pragma warning( disable : 4996 )
 
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CLog* CLog::m_pI = 0;

CLog::CLog( const char* pFile )
{
	s_File.clear();
	s_File.open( pFile );
}

CLog::~CLog()
{
	if ( s_File.is_open() ) s_File.close();
}

void CLog::debug(  const char* pMsg, bool bLog )
{
	if ( !bLog ) return;
	s_File << pMsg;
}

void CLog::debug( int& value, bool bLog )
{
	if ( !bLog ) return;
	s_File << value;
}

void CLog::debug( long long& value, bool bLog )
{
	if ( !bLog ) return;

	char buf[64];
	_i64toa_s( value, buf, 64, 30 );
	std::string sValue( buf );
	s_File << sValue;
}

void CLog::debug( double value, bool bLog )
{
	if ( !bLog ) return;
	int iDigit = 0, iSign = 0;
	std::string s(_ecvt( value, 14, &iDigit, &iSign ));

	s.insert( 1, "," );
	if ( iSign != 0 ) s.insert( 0, "-" );
	else s.append( "0" );
	if ( iDigit >= 1 ) s.append( "0" );
	s.append( "E" );
	if ( iDigit-1 < 0 ) s.append( "-" );
	if ( abs( iDigit-1 ) < 10 ) s.append( "00" );
	else if ( abs( iDigit-1 ) < 100 ) s.append( "0" );

	char szBuf[32];
	_itoa_s( abs(iDigit-1), szBuf, 10 );
	s.append( szBuf );

	s_File << s.c_str();
}

 void CLog::debug( CCMMatrix& M, bool bLog )
 {
	 if ( !bLog ) return;

	 debugEndl( "" );
	 for ( int i = 0; i < M.getRowCount(); i++ )
	 {
		 for ( int j = 0; j < M.getColCount(); j++ )
		 {
			 debug( M.getElement( i, j ) );
			 if ( j < M.getColCount()-1 ) s_File << ' ';
		 }
		 s_File << '\n';
	 }
	 debugEndl( "" );
 }

 void CLog::debug( CCMVector& v, bool bLog )
 {
	 if ( !bLog ) return;

	 debugEndl( "" );
	 for ( int i = 0; i < v.getDimension(); i++ )
	 {
		 debugEndl( v.getElement( i ) );
	 }
	 debugEndl( "" );
 }

 void CLog::debug( CCMVector& vR, CCMVector& vI, bool bLog )
 {
	 if ( !bLog ) return;

	 debugEndl( "" );
	 for ( int i = 0; i < vR.getDimension(); i++ )
	 {
		 debug( vR.getElement( i ) );
		 debug( " " );
		 debugEndl( vI.getElement( i ) );
	 }
	 debugEndl( "" );
 }

 void CLog::debug( std::vector<double>& v, bool bLog )
 {
	 if ( !bLog ) return;

	 debugEndl( "" );
	 for ( int i = 0; i < (int) v.size(); i++ )
	 {
		 debugEndl( v[i] );
	 }
	 debugEndl( "" );
 }

// void CLog::debug( CSparseMatrix& M, bool bLog )
// {
	// if ( !bLog ) return;

	// debugEndl( "" );
	// for ( int i = 0; i < M.getColCount(); i++ )
	// {
		// for ( int j = 0; j < M.getColCount(); j++ )
		// {
			// debug( M.getElement( i, j ) );
			// s_File << ';';
		// }
		// s_File << '\n';
	// }
	// debugEndl( "" );
// }

void CLog::debugEndl(  const char* pMsg, bool bLog )
{
	if ( !bLog ) return;
	debug( pMsg );
	newline();
}

void CLog::debugEndl( int& value, bool bLog )
{
	if ( !bLog ) return;
	debug( value );
	newline();
}

void CLog::debugEndl( long long& value, bool bLog )
{
	if ( !bLog ) return;
	debug( value );
	newline();
}

void CLog::debugEndl( double value, bool bLog )
{
	if ( !bLog ) return;
	debug( value );
	newline();
}

void CLog::newline()
{
	s_File << '\n';
	s_File.flush();
}

void CLog::initialize( const char* pFile )
{
	if ( !m_pI ) m_pI = new CLog( pFile ); 
}

void CLog::cleanup()
{
	if ( m_pI )
	{
		delete m_pI;
		m_pI = 0;
	}
}

