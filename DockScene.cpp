#include "StdAfx.h"
#include "DockScene.h"
#include "Framework.h"
#include "GameStatus.h"
#include <iosfwd>


namespace Game
{

DockScene::DockScene(void)
{}

DockScene::~DockScene(void)
{}

void DockScene::LoadResources()
{
  m_lua.DoScript( "dock.lua" );

  resources().MakeAnimation(texDockSceneBg, 1.0f, m_anmBackground);

  m_cRepairBonus.Init(&g_mouseState, texBonusRecovery, 1.0f, "Maximum health", 100, 100);
  m_cBombSpeedBonus.Init(&g_mouseState, texBonusBombSpeed, 1.0f, "Bomb speed", 100, 200);
  m_cShipSpeedBonus.Init(&g_mouseState, texBonusShipSpeed, 1.0f, "Engine power", 100, 300);
  m_btnNextWave.Init(&g_mouseState, "Next wave!", 750, 800);

  m_cRepairBonus.SetScript("dock\\health.lua");
  m_cBombSpeedBonus.SetScript("dock\\bomb.lua");
  m_cShipSpeedBonus.SetScript("dock\\engine.lua");
}

void DockScene::SetUp()
{
  BonusPoints = ShipUpgradeStatus::instance().totalBonus;

  m_screenWidth = framework().Window_Width();
  m_screenHeight = framework().Window_Height();
  
  m_cRepairBonus.SetHighlightedBars( ShipUpgradeStatus::instance().m_lvlHull );
  m_cBombSpeedBonus.SetHighlightedBars( ShipUpgradeStatus::instance().m_lvlBombSpeed );
  m_cShipSpeedBonus.SetHighlightedBars( ShipUpgradeStatus::instance().m_lvlEngine );
  
}

void DockScene::TearDown()
{
  ShipUpgradeStatus::instance().m_lvlHull = m_cRepairBonus.HighlightedBars();
  ShipUpgradeStatus::instance().m_lvlBombSpeed = m_cBombSpeedBonus.HighlightedBars();
  ShipUpgradeStatus::instance().m_lvlEngine = m_cShipSpeedBonus.HighlightedBars();
}

void DockScene::Loop( float dt )
{
  m_anmBackground.update(dt);

  m_cRepairBonus.Loop(dt);
  m_cBombSpeedBonus.Loop(dt);
  m_cShipSpeedBonus.Loop(dt);

  m_btnNextWave.Loop(dt);
}

void DockScene::Render()
{    
  m_anmBackground.render(0.f,0.f, m_screenWidth, m_screenHeight);

  m_cRepairBonus.Render();
  m_cBombSpeedBonus.Render();
  m_cShipSpeedBonus.Render();

  m_btnNextWave.Render();

  std::stringstream strStream;
  strStream << "Bonus points available: " << BonusPoints;

  SDL_Color clr = {0,255,0,0};
  m_fntBonusPoints.draw_solid(100, 50, gfx_ttf_font::hLeft, gfx_ttf_font::vTop, strStream.str(), 25, clr);
}

void DockScene::OnMouseMotion( int x, int y )
{

}

void DockScene::OnMouseButtonDown( MouseButton button, int x, int y )
{
  if(button != mbLeft)
    return;

  if(m_cRepairBonus.PtInControl(x, y))
  {
    m_cRepairBonus.IncrementLevel(BonusPoints);
  }

  if(m_cBombSpeedBonus.PtInControl(x, y))
  {
    m_cBombSpeedBonus.IncrementLevel(BonusPoints);
  }

  if(m_cShipSpeedBonus.PtInControl(x, y))
  {
    m_cShipSpeedBonus.IncrementLevel(BonusPoints);
  }

  if(m_btnNextWave.PtInControl(x, y))
  {
    ShipUpgradeStatus::instance().totalBonus = BonusPoints;
    SwitchToScene(ScnGame);
  }
}

//////////////////////////////////////////////////////////////////////////
//
//
//

BonusControl::BonusControl() : m_numBars(10), m_highlightedBars(0)
{

}

BonusControl::~BonusControl()
{

}

void BonusControl::Init( MouseState* pMouseState, TextureId texId, float scale, const std::string& tip, float left, float top )
{
  m_pMouseState = pMouseState;

  m_left = left;
  m_top = top;
  m_tip = tip;

  resources().MakeAnimation(texId, scale, m_anmIcon);
}

void BonusControl::SetScript( const char* sfile )
{
  m_lua.Init();
  m_lua.DoScript(sfile);

  SetBarsCount( m_lua.GetValue<int>("level_max", 10) );
}

void BonusControl::Loop( float dt )
{
  m_anmIcon.update(dt);
}

void BonusControl::Render()
{
  const int tipOffsetH = 5;

  const int fontSize = 21;
  int tipW = 0, tipH = 0;
  m_fntTip.size_string(m_tipAndPrice, fontSize, &tipW, &tipH);

  SDL_Color clr = {0,255,0,0};
  m_fntTip.draw_solid(m_left, m_top, gfx_ttf_font::hLeft, gfx_ttf_font::vTop, m_tipAndPrice, fontSize, clr);

  ge_rectF rcIconBox = m_anmIcon.bounding_box(m_left, m_top);

  float iconCX = m_left + rcIconBox.w() / 2.0f;
  float iconCY = m_top + tipH + tipOffsetH + rcIconBox.h() / 2.0f;

  m_anmIcon.render(iconCX, iconCY);

  rcIconBox = m_anmIcon.bounding_box(iconCX, iconCY);

  const float barW = 10.0f;
  const float barH = rcIconBox.h();

  float iconW = rcIconBox.w();

  const float barSpacing = 10.0f;
  const float barOffset = 20.0f;

  float xb = m_left + iconW + barOffset;
  float yb = rcIconBox.top();

  SDL_Rect rcBar = {xb, yb, barW, barH };

  SDL_SetRenderDrawBlendMode(sdl_renderer(), SDL_BLENDMODE_BLEND);
  for(int i = 0; i < BarsCount(); i++)
  {
    if(i < m_highlightedBars )
    {
      SDL_SetRenderDrawColor(sdl_renderer(), 82, 240, 60, 200);
    }
    else
    {
      SDL_SetRenderDrawColor(sdl_renderer(), 89, 179, 217, 70);
    }

    SDL_RenderFillRect(sdl_renderer(), &rcBar);
    rcBar.x += (barW + barSpacing);
  }

  m_rcControl = ge_rectI(m_left, m_top, rcBar.x - m_left - barSpacing, rcIconBox.h() + tipOffsetH + tipH);
  m_rcControl.offset_rect(5,3);

  SDL_SetRenderDrawColor(sdl_renderer(), 0, 200, 0, 255);
  if( m_rcControl.point_in_rect(m_pMouseState->m_lastX, m_pMouseState->m_lastY) )
  {
    SDL_Rect rc = {m_rcControl.left(), m_rcControl.top(), m_rcControl.w(), m_rcControl.h() };
    SDL_RenderDrawRect(sdl_renderer(), &rc);
  }
}

bool BonusControl::PtInControl( int x, int y )
{
  return m_rcControl.point_in_rect(x, y);
}

bool BonusControl::IncrementLevel( int& availablePoints )
{
  if( m_highlightedBars >= m_numBars)
    return false;

  if( m_price > availablePoints )
    return false;

  availablePoints -= m_price;
  SetHighlightedBars( m_highlightedBars+1 );
  return true;
}

void BonusControl::SetHighlightedBars( int bars )
{
  m_highlightedBars = bars;

  LuaRef r = m_lua.Call("next_level_price", m_highlightedBars, m_numBars);
  m_price = r.cast<int>();

  std::stringstream str;
  str << m_tip << "; Needed for the next level:" << m_price;
  m_tipAndPrice = str.str();
}

void ButtonControl::Init( MouseState* pMouseState, const std::string& text, float left, float top )
{
  m_pMouseState = pMouseState;

  m_left = left;
  m_top = top;
  m_text = text;
}

void ButtonControl::Loop( float dt )
{

}

void ButtonControl::Render()
{
  const int fontSize = 40;
  int tipW = 0, tipH = 0;
  m_fnt.size_string(m_text, fontSize, &tipW, &tipH);

  m_rcControl = ge_rectI(m_left, m_top, tipW, tipH);
  m_rcControl.offset_rect(5,3);

  bool bMouseOver = m_rcControl.point_in_rect(m_pMouseState->m_lastX, m_pMouseState->m_lastY);

  
  if(bMouseOver)
  {
    SDL_Color clr = {220,109,129,0};
    m_fnt.draw_solid(m_left, m_top, gfx_ttf_font::hLeft, gfx_ttf_font::vTop, m_text, fontSize, clr);
  }
  else
  {
    SDL_Color clr = {0,255,0,0};
    m_fnt.draw_solid(m_left, m_top, gfx_ttf_font::hLeft, gfx_ttf_font::vTop, m_text, fontSize, clr);
  }
  
  if(bMouseOver)
  {
    SDL_SetRenderDrawColor(sdl_renderer(), 220, 109,129, 255);
    SDL_Rect rc = {m_rcControl.left(), m_rcControl.top(), m_rcControl.w(), m_rcControl.h() };
    SDL_RenderDrawRect(sdl_renderer(), &rc);    
  }
}

bool ButtonControl::PtInControl( int x, int y )
{
  return m_rcControl.point_in_rect(x, y);
}

} //namespace Game
