#pragma once
#include "ObjectState2.h"


namespace Game
{

class NormalBombSinkState : public ObjectState
{
public:
  NormalBombSinkState(void);
  virtual ~NormalBombSinkState(void);

  virtual bool Init();
  virtual void Loop(float dt);

  virtual void GetTrajectory(Trajectory& tr);

  bool CheckIfBombHitEnemy(); 

private:
  float m_speed;
};


}//namespace Game