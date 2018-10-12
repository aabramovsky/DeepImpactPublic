#pragma once
#include "LuaScript.h"


namespace Game
{

class ShipUpgradeStatus
{
private:
  ShipUpgradeStatus();
  ~ShipUpgradeStatus();

public:
  static ShipUpgradeStatus& instance();

  int m_lvlHull;
  int m_lvlEngine;
  int m_lvlBombSpeed;

  // test statistics
  int numRegSubsDestroyed;
  int totalBonus;
private:
  static ShipUpgradeStatus m_instance; 
};

class luaShipUpgradeStatus
{
public:
  luaShipUpgradeStatus();
  virtual ~luaShipUpgradeStatus();

  int lvlHull();
  int lvlEngine();
  int lvlBombSpeed();
  
  void AddBonus(int val);

public:
  static void LuaRegister(LuaScript& lua);
};

}//Game