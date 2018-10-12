#pragma once

#include "LuaScript.h"
#include "ObjectState2.h"

namespace Game
{


  class luaDynamicTraits
  {
  public:
    luaDynamicTraits();
    luaDynamicTraits(const luaDynamicTraits& traits);
    luaDynamicTraits& operator = (const luaDynamicTraits& traits);
    ~luaDynamicTraits();

    void SetSpeedMax(float sx, float sy);
    void SetAcceleration(float ax, float ay);
    void SetDeceleration(float dx, float dy);
  
  public:
    static void LuaRegister(LuaScript& lua);

  public:
    DynamicTraits m_traits;
  };


}//namespace Game