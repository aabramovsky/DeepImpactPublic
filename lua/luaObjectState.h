#pragma once
#include "LuaScript.h"
#include "luaWeaponSlot.h"
#include "luaGameScene.h"
#include "luaDynamicTraits.h"
#include "luaRect2d.h"

namespace Game
{
  
  
  class luaObjectState
  {
  public:
    luaObjectState(ObjectState* pState);
    virtual ~luaObjectState();

    void LoadResources();

    void AddWeaponSlot(int slotId, luaWeaponSlot& slot);
    luaGameScene Scene();

    void SetDynamicTraits(luaDynamicTraits& traits);
    void SetHealthStatus(float health, float maxHealth);

    luaSettings GetObjectSettings();
    void SetObjectSettings(const luaSettings& s);

    void MoveTo(float hotX, float hotY);

    luaRect2d GetSize();

  public:
    static void LuaRegister( LuaScript& lua );

  private:
    ObjectState* m_pState;
  };


}//namespace Game