#include "StdAfx.h"
#include "luaWeaponSlot.h"

namespace Game
{


  luaWeaponSlot::luaWeaponSlot() : m_slotPtr( new WeaponSlot() )
  {
  }

  luaWeaponSlot::luaWeaponSlot( WeaponSlotPtr& slotPtr ): m_slotPtr(slotPtr)
  {
  }

  luaWeaponSlot::~luaWeaponSlot()
  {
  }

  void luaWeaponSlot::SetWeapon( const char* name )
  {
    std::string inName(name);

    if(inName == "normal_bomb")
    {
      m_slotPtr->SetWeapon(WPN_NORMAL_BOMB);
    }
    else if(inName == "antiship_mine")
    {
      m_slotPtr->SetWeapon(WPN_ANTISHIP_MINE);
    }
    else if(inName == "rocket")
    {
      m_slotPtr->SetWeapon(WPN_ROCKET);
    }
  }

  void luaWeaponSlot::SetReloadDelay( float delay )
  {
    m_slotPtr->SetReloadDelay(delay);
  }

  void luaWeaponSlot::SetOffset( float dx, float dy )
  {
    m_slotPtr->SetOffset(dx, dy);
  }

  void luaWeaponSlot::SetSettings( luaSettings& settings )
  {
    m_slotPtr->SetSettings( *settings.activeSettings() );
  }

  void luaWeaponSlot::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
        .beginClass <luaWeaponSlot> ("WeaponSlot")
          .addConstructor <void (*) (void)> ()
          .addFunction ("SetWeapon", &luaWeaponSlot::SetWeapon)
          .addFunction ("SetReloadDelay", &luaWeaponSlot::SetReloadDelay)
          .addFunction ("SetOffset", &luaWeaponSlot::SetOffset)
          .addFunction ("SetSettings", &luaWeaponSlot::SetSettings)
        .endClass ()      
      .endNamespace ();
  }


}//namespace Game
