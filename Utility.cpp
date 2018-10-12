#include "stdafx.h"
#include "Utility.h"
#include <winbase.h>
#include <shlwapi.h>
#include <atltrace.h>
#include "LuaScript.h"
#include <string>
#include <wtypes.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "Framework.h"

namespace Files
{

  Str::tstring GetExeModulePathT()
  {
    TCHAR buf[MAX_PATH] = {0};
    GetModuleFileName( NULL, buf, MAX_PATH );
    PathRemoveFileSpec(buf);

    return std::basic_string<TCHAR>(buf);
  }

  

char* GetExeModulePath(char* dest, DWORD destSize)
{
  if (!dest) return NULL;
  if (MAX_PATH > destSize) return NULL;

  DWORD length = GetModuleFileNameA( NULL, dest, destSize );
  PathRemoveFileSpecA(dest);
  return dest;
}

std::string GetExeModulePath()
{
  char buf[MAX_PATH] = {0};
  GetExeModulePath(buf, MAX_PATH);

  return std::string(buf);
}

std::string GetRespackOverridePath()
{
  char buf[MAX_PATH] = {0};
  Files::GetExeModulePath(buf, MAX_PATH);
  PathCombineA(buf, buf, "respack_override.lua");

  return std::string(buf);
}

std::string GetDefaultRespackPath()
{
  return GetExeModulePath();
}

std::string GetRespackPath()
{
  std::string respackPath;

  try
  {    
    std::string spath = Files::GetRespackOverridePath();

    LuaScript overrideScript;
    overrideScript.DoFile(spath.c_str());

    LuaRef v = overrideScript.GetGlobal("g_resourcePath");
    if( !v.isNil() )
    {
      respackPath = v.cast<std::string>();
      respackPath = StdGetFullPathName(respackPath);
    }
  }
  catch (...)
  {
  }

  if( respackPath.empty() )
    respackPath = Files::GetDefaultRespackPath();

  return respackPath;
}

Str::tstring GetRespackPathT()
{
  std::string respackPath;

  try
  {    
    std::string spath = Files::GetRespackOverridePath();

    LuaScript overrideScript;
    overrideScript.DoFile(spath.c_str());

    LuaRef v = overrideScript.GetGlobal("g_resourcePath");
    if( !v.isNil() )
    {
      respackPath = v.cast<std::string>();
      respackPath = StdGetFullPathName(respackPath);
    }
  }
  catch (...)
  {
  }

  if( respackPath.empty() )
    respackPath = Files::GetDefaultRespackPath();

  Str::tstring tresult;
#ifdef UNICODE
  Str::tstring convResult(MAX_PATH, _T('\0'));
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, respackPath.c_str(), respackPath.length(), &convResult.at(0), convResult.capacity());
  convResult.swap(tresult);
#else
  tresult = respackPath;
#endif

  return tresult;
}

std::string GetSettingsPath()
{
  std::string res = GetScriptsFolderPath() + "\\settings.lua";
  return res;
}

std::string GetScriptsFolderPath()
{
  std::string scriptsPath;

  try
  {    
    std::string spath = Files::GetRespackOverridePath();

    LuaScript overrideScript;
    overrideScript.DoFile(spath.c_str());

    LuaRef v = overrideScript.GetGlobal("g_scriptsPath");
    if( !v.isNil() )
    {
      spath = v.cast<std::string>();
      scriptsPath = StdGetFullPathName( spath );
    }
  }
  catch (...)
  {
  }

  if( scriptsPath.empty() )
    scriptsPath = Files::GetExeModulePath() + std::string("\\scripts");

  return scriptsPath;
}

std::string GetFontsFolderPath()
{
  return GetRespackPath() + std::string("\\ttf_fonts");
}

SDL_RWops* GetFontRW( const char* fName )
{
  return NULL;
}

SDL_Surface* LoadSurface( const char* name )
{
  std::string strResPath = Files::StdPathCombine(Files::GetRespackPath(), name);

  FileBuffer fbuf;
  if( !fbuf.Load(strResPath) )
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("failed to create file buffer; path=") << strResPath.c_str());
    return NULL;
  }

  SDL_Surface *surface = IMG_Load_RW(fbuf.GetRWops(), 1);
  if (surface == NULL)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("IMG_Load_RW failed; path=") << strResPath.c_str());
    return NULL;
  }

  return surface;
}

TTF_Font* LoadFont( const char* name, int ptsize )
{
  std::string fontPath = Files::GetFontsFolderPath() + "\\arial.ttf";

  static FileBuffer fbuf;
  if( !fbuf.Load(fontPath) )
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("failed to create file buffer; path=") << fontPath.c_str());
    return NULL;
  }

  return TTF_OpenFontRW(fbuf.GetRWops(), 1, ptsize);
}

std::string StdGetFullPathName( const std::string& path )
{
  if(path[1] == ':')
    return path; //absolute path

  std::string exePath = GetExeModulePath();
  exePath += "\\";
  exePath += path;

  char fullPathBuf[MAX_PATH] = {0};
  char buf[MAX_PATH] = {0};

  GetFullPathNameA(exePath.c_str(), MAX_PATH, fullPathBuf, NULL);

  return fullPathBuf;
}

std::string StdPathCombine( const std::string& str1, const std::string& str2 )
{
  return StdPathCombine(str1.c_str(), str2.c_str());
}

std::string StdPathCombine( const std::string& str1, const char* str2 )
{
  return StdPathCombine(str1.c_str(), str2);
}

std::string StdPathCombine( const char* str1, const char* str2 )
{   
  char buf[MAX_PATH] = {0};
  PathCombineA(buf, str1, str2);

  return std::string(buf);
}


FileBuffer::FileBuffer() : hFile(INVALID_HANDLE_VALUE), rwops(NULL), buffer(NULL)
{
  hFile = INVALID_HANDLE_VALUE;
  bAutoFreeRW = true;
}

FileBuffer::~FileBuffer()
{
   Clear();
}

bool FileBuffer::Load( const std::string& pathname )
{
  Clear();

  Str::tstring tsPath;
#ifdef UNICODE
  Str::tstring convResult(MAX_PATH, _T('\0'));
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pathname.c_str(), pathname.length(), &convResult.at(0), convResult.capacity());
  convResult.swap(tsPath);
#else
  tsPath = pathname;
#endif

  hFile = CreateFile(tsPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile == INVALID_HANDLE_VALUE)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("CreateFile failed; path=") << tsPath.c_str());
    return false;
  }

  const int sBufSize = 1024*1024*5; // 5 megabytes is the maximim 

  DWORD dwSize = GetFileSize(hFile, NULL);
  if(dwSize > sBufSize)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("GetFileSize - buffer too small; path=") << tsPath.c_str() << "; size" << dwSize);
    return false;
  }

  buffer = new byte[dwSize];

  DWORD dwRead = 0;
  if(ReadFile(hFile, buffer, dwSize, &dwRead, NULL) == FALSE)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("ReadFile failed; path=") << tsPath.c_str() << "; size" << dwSize);
    return false;
  }


  rwops = SDL_RWFromConstMem(buffer, dwSize);
  if( rwops == NULL )
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("[LoadSurface] SDL_RWFromMem failed; path=") << tsPath.c_str() << "; size" << dwSize);
    return false;
  }

  return true;
}

void FileBuffer::Clear()
{
  if(hFile != INVALID_HANDLE_VALUE)
  {
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
  }

  if(rwops != NULL)
  {
    if(bAutoFreeRW)
      SDL_FreeRW(rwops);
    rwops = NULL;
  }

  if(buffer != NULL)
  {
    delete[] buffer;
    buffer = NULL;
  }
}

}//Namespace Files