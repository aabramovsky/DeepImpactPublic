#include "StdAfx.h"
#include "LuaScript.h"
#include "Framework.h"
#include "IObject.h"
#include "ObjectState2.h"
#include "RegSubMoveState.h"
#include "WeaponSlot.h"
#include "ShipSailState.h"
#include "GameScene.h"
#include "AntishipMineEmersionState.h"
#include "RocketFlyState.h"
#include "Scene.h"
#include "Utility.h"
#include "lua\luaObject.h"
#include "lua\luaAtomSubmarine.h"
#include "lua\luaRegularSubmarine.h"
#include "lua\luaObjectState.h"
#include "lua\luaGameScene.h"
#include "lua\luaDynamicTraits.h"
#include "GameStatus.h"

LuaScript::LuaScript(void)
{
  LuaNewState();
}

LuaScript::~LuaScript(void)
{
  Cleanup();
}

void LuaScript::Init()
{
  Cleanup();

  LuaNewState();

  luaL_openlibs(m_pLua);

  RegisterConstant(framework().Window_Width(), "screenWidth");
  RegisterConstant(framework().Window_Height(), "screenHeight");

  Game::IObject::LuaRegister(*this);
  Game::ContextObject::LuaRegister(*this);

  Game::IObjectState::LuaRegister(*this);
  Game::ObjectState::LuaRegister(*this);
  
  Game::WeaponSlot::LuaRegister(*this);

  Game::RegSubMoveState::LuaRegister(*this);
  Game::AtomSubMoveState::LuaRegister(*this);

  Game::ShipSailState::LuaRegister(*this);

  Game::RocketFlyState::LuaRegister(*this);

  Game::AntishipMineEmersionState::LuaRegister(*this);

  //////////////////////////////////////////////////////////////////////////
  //
  Game::luaSettings::LuaRegister(*this);

  Game::luaObject::LuaRegister(*this);
  Game::luaAtomSubmarine::LuaRegister(*this);
  Game::luaRegularSubmarine::LuaRegister(*this);
  Game::luaObjectState::LuaRegister(*this);
  Game::luaWeaponSlot::LuaRegister(*this);
  Game::luaGameScene::LuaRegister(*this);
  Game::luaDynamicTraits::LuaRegister(*this);
  Game::luaRect2d::LuaRegister(*this);
  Game::luaShipUpgradeStatus::LuaRegister(*this);

  DoScript("common_lib.lua");
}

void LuaScript::Cleanup()
{
  if(m_pLua != NULL)
    lua_close(m_pLua);

  m_pLua = NULL;
}

void LuaScript::DoScript( const char* scriptName )
{
  std::string fullPath = Files::GetScriptsFolderPath();
  fullPath += "\\";
  fullPath += std::string(scriptName);

  DoFile(fullPath.c_str());
}

void LuaScript::DoFile(const char* fullPath)
{
  if( luaL_dofile(m_pLua, fullPath) )
  {
    LuaThrow();
  }
}

LuaRef LuaScript::GetGlobal( const char* name )
{
  return getGlobal(m_pLua, name);
}

void LuaScript::LuaThrow()
{
  const char* err = lua_tostring(m_pLua, -1);
  luabridge::LuaException l(m_pLua, 0);
  throw l;
}

void LuaScript::LuaNewState()
{
  m_pLua = luaL_newstate();
  if(m_pLua == NULL)
    throw std::exception("cannot create lua state");
}

void GetScriptPath(const char* scriptName, char* dest, DWORD destSize)
{
  Files::GetExeModulePath(dest, destSize);
  strcat_s(dest, destSize, "\\scripts\\");
  strcat_s(dest, destSize, scriptName);
}

void GetLogPath( const char* logName, char* dest, DWORD destSize )
{
  Files::GetExeModulePath(dest, destSize);
  strcat_s(dest, destSize, "\\");
  strcat_s(dest, destSize, logName);
}
