#pragma once
#include <Box2D/Common/b2Math.h>

#include "LuaScript.h"
#include <map>
#include "Settings.h"
#include "gerect.h"
#include "Gfx/gfx_regions.h"

namespace Game
{

class IObject;
class GameScene;
class ContextObject;

class IObjectState
{
public:
  IObjectState(void)
    :  m_id(-1), m_pParent(NULL)
  {}
  virtual ~IObjectState(void) 
  {}

  virtual bool Init() = 0;
  virtual void Loop(float dt) = 0;
  virtual void Render() = 0;

  ContextObject* Parent() { return m_pParent; }
  const ContextObject* Parent() const { return m_pParent; }
  void SetParent(ContextObject* pParent) { m_pParent = pParent; }

  template<typename T>
  T* Parent() { return dynamic_cast<T*>(m_pParent); }
  
  GameScene* Scene();
  int WaveNumber();

  int Orientation() const;
  void SetOrientation(int orientation);
  void FlipOrientation();

  int stateId() const { return m_id; }
  void setStateId(int id) { m_id = id; }

  int BehaviorId();

  const Settings& GetSettings() const;
  float SGet(const char* name, float defVal);

  virtual void GetXY(float& hotX, float& hotY) {}
  virtual void MoveTo(float hotX, float hotY) {}

  virtual bool GetBoundingRect(ge_rectF& rc) = 0;
  virtual bool GetBoundRegion(gfx_region& region) = 0;

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );

public:
  static void LuaRegister( LuaScript& lua );

private:
  ContextObject* m_pParent;
  int m_id;
};

}//namespace Game