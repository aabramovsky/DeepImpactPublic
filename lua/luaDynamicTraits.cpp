#include "stdafx.h"
#include "luaDynamicTraits.h"

namespace Game
{

  luaDynamicTraits::luaDynamicTraits()
  {}

  luaDynamicTraits::luaDynamicTraits( const luaDynamicTraits& traits )
  {
    m_traits = traits.m_traits;
  }

  luaDynamicTraits& luaDynamicTraits::operator=( const luaDynamicTraits& traits )
  {
    m_traits = traits.m_traits;
    return *this;
  }

  luaDynamicTraits::~luaDynamicTraits()
  {}

  void luaDynamicTraits::SetSpeedMax( float sx, float sy )
  {
    m_traits.xSpeedMax = sx;
    m_traits.ySpeedMax = sy;
  }

  void luaDynamicTraits::SetAcceleration( float ax, float ay )
  {
    m_traits.xAccel = ax;
    m_traits.yAccel = ay;
  }

  void luaDynamicTraits::SetDeceleration( float dx, float dy )
  {
    m_traits.xDecel = dx;
    m_traits.yDecel = dy;
  }

  void luaDynamicTraits::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
        .beginClass <luaDynamicTraits> ("DynamicTraits")
        .addConstructor <void (*) (void)> ()
          .addFunction ("SetSpeedMax", &luaDynamicTraits::SetSpeedMax)
          .addFunction ("SetAcceleration", &luaDynamicTraits::SetAcceleration)
          .addFunction ("SetDeceleration", &luaDynamicTraits::SetDeceleration)          
        .endClass ()      
      .endNamespace ();
  }


}//namespace Game