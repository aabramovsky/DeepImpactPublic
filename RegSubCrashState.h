#pragma once
#include "ObjectState2.h"


namespace Game
{


class RegSubCrashState : public ObjectState
{
public:
  RegSubCrashState(void);
  virtual ~RegSubCrashState(void);

  virtual bool Init();
  virtual void Loop(float dt);
	virtual bool GetBoundingRect(ge_rectF& rc) { return false; }
private:
  float m_sinkSpeed;
};


}//namespace Game