#pragma once
#include "Scene.h"

#include "LuaScript.h"
#include "RefCounterImpl.h"
#include <list>
#include "Gfx\gfx_animation.h"
#include "Gfx\gfx_ttf_font.h"
#include "ResourceManager.h"


namespace Game
{

struct BonusControl
{
  std::string m_tip;
  std::string m_tipAndPrice;
  gfx_animation m_anmIcon;
  gfx_ttf_font m_fntTip;

  float m_left;
  float m_top;

  int m_highlightedBars;

  MouseState* m_pMouseState;

  ge_rectI m_rcControl;

  BonusControl();
  ~BonusControl();

  void Init(MouseState* pMouseState, TextureId texId, float scale, const std::string& tip, float left, float top);
  void SetScript(const char* sfile);

  void Loop(float dt);

  void Render();

  bool PtInControl(int x, int y);
  bool IncrementLevel(int& availablePoints);

  int HighlightedBars() const { return m_highlightedBars; }
  void SetHighlightedBars(int bars);

  const int BarsCount() const { return m_numBars; }
  void SetBarsCount(const int val) { m_numBars = val; }

  void SetTipText(const std::string& tip) { m_tip = tip; }

private:
  int m_numBars;
  int m_price;

  LuaScript m_lua;
};

struct ButtonControl
{
  float m_left;
  float m_top;

  MouseState* m_pMouseState;

  std::string m_text;
  gfx_ttf_font m_fnt;
  
  ge_rectI m_rcControl;


  void Init(MouseState* pMouseState, const std::string& text, float left, float top);

  void Loop(float dt);

  void Render();

  bool PtInControl(int x, int y);
};

//////////////////////////////////////////////////////////////////////////
//
//
//

class DockScene : public Scene
{
public:
  DockScene(void);
  virtual ~DockScene(void);

  virtual void LoadResources();
  virtual void SetUp();
  virtual void TearDown();

  virtual void Loop(float dt);
  virtual void Render(); 
  
  virtual void OnMouseMotion(int x, int y);
  virtual void OnMouseButtonDown(MouseButton button, int x, int y);

  void RenderBonusControl(float left, float top, gfx_animation& icon, int numBars, int numHighlightedBars, const std::string& tip);

private:
  gfx_animation m_anmBackground;
  
  BonusControl m_cRepairBonus;
  BonusControl m_cBombSpeedBonus;
  BonusControl m_cShipSpeedBonus;

  ButtonControl m_btnNextWave;

  gfx_ttf_font m_fntBonusPoints;

  int BonusPoints;

  int health_level_base_price;
  int health_level_price_mult;
  int health_next_level_price;

  LuaScript m_lua;
};

} //namespace Game