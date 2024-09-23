// EADSy.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include <iostream>

#include "util/CLog.h"
#include "util/CEADSyConfig.h"
#include "util/CEADSyException.h"

#include "pipeline/CPipelineOperator.h"


CEADSyConfig& cfg_m(){return CEADSyConfig::instance();}

void startUp(int argc, char* argv[], std::string aFile)
{
    CLog::initialize(aFile.c_str());
    if (argc >= 2) CEADSyConfig::init(argv[1]);
    else CEADSyConfig::init("");
    std::string startMsg = std::string("EADSy startet. \nExecutable: ") + std::string(argv[0]).append("\n");
    startMsg.append("Operation Mode: ").append(cfg_m().getOperationMode().append("\n"));
    startMsg.append("Differentiation Method: ").append(cfg_m().getDifferentiationMethod().append("\n"));
    startMsg.append("Estimation Method: ").append(cfg_m().getEstimationMethod().append("\n"));
    std::cout << startMsg << std::endl;
}

void cleanUp()
{
    CEADSyConfig::close();
    CLog::cleanup();
}

//A memory leak occurs, when instantiated in main
std::string getLogFileName() {return std::string("log/EADSy.log");};

//A memory leak occurs, when instantiated in main
std::string getExceptionMsg()
{
    return std::string(". Try to debug the application. Enable exceptions in the Visual Studio window occuring after pressing ctrl + alt + E.");
}

//A memory leak occurs, when instantiated in main
std::string getDefaultErrMsg()
{
    return std::string("An error occured that could not be caught by std::exception: ") + getExceptionMsg();
}


int main(int argc, char* argv[])
{
    int iReturn = 0;
    try 
    {
        startUp(argc, argv, getLogFileName());
        CPipelineOperator po;
        po.run();
        cleanUp();
    }
    catch (CEADSyException& e) 
    {
        iReturn = 1;
        std::cout << "A CEADSyException occured: " << e.what() << getExceptionMsg() << std::endl; 
    }
    catch (std::exception& e) 
    {
        iReturn = 2;
        std::cout << "A std::exception occured: " << e.what() << getExceptionMsg() << std::endl; 
    }
    catch (...) 
    {
        iReturn = 3;
        std::cout << "An error occured! " << getDefaultErrMsg() << std::endl;
    };

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return iReturn;
}
