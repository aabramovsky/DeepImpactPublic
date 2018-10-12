#pragma once
#include "LuaScript.h"
#include "Weapon.h"
#include "lua\luaSettings.h"

namespace Game
{

class GameScene;
class IObjectState;
class AntishipMine;
class IScreenObject;

enum WeaponType
{
  WPN_NORMAL_BOMB,
  WPN_ANTISHIP_MINE,
  WPN_ROCKET
};

class WeaponSlot : public RefCounterImpl
{
public:
  WeaponSlot();
  virtual ~WeaponSlot(void);

  void Init(int weaponType, 
            float reloadDelay,
            float dx,
            float dy,
            GameScene* pScene, 
            IScreenObject* pOwner);

  void SetWeapon(WeaponType weaponType);
  void SetReloadDelay(float delay);
  void SetOffset(float dx, float dy);
  void AttachTo(ObjectState* pState);

  void Loop(float dt);

  void TryReload(float dt);
  
  void CreateWeapon();

  void AlignSlotToShipPos();

  bool Fire();

  void ForceFire();

  Settings& GetSettings();
  void SetSettings(const Settings& s) { m_settings = s; }

public:
  luaSettings luaGetSettings();
  static void LuaRegister(LuaScript& lua);

private:
  GameScene* m_pScene;
  IScreenObject* m_pOwner;

  float m_dx;
  float m_dy;

  float m_reloadDt;
  float m_reloadDelay;

  int m_weaponType;

  WeaponPtr m_weaponPtr;

  Settings m_settings;
};

typedef RefCountedPtrBase<WeaponSlot> WeaponSlotPtr;

}//namespace Game