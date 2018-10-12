#pragma once
#include <string>
#include <wtypes.h>
#include "SDL_rwops.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include <vector>

namespace Str
{
  typedef std::basic_string<TCHAR> tstring;
}

namespace Files
{
  Str::tstring GetExeModulePathT();
  Str::tstring GetRespackPathT();

  char* GetExeModulePath(char* dest, DWORD destSize);
  std::string GetExeModulePath();

  std::string GetRespackPath();

  std::string GetSettingsPath();

  std::string GetScriptsFolderPath();

  std::string GetFontsFolderPath();

  SDL_RWops* GetFontRW(const char* fName);

  SDL_Surface* LoadSurface(const char* name);
  TTF_Font*    LoadFont(const char* name, int ptsize);

  std::string StdPathCombine(const std::string& str1, const std::string& str2);
  std::string StdPathCombine(const std::string& str1, const char* str2);
  std::string StdPathCombine(const char* str1, const char* str2);

  std::string StdGetFullPathName( const std::string& path );

  class FileBuffer
  {
    HANDLE hFile;
    byte* buffer;
    SDL_RWops* rwops;

  public:
    FileBuffer();
    ~FileBuffer();

    bool Load(const std::string& pathname);
    void Clear();

    SDL_RWops* GetRWops() { return rwops; }
    bool bAutoFreeRW;
  };

}
