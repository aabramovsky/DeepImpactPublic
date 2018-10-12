#pragma once


extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge.h>

using namespace luabridge;


class LuaScript
{
public:
  LuaScript(void);
  ~LuaScript(void);

  void Init();
  void Cleanup();

  void DoScript(const char* scriptName);
  void DoFile(const char* fullPathname);

  lua_State* State() { return m_pLua; }

  LuaRef GetGlobal(const char* name);

  template<typename T>
  T GetValue(const char* name)
  {
    LuaRef v = GetGlobal(name);
    if( !v.isNil() )
      return v.cast<T>();

    return T();
  }

  template<typename T>
  T GetValue(const char* name, const T& defVal)
  {
    LuaRef v = GetGlobal(name);
    if( !v.isNil() )
      return v.cast<T>();

    return defVal;
  }

  LuaRef Call(const char* fname)
  {
    LuaRef func = getGlobal(m_pLua, fname);
    return func();
  }

  template<typename P1>
  LuaRef Call(const char* fname, P1 p1)
  {
    LuaRef func = getGlobal(m_pLua, fname);
    return func(p1);
  }

  template<typename P1, typename P2>
  LuaRef Call(const char* fname, const P1& p1, const P2& p2)
  {
    LuaRef func = getGlobal(m_pLua, fname);
    return func(p1, p2);
  }

  template<typename P1, typename P2, typename P3>
  LuaRef Call(const char* fname, const P1& p1, const P2& p2, const P3& p3)
  {
    LuaRef func = getGlobal(m_pLua, fname);
    return func(p1, p2, p3);
  }

  void RegisterConstant(int value, char* constantname)
  {
    lua_pushinteger(m_pLua, value);
    lua_setglobal(m_pLua,constantname);
  }

  void RegisterConstant(double value, char* constantname)
  {
    lua_pushnumber(m_pLua, value);
    lua_setglobal(m_pLua,constantname);
  }

  void RegisterConstant(char* value, char* constantname)
  {
    lua_pushstring(m_pLua, value);
    lua_setglobal(m_pLua,constantname);
  }


  void RegisterConstant(bool value, char* constantname)
  {
    lua_pushboolean(m_pLua, value);
    lua_setglobal(m_pLua,constantname);
  }

  void RegisterConstant(lua_CFunction value, char* constantname)
  {
    lua_pushcfunction(m_pLua, value);
    lua_setglobal(m_pLua,constantname);
  }

private:
  void LuaThrow();
  void LuaNewState();

private:
  lua_State* m_pLua;
};


#define LUA_CALL0(name) m_lua.GetGlobal(name)()
#define LUA_CALL(name, params) m_lua.GetGlobal(name)(params)

void GetScriptPath(const char* scriptName, char* dest, DWORD destSize);
void GetLogPath(const char* logName, char* dest, DWORD destSize);
