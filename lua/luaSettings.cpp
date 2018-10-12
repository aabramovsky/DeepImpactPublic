#include "stdafx.h"
#include "luaSettings.h"


namespace Game
{
  luaSettings::luaSettings() : m_pOwned(new Settings()), m_pShared(NULL)
  {
  }

  luaSettings::luaSettings( Settings& s ) : m_pShared(&s)
  {

  }

  luaSettings::luaSettings( const luaSettings& ls )
  {
    m_pOwned = ls.m_pOwned;
    m_pShared = ls.m_pShared;    
  }

  luaSettings& luaSettings::operator = (const luaSettings& ls)
  {
    m_pOwned = ls.m_pOwned;
    m_pShared = ls.m_pShared;

    return *this;
  }

  luaSettings::~luaSettings()
  {
  }

  void luaSettings::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
      .beginClass <luaSettings> ("Settings")
        .addConstructor <void (*) (void)> ()
          .addFunction ("GetF", &luaSettings::GetF)
          .addFunction ("SetF", &luaSettings::SetF)
          .addFunction ("GetI", &luaSettings::GetI)
          .addFunction ("SetI", &luaSettings::SetI)
          .addFunction ("GetS", &luaSettings::GetS)
          .addFunction ("SetS", &luaSettings::SetS)
        .endClass ()      
      .endNamespace ();
  }

} // nanmespace Game