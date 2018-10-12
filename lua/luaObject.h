#pragma once

#include <map>
#include "IScreenObject.h"
#include "luaSettings.h"

namespace Game
{

class GameScene;

class luaObject
{
public:
  luaObject();
  luaObject(IObject* pObject);  
  virtual ~luaObject();

  const char* type();

  int id();
  int TeamId();

  int BehaviorId();
  void SetBehaviorId(int val);

  float GetX();
  float GetY();
  void MoveTo(float hotX, float hotY);

  luaSettings GetSettings();
  void SetSettings(const luaSettings& s);

public:
  static void LuaRegister(LuaScript& lua);

public:
  static GameScene* gpGameScene;

public:
  ObjectPtr m_objectPtr;
};

}//namespace Game