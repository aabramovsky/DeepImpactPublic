#pragma once
#include "LuaScript.h"
#include "gerect.h"

namespace Game
{

  
  class luaRect2d
  {
  public:
    luaRect2d();
    luaRect2d(const luaRect2d& rc);
    luaRect2d(const ge_rectF& rcF);
    luaRect2d& operator=(const luaRect2d& rc);
    ~luaRect2d();

    float Width();
    float Height();

  public:
    static void LuaRegister( LuaScript& lua );

  private:
    ge_rectF m_rect;
  };


}//namespace Game