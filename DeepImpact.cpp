// DeepImpact.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DeepImpact.h"
#include "Framework.h"
#include "detail\LuaException.h"
#include <fstream>

#include "Utility.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
  ::log4cplus::initialize(); 
  ::log4cplus::tstring iniFile = Files::GetExeModulePathT() + L"\\log4cplus_configure.ini";
  ::log4cplus::PropertyConfigurator::doConfigure( iniFile );

  logger = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("main"));
  logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);

  try
  {
    LOG4CPLUS_TRACE(logger, "Before framework.run");
    framework().Run();
  }
  catch (luabridge::LuaException& e)
  {
    LOG4CPLUS_ERROR(logger, e.m_what.c_str());    
  }
  catch (...)
  {   
    LOG4CPLUS_ERROR(logger, "unexpected exception erminated program"); 
  }
  
 
  return 0;
}