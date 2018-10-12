#include "StdAfx.h"
#include "luaRect2d.h"

namespace Game
{

  luaRect2d::luaRect2d()
  {
  }

  luaRect2d::luaRect2d( const luaRect2d& rc ) : m_rect(rc.m_rect)
  {
  }

  luaRect2d::luaRect2d( const ge_rectF& rcF ) : m_rect(rcF)
  {    
  }

  luaRect2d& luaRect2d::operator=( const luaRect2d& rc )
  {
    m_rect = rc.m_rect;
    return *this;
  }

  luaRect2d::~luaRect2d()
  {
  }

  float luaRect2d::Width()
  {
    return m_rect.w();
  }

  float luaRect2d::Height()
  {
    return m_rect.h();
  }

  void luaRect2d::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
        .beginClass <luaRect2d> ("Rect2d")
          .addConstructor <void (*) (void)> ()
          .addFunction ("Width", &luaRect2d::Width)
          .addFunction ("Height", &luaRect2d::Height)
        .endClass ()      
      .endNamespace ();
  }


}//namespace Game