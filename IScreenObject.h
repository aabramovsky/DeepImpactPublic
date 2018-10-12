#pragma once
#include "IObject.h"
#include "gepoint.h"
#include "vector2d.h"

namespace Game
{

class GameScene;

enum UnitOrientation
{
  uoLeft = -1,
  uoRight = 1
};

inline UnitOrientation uoReverse(UnitOrientation uo)
{
  return (uo == uoLeft ? uoRight : uoLeft);
}

struct Trajectory
{
  bool bValid;
  ge::point2d ptPos;
  ge::vector2d vSpeed;
};

class IScreenObject : public IObject
{
public:
  IScreenObject(GameScene* pScene);
  virtual ~IScreenObject(void);

  virtual void Init() = 0;
  virtual void Loop(float dt) = 0;
  virtual void Render() = 0;

  Game::UnitOrientation Orientation() const { return m_orientation; }
  void SetOrientation(Game::UnitOrientation val) { m_orientation = val; }

  virtual void GetXY(float& hotX, float& hotY) = 0;
  virtual void MoveTo(float hotX, float hotY) = 0;
  void AlignToObject(IScreenObject* objAlignTo, float dx = 0.f, float dy = 0.f);
  
  virtual void GetTrajectory(Trajectory& tr);

private:
  UnitOrientation m_orientation;
};


}//Game
