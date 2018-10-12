#pragma once

#include <map>
#include "IScreenObject.h"

namespace Game
{

class ObjectState;

class ContextObject : public IScreenObject
{
public:
  ContextObject(GameScene* pScene);
  virtual ~ContextObject(void);

  virtual void Loop(float dt);
  virtual void Render();

  void AddState(int id, ObjectState* pState);
  virtual void SwitchState(int idFrom, int idTo) {};

  ObjectState* currentState() { return m_pCurrentState; }
  const ObjectState* currentState() const { return m_pCurrentState; }
  void setCurrentState(ObjectState* pState) { m_pCurrentState = pState; }

  virtual void GetXY(float& hotX, float& hotY);
  virtual void MoveTo(float hotX, float hotY);
  virtual void GetTrajectory(Trajectory& tr);

  virtual bool GetSize(ge_rectF& rcSize);
  virtual bool GetBoundingRect(ge_rectF& rc);
  virtual bool GetBoundRegion(gfx_region& region);

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );

public:
  static void LuaRegister(LuaScript& lua);

private:
  std::map<int, ObjectState*> m_states;
  ObjectState* m_pCurrentState;
};


}//namespace Game