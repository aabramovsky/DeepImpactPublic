#include "StdAfx.h"
#include "Controller.h"
#include "AtomSubmarine.h"
#include "GameScene.h"
#include "NormalBomb.h"

#include <stdlib.h>
#include <math.h>
#include "AtomSubMoveState.h"
#include <atltrace.h>
#include "Framework.h"
#include "ObjectState2.h"
#include "ray2d.h"
#include "gepoint.h"
#include "vector2d.h"

namespace Game
{


Controller::Controller(ContextObject* pSub)
  : m_pSub(pSub)
{
  m_pScene = pSub->Scene();

  m_bRight = true;
  m_bLeft = false;

  m_bUp = false;
  m_bDown = false;
}

Controller::~Controller(void)
{
}

void Controller::Init( ObjectState* pState )
{
  m_pState = pState;

  RouteTask t1 = {rtMove, ge::point2d(m_pScene->ScreenWidth() - 100, m_pScene->GetWaterline() + 100)};
  RouteTask t2 = {rtMove, ge::point2d(100, m_pScene->GetBottomline() - 100)};

  m_route.push_back(t1);
  m_route.push_back(t2);

  m_routeTask = 0;
}

void Controller::Loop( float dt )
{
  if( !DoAvoidBombs() )
  {
    //ATLTRACE("continue route\n");
    ContinueRoute();
  }
  //else
  {
    //ATLTRACE("--avoid bomb\n");
  }
}

bool Controller::DoAvoidBombs()
{
  m_bombAbove = false;
  m_bombInFront = false;

  ObjectsCollection bombs;
  m_pScene->GetObjectsOfType<NormalBomb>(0, bombs);

  bool bTryingAvoid = false;

  ObjectsCollection::iterator it = bombs.begin();
  while(it != bombs.end())
  {
    if( TryAvoidWeapon(it->second) )
      bTryingAvoid = true;

    ++it;
  }

  if(!bTryingAvoid)
    return false;

  ReleaseControls();

  //ATLTRACE("above=%d\n", (int)m_bombAbove);
  //ATLTRACE("infront=%d\n", (int)m_bombInFront);
  

  if(m_bombAbove && m_bombInFront)
  {    
    //ATLTRACE("   **turn\n");
    PressDown();    
    //ContinueRoute();
  }
  else if(m_bombAbove)
  {
    //ATLTRACE("   **sink\n");
    PressDown();
    SpeedUp();
    //ContinueRoute();
  }
  else if(m_bombInFront)
  {
    //ATLTRACE("   **stop\n");
    ReleaseLeftRight();
  }

  return true;
}

bool IsStopped(Trajectory& tr)
{
  return (tr.vSpeed.length() < 0.1);
}

double S(double v0, double t)
{
  return (v0*t);
}

double S(double v0, double a, double t)
{
  return (v0*t + a * t * t / 2.0);
}

double S(double v0, double a, double t, double vMax)
{
  double tmax = (vMax - v0) / a;
  if(t < tmax)
  {
    return S(v0, a, t);
  }
  else
  {
    return ( S(v0, a, tmax) + S(vMax, t - tmax) );
  }
}

ge::vector2d S(const ge::vector2d& v0, const ge::vector2d& a, double t, const ge::vector2d& vMax)
{
  double sx = S(v0.x, a.x, t, vMax.x);
  double sy = S(v0.y, a.y, t, vMax.y);

  return ge::vector2d(sx, sy);
}

bool Controller::TryAvoidWeapon( ObjectPtr& weaponPtr )
{
  Trajectory trW;
  IScreenObject* pObj = weaponPtr.get<IScreenObject>();
  if(pObj == NULL)
    return false;

  pObj->GetTrajectory(trW);
  if(!trW.bValid)
    return false;

  Trajectory trSub;
  m_pSub->GetTrajectory(trSub);
  if(!trSub.bValid)
    return false;

  ge::ray2d rayW(trW.ptPos, trW.vSpeed);
  
  ge::vector2d vSpeedSub(trSub.vSpeed.x, 0);
  if( vSpeedSub.isZeroLength(1.0e-1) )
    vSpeedSub = ge::vector2d((int)m_pSub->Orientation(), 0);

  ge_rectF rcSubSize;
  m_pSub->GetSize(rcSubSize);
  
  ge::point2d ptSub = trSub.ptPos;
  if(vSpeedSub.x > 0)
  {
    ptSub._x -= rcSubSize.w() / 2;
  }
  else
  {
    ptSub._x += rcSubSize.w() / 2;
  }
  ge::ray2d raySub(ptSub, vSpeedSub);

  
  if(0)// debug drawing search rays
  {
    //DEBUG_DRAW(rayW);
    //DEBUG_DRAW(raySub);
  }

  ge::point2d intPnt;
  if( !raySub.intersectWith(rayW, intPnt) )
    return false;

  double distW = intPnt.distanceTo(trW.ptPos);
  double distSub = intPnt.distanceTo(trSub.ptPos);

  
  double tSubSize = rcSubSize.w() / ::fabs(trSub.vSpeed.x);

  double safetyOffset = 30;
  bool bAbove = false;
  if(trW.ptPos.x() > (rcSubSize.left() - safetyOffset) && trW.ptPos.x() < (rcSubSize.right() + safetyOffset)) // probably bomb is above
    bAbove = true;

  double tW = distW / trW.vSpeed.length();

  DynamicTraits dtSub = m_pState->GetDynamicTraits();  

  ge::vector2d vSubS = S(trSub.vSpeed, dtSub.a(), tW, dtSub.vMax());

  double dBombWidthX = 60.0; // TEST TEST

  double subXCenter = trSub.ptPos.x() + vSubS.x; // predicted x position of the submarine after tw secs
  double subX0 = subXCenter - (rcSubSize.w()/2 + dBombWidthX);
  double subX1 = subXCenter + (rcSubSize.w()/2 + dBombWidthX);

  
  double wXCenter = trW.ptPos.x();
  double wX0 = wXCenter - dBombWidthX / 2;
  double wX1 = wXCenter + dBombWidthX / 2;

  ge_rectF rsub(subX0, 0, subX1 - subX0, 1);
  ge_rectF rw(wX0, 0, wX1 - wX0, 1);

  bool bHitProbable = rsub.intersect(rw);

  //static bool bLastP = false;

  //if(bLastP == true && bHitProbable == false)
  //{
  //  int t = 0;
  //}
  //bLastP = bHitProbable;
  
  if( !bHitProbable )
    return false;     

  //ATLTRACE("hit probable subx=%lf\n", subXCenter);
    
  if(bAbove) // probably bomb is above
  {    
    //ATLTRACE("   ++above\n");
    m_bombAbove = true;         
  }
  else
  {
    //ATLTRACE("   ++in front\n");
    m_bombInFront = true;
  }

  return true;
}

void Controller::SpeedUp()
{
  if(m_pState->Orientation() == uoLeft)
  {
    PressLeft();
  }
  else
  {
    PressRight();
  }
}

void Controller::TurnAround()
{
  if(m_pState->Orientation() == uoLeft)
  {
    PressRight();
  }
  else
  {
    PressLeft();
  }
}

bool Controller::Left() const
{
  return m_bLeft;
}

bool Controller::Right() const
{
  return m_bRight;
}

bool Controller::Up() const
{
  return m_bUp;
}

bool Controller::Down() const
{
  return m_bDown;
}

void Controller::PressLeft()
{
  m_bLeft = true;
  m_bRight = false;
}

void Controller::PressRight()
{
  m_bLeft = false;
  m_bRight = true;
}

void Controller::PressUp()
{
  m_bUp = true;
  m_bDown = false;
}

void Controller::PressDown()
{
  m_bUp = false;
  m_bDown = true;
}

void Controller::ReleaseUpDown()
{
  m_bUp = false;
  m_bDown = false;
}

void Controller::ReleaseLeftRight()
{
  m_bLeft = false;
  m_bRight = false;
}

void Controller::ReleaseControls()
{
  ReleaseLeftRight();
  ReleaseUpDown();
}

void Controller::FlipOrientation()
{
  m_pSub->SetOrientation( uoReverse(m_pSub->Orientation()) );
}

void Controller::ContinueRoute()
{
  RouteTask& rt = m_route[m_routeTask];

  Trajectory trSub;
  m_pSub->GetTrajectory(trSub);
  if(!trSub.bValid)
    return;

  const double dTol = 25.0;
  const double dControlsTol = 5.0;

  ge::vector2d vToTarget = rt.ptDestination - trSub.ptPos;
  if(vToTarget.length() < dTol) // task seems to be completed
  {
    cycle_inc_index(m_routeTask, m_route.size());
    return;
  }

  ReleaseControls();

  const DynamicTraits& dt = m_pState->GetDynamicTraits();

  double vx = ::fabs(trSub.vSpeed.x);
  double vy = ::fabs(trSub.vSpeed.y);

  double sx = ::fabs(vToTarget.x);
  double tx = sx / vx;
  double txa = vx / dt.xDecel;

  if(tx > txa && sx > dControlsTol) // time to target is more that time we need to stop
  {
    vToTarget.x > 0 ? PressRight() : PressLeft();      
  }

  double sy = ::fabs(vToTarget.y);
  double ty = sy / vy;
  double tya = vy / dt.yDecel;

  if(ty > tya && sy > dControlsTol)
  {
    vToTarget.y > 0 ? PressDown() : PressUp();
  } 
}

} // namespace game