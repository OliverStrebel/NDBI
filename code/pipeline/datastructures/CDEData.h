// CDEData.h: Schnittstelle für die Klasse CDEData.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined (CDEDATA_H)
#define CDEDATA_H

#ifndef __VECTOR__
#include <vector>
#endif

#ifndef CEADSYEXCEPTION_H
#include "util/CEADSyException.h"
#endif


class CDEData
{
public:
    CDEData(int iRowCount, int iColCount, int dependenVariableOffset);
    CDEData(int iRowCount, int dependenVariableOffset, std::vector<double*>* pData);
	virtual ~CDEData();

    int getColCount() {return (int) m_pData->size();};
    int getRowCount() {return m_iRowCount;};
    int getDependendVariableOffset() {return m_iDependendVariableOffset;};
    int getCompCount() {return ((int)m_pData->size() - m_iDependendVariableOffset);};

    double get(int iCol, int iRow)
    {
        if ((iCol < 0) || (iCol >= m_pData->size()) || (iRow < 0) || (iRow >= m_iRowCount))
            throw CEADSyException("CDEData.get: Index out of Bounds!");
        return (*m_pData)[iCol][iRow];
    };
    void set(int iCol, int iRow, double aValue)
    { 
        if ((iCol < 0) || (iCol >= m_pData->size()) || (iRow < 0) || (iRow >= m_iRowCount))
            throw CEADSyException("CDEData.set: Index out of Bounds!");
        (*m_pData)[iCol][iRow] = aValue;
    };

private:
	CDEData(): m_iDependendVariableOffset(0), m_pData(0), m_iRowCount(0) {};
    
    int m_iDependendVariableOffset;
    int m_iRowCount;
    std::vector<double*>* m_pData;
};

#endif //CDEDATA_H