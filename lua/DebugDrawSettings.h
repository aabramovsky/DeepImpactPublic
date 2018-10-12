#pragma once

#include "LuaScript.h"

class luaDebugDrawSettings
{
  static luaDebugDrawSettings gDebugDrawSettings;

  luaDebugDrawSettings();

public:
  bool dd_draw_waterline;
  bool dd_draw_bottomline;

  bool dd_ship_life;
  bool dd_submarines_life;

  bool dd_anim_draw_framebox;
  bool dd_anim_draw_boundingbox;
  bool dd_anim_draw_regions;

  static luaDebugDrawSettings& instance();

  void Load();
};

#define DbgSettings luaDebugDrawSettings::instance
