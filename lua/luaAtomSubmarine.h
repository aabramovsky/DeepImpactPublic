#pragma once
#include "luaObject.h"


namespace Game
{


  class luaAtomSubmarine : public luaObject
  {
  public:
    luaAtomSubmarine(const luaSettings& ls);
    virtual ~luaAtomSubmarine();

  public:
    static void LuaRegister(LuaScript& lua);
  };


}//namespace Game