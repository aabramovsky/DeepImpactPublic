#pragma once
#include "ContextObject.h"
#include "RegSubMoveState.h"
#include "RegSubCrashState.h"
#include "RegSubFallState.h"
#include "RegSubBottomState.h"

namespace Game
{


class RegularSubmarine : public ContextObject
{
public:
  RegularSubmarine(GameScene* pScene);
  virtual ~RegularSubmarine(void);

  DEFINE_STANDARD_OPS(RegularSubmarine)

  virtual void Init();
  virtual void SwitchState(int idFrom, int idTo);

private:
  RegSubMoveState m_stMove;
  RegSubCrashState m_stCrash;
  RegSubFallState  m_stFall;
  RegSubBottomState m_stBottom;
};


}//namespace Game