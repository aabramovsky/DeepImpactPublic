#pragma once
#include "LuaScript.h"
#include "Weapon.h"
#include "lua\luaSettings.h"
#include "WeaponSlot.h"

namespace Game
{

class luaWeaponSlot
{
public:
  luaWeaponSlot();
  luaWeaponSlot(WeaponSlotPtr& slotPtr);
  ~luaWeaponSlot();

  void SetWeapon(const char* name);
  void SetReloadDelay(float delay);
  void SetOffset(float dx, float dy);
  void SetSettings(luaSettings& settings);

public:
  static void LuaRegister(LuaScript& lua);

public:
  WeaponSlotPtr m_slotPtr;
};

typedef RefCountedPtrBase<WeaponSlot> WeaponSlotPtr;

}//namespace Game