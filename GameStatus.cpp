#include "StdAfx.h"
#include "GameStatus.h"

namespace Game
{

  Game::ShipUpgradeStatus ShipUpgradeStatus::m_instance;

  ShipUpgradeStatus::ShipUpgradeStatus()
  {
    m_lvlHull = 1;
    m_lvlEngine = 1;
    m_lvlBombSpeed = 1;
    numRegSubsDestroyed = 0;
  }

  ShipUpgradeStatus::~ShipUpgradeStatus()
  {

  }

  Game::ShipUpgradeStatus& ShipUpgradeStatus::instance()
  {
    return m_instance;
  }

  //////////////////////////////////////////////////////////////////////////
  //
  //

  luaShipUpgradeStatus::luaShipUpgradeStatus()
  {

  }

  luaShipUpgradeStatus::~luaShipUpgradeStatus()
  {

  }

  int luaShipUpgradeStatus::lvlHull()
  {
    return ShipUpgradeStatus::instance().m_lvlHull;
  }

  int luaShipUpgradeStatus::lvlEngine()
  {
    return ShipUpgradeStatus::instance().m_lvlEngine;
  }

  int luaShipUpgradeStatus::lvlBombSpeed()
  {
    return ShipUpgradeStatus::instance().m_lvlBombSpeed;
  }

  void luaShipUpgradeStatus::AddBonus( int val )
  {
    ShipUpgradeStatus::instance().totalBonus += val;
  } 

  void luaShipUpgradeStatus::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
         .beginClass <luaShipUpgradeStatus> ("ShipUpgradeStatus")
          .addConstructor <void (*) (void)> ()
          .addFunction ("lvlHull", &luaShipUpgradeStatus::lvlHull)
          .addFunction ("lvlEngine", &luaShipUpgradeStatus::lvlEngine)
          .addFunction ("lvlBombSpeed", &luaShipUpgradeStatus::lvlBombSpeed)
          .addFunction ("AddBonus", &luaShipUpgradeStatus::AddBonus)
        .endClass ()      
      .endNamespace ();
  }

}//Game