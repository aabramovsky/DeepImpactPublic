#pragma once
#include "luaObject.h"


namespace Game
{

  class luaRegularSubmarine : public luaObject
  {
  public:
    luaRegularSubmarine(const luaSettings& ls);
    virtual ~luaRegularSubmarine();

  public:
    static void LuaRegister(LuaScript& lua);
  };

}//namespace Game