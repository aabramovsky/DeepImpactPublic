#include "StdAfx.h"
#include "DebugDrawSettings.h"

luaDebugDrawSettings luaDebugDrawSettings::gDebugDrawSettings;

luaDebugDrawSettings::luaDebugDrawSettings()
{
  memset(this, 0, sizeof(*this));
}

luaDebugDrawSettings& luaDebugDrawSettings::instance()
{
  return gDebugDrawSettings;
}

void luaDebugDrawSettings::Load()
{
  LuaScript settingsScript;
  settingsScript.DoScript( "settings.lua" );

  dd_draw_waterline = settingsScript.GetValue<bool>("dd_draw_waterline", false);
  dd_draw_bottomline = settingsScript.GetValue<bool>("dd_draw_bottomline", false);  

  dd_ship_life = settingsScript.GetValue<bool>("dd_ship_life", false);
  dd_submarines_life = settingsScript.GetValue<bool>("dd_submarines_life", false);  

  dd_anim_draw_framebox = settingsScript.GetValue<bool>("dd_anim_draw_framebox", false);
  dd_anim_draw_boundingbox = settingsScript.GetValue<bool>("dd_anim_draw_boundingbox", false);  
  dd_anim_draw_regions = settingsScript.GetValue<bool>("dd_anim_draw_regions", false);
}
