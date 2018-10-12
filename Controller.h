#pragma once
#include "RefCounterImpl.h"
#include "RefCountedPtrBase.h"
#include "IObject.h"
#include <vector>
#include "gepoint.h"

namespace Game
{

class ContextObject;
class GameScene;
class AtomSubMoveState;
class ObjectState;

enum RouteTaskType
{
  rtMove,
};

struct RouteTask 
{
  RouteTaskType type;
  ge::point2d ptDestination;
};

typedef std::vector<RouteTask> Route;

class Controller
  : public RefCounterImpl
{
public:
  Controller(ContextObject* pSub);
  ~Controller(void);

  void Init(ObjectState* pState);

  void Loop(float dt);

  bool Left() const;
  bool Right() const;
  bool Up() const;
  bool Down() const;
  

  void PressLeft();
  void PressRight();
  void PressUp();
  void PressDown();
  void ReleaseUpDown();
  void ReleaseLeftRight();
  void ReleaseControls();
 
  void SpeedUp();  
  void TurnAround();
  void FlipOrientation();

  bool DoAvoidBombs();
  bool TryAvoidWeapon( ObjectPtr& weaponPtr );
  void ContinueRoute();
private:
  ContextObject* m_pSub;
  ObjectState* m_pState;

  GameScene* m_pScene;

  bool m_bLeft;
  bool m_bRight;

  bool m_bUp;
  bool m_bDown;

  Route m_route;
  int m_routeTask;

  bool m_bombInFront;
  bool m_bombAbove;
};


typedef RefCountedPtrBase<Controller> ControllerPtr;

} // namespace game