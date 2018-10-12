#pragma once
#include "Settings.h"
#include <memory>


namespace Game
{

class luaSettings
{
  std::tr1::shared_ptr<Settings> m_pOwned;
  Settings* m_pShared;

public:
  luaSettings();
  luaSettings(Settings& s);
  luaSettings(const luaSettings& ls);
  luaSettings& operator = (const luaSettings& ls);
  ~luaSettings();

  //////////////////////////////////////////////////////////////////////////
  // 
  const Settings* activeSettings() const
  {
    if(m_pShared != NULL)
      return m_pShared;

    return m_pOwned.get();
  }

  Settings* activeSettings()
  {
    if(m_pShared != NULL)
      return m_pShared;

    return m_pOwned.get();
  }

  //////////////////////////////////////////////////////////////////////////
  // float settings
  float GetF(const char* name, const float& defVal) const
  {
    return activeSettings()->Get(name, defVal);
  }
  void  SetF(const char* name, const float& val)
  {
    return activeSettings()->Set(name, val);
  }
  void  CopyEntryF(const char* thisName, const char* toName, Settings& copyTo) const
  {
    return activeSettings()->CopyEntry<float>(thisName, toName, copyTo);
  }

  //////////////////////////////////////////////////////////////////////////
  // int settings
  int GetI(const char* name, const int& defVal) const
  {
    return activeSettings()->Get(name, defVal);
  }
  void  SetI(const char* name, const int& val)
  {
    return activeSettings()->Set(name, val);
  }
  void  CopyEntryI(const char* thisName, const char* toName, Settings& copyTo) const
  {
    return activeSettings()->CopyEntry<int>(thisName, toName, copyTo);
  }

  //////////////////////////////////////////////////////////////////////////
  // string settings
  std::string GetS(const char* name, const std::string& defVal) const
  {
    return activeSettings()->Get(name, defVal);
  }
  void  SetS(const char* name, const std::string& val)
  {
    return activeSettings()->Set(name, val);
  }
  void  CopyEntryS(const char* thisName, const char* toName, Settings& copyTo) const
  {
    return activeSettings()->CopyEntry<std::string>(thisName, toName, copyTo);
  }

public:
  static void LuaRegister(LuaScript& lua);
};

} // nanmespace Game