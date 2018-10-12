#pragma once
#include "ObjectState2.h"



namespace Game
{

class NormalBombOnShipState : public ObjectState
{
public:
  NormalBombOnShipState(void);
  virtual ~NormalBombOnShipState(void);

  void Loop( float dt );
};


}//namespace Game