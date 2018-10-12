#pragma once
#include "LuaScript.h"
#include "luaObject.h"


namespace Game
{

class GameScene;

class luaGameScene
{
public:
  luaGameScene(GameScene* pScene);
  virtual ~luaGameScene();
 
  float GetWaterline();
  float GetBottomline();

  void AddSubmarine(luaObject* pluaSub);

  void ObjectiveCompleted();

public:
  static void LuaRegister(LuaScript& lua);

public:
  GameScene* m_pScene;
};


} //namespace Game