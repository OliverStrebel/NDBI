//CSubDirFileFilter.cpp: Implementierung der Klasse 
//CSubDirFileFilter.
//
////////////////////////////////////////////////////////////////////////////////

#include "CSubDirFileFilter.h"

#include "CLog.h"

#include <filesystem>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


/*
Necessary since the includes

#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

do not work simultaneously in CEADSyConfig.cpp.
*/

using recursiveDirIt = std::filesystem::recursive_directory_iterator;
using entry = std::filesystem::directory_entry;

CSubDirFileFilter::CSubDirFileFilter()
{
}

CSubDirFileFilter::~CSubDirFileFilter()
{

}

std::vector<std::string> CSubDirFileFilter::getFileNames(std::string aPath, std::string anEnding)
{
    std::vector<std::string> fileNames;
    for (const entry& dirEntry : recursiveDirIt(aPath))
    {
        std::string s = dirEntry.path().string();
        if (s.find(anEnding, s.size() - 1 - anEnding.size()) != std::string::npos) fileNames.push_back(s);
    }
    return fileNames;
}

