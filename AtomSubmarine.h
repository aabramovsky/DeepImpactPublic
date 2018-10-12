#pragma once
#include "ContextObject.h"
#include "AtomSubMoveState.h"
#include "AtomSubCrashState.h"
#include "AtomSubFallState.h"
#include "AtomSubBottomState.h"


namespace Game
{


class AtomSubmarine : public ContextObject
{
public:
  AtomSubmarine(GameScene* pScene);
  virtual ~AtomSubmarine(void);

  DEFINE_STANDARD_OPS(AtomSubmarine)

  virtual void Init();
  virtual void SwitchState(int idFrom, int idTo);

private:
  AtomSubMoveState m_stMove;
  AtomSubCrashState m_stCrash;
  AtomSubFallState  m_stFall;
  AtomSubBottomState m_stBottom;

};


}//namespace Game