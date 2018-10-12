#include "stdafx.h"
#include "gfx_animation.h"
#include "SDL.h"
#include "Framework.h"
#include "sdl_connectors.h"
#include "lua\DebugDrawSettings.h"

gfx_animation::gfx_animation()
: _sprite(NULL), _numFrames(0), _fps(0), _bPlaying(false), _curFrame(0), _secsSinceLastFrame(0)
{
}

gfx_animation::gfx_animation( SDL_Texture* pTex, int numFrames, float fps )
	: _sprite(pTex), _numFrames(numFrames), _fps(fps), _bPlaying(false), _curFrame(0), _secsSinceLastFrame(0)
{
  init_fields();
}

gfx_animation::~gfx_animation()
{}

void gfx_animation::init( SDL_Texture* pTex, int numFrames, float fps )
{
  _sprite.init(pTex);
  _numFrames = numFrames;
  _fps = fps;

  init_fields();
}

void gfx_animation::init( SDL_Texture* pTex, int numFrames, float fps, FrameRegionsPtr& frPtr )
{
  init(pTex, numFrames, fps);
  m_regions = frPtr;
}

void gfx_animation::init_fields()
{
  _fSpeed = 1.0f / _fps;

  _frameWidth = _sprite.texture_width() / _numFrames;
  _frameHeight = _sprite.texture_height();

  _hotSpot.set(_frameWidth / 2.0f, _frameHeight / 2.0f);

  set_mode(playLoop);

  set_frame(0); 

  _bPlaying = true;
}

void gfx_animation::play()
{
	_bPlaying = true;
}

void gfx_animation::stop()
{
	_bPlaying = false;
}

bool gfx_animation::update( float dt )
{
	if( !_bPlaying )
		return false;

	_secsSinceLastFrame += dt;

	int framesPassed = (int)(_secsSinceLastFrame * _fps);
	if(framesPassed < 1)
		return false;

  int lastFrame = _curFrame;

  _secsSinceLastFrame -= (framesPassed / _fps);

  if(get_mode() == playForward)
  {
    if(_curFrame == _numFrames - 1 && framesPassed > 0)
      _bPlaying = false;

    limited_increment(_curFrame, framesPassed, _numFrames-1);
  }
  else
  {
    loop_increment(_curFrame, framesPassed, 0, _numFrames-1);
  }
	
  set_frame(_curFrame);

  return (_curFrame != lastFrame);
}

bool gfx_animation::is_playing() const
{
	return _bPlaying;
}

gfx_animation::PlayMode gfx_animation::get_mode() const
{
  return _playMode;
}

void gfx_animation::set_mode( PlayMode mode )
{
  _playMode = mode;
}

void gfx_animation::set_frame( int n )
{
	assert(n >= 0);
  assert(_numFrames > 0);
	//assert(n < _numFrames);

	while (n >= _numFrames)
    n -= _numFrames;

	int x = _frameWidth * n;

	_sprite.set_source_rect(x, 0, _frameWidth, _frameHeight);  
  _sprite.set_hot_spot(_hotSpot.x() + _frameWidth*_curFrame, _hotSpot.y());
}

float gfx_animation::get_scale() const
{
  return _sprite.get_scale();
}

void gfx_animation::set_scale( float scale )
{
  _sprite.set_scale(scale);
}

void gfx_animation::set_flipped_h( bool bHorisontalFlip )
{
  _sprite.set_flipped_h(bHorisontalFlip);
}

bool gfx_animation::is_flipped_h() const
{
  return _sprite.is_flipped_h();
}

float gfx_animation::fps() const
{
  return _fps;
}

void gfx_animation::set_fps( float fps )
{
  _fps = fps;
}

ge_rectI gfx_animation::frame_size()
{
  return ge_rectI(0, 0, _frameWidth, _frameHeight);
}

ge_rectF gfx_animation::frame_box(float x, float y)
{
  ge_rectF rcRes = frame_size();
  rcRes.scaleBy( get_scale() );
  rcRes.moveBy(x - _hotSpot.x()*get_scale(), y - _hotSpot.y()*get_scale());

  return rcRes;
}

ge_rectF gfx_animation::bounding_box( float x, float y )
{
  gfx_region& rgn = m_regions->region( get_frame() );

  ge_rectF rcRes = rgn.mBoundingBox;
  rcRes.scaleBy( get_scale() );
  rcRes.moveBy(x - _hotSpot.x()*get_scale(), y - _hotSpot.y()*get_scale());

  return rcRes;
}

//gfx_region gfx_animation::bounding_region( float x, float y )
//{
//  gfx_region& rgn = m_regions->region( get_frame() );
//  return rgn;
//}

void gfx_animation::bounding_region(float x, float y, gfx_region& res)
{
  const gfx_region& rgn = m_regions->region( get_frame() );

  bool bFlipped = is_flipped_h();

  float dx = x - _hotSpot.x()*get_scale();
  float dy = y - _hotSpot.y()*get_scale();
  ge_rectF rc;

  gfx_region::RegionItem::const_iterator it = rgn.mItems.begin();
  while(it != rgn.mItems.end())
  {
    rc = *it;
    if(bFlipped)
      rc.set_left( _frameWidth - rc.right() );
    rc.scaleBy(get_scale());
    rc.moveBy(dx, dy);

    res.Push(rc);

    ++it;
  }
}

void gfx_animation::render( SDL_Renderer *pDstRenderer, float x, float y, float rotAngle /*= 0.0f*/ )
{
	_sprite.render(pDstRenderer, x, y, rotAngle);

  // drawing frame box
  if(DbgSettings().dd_anim_draw_framebox)
  {
    SDL_SetRenderDrawColor(pDstRenderer, 255, 0, 0 ,0);
    ge_rectF rc = frame_box(x, y);
    SDL_RenderDrawLine(pDstRenderer, rc.left(), rc.top(), rc.right(), rc.top());
    SDL_RenderDrawLine(pDstRenderer, rc.right(), rc.top(), rc.right(), rc.bottom());
    SDL_RenderDrawLine(pDstRenderer, rc.right(), rc.bottom(), rc.left(), rc.bottom());
    SDL_RenderDrawLine(pDstRenderer, rc.left(), rc.bottom(), rc.left(), rc.top());

    int rsize = 2;
    SDL_Rect rcTargetPt = {x-rsize, y-rsize, rsize*2, rsize*2 };
    SDL_RenderFillRect(pDstRenderer, &rcTargetPt);
  }

  // drawing frame bounding box
  if(DbgSettings().dd_anim_draw_boundingbox)
  {
    SDL_SetRenderDrawColor(pDstRenderer, 255, 0, 0 ,0);
    ge_rectF rc = bounding_box(x, y);
    SDL_RenderDrawLine(pDstRenderer, rc.left(), rc.top(), rc.right(), rc.top());
    SDL_RenderDrawLine(pDstRenderer, rc.right(), rc.top(), rc.right(), rc.bottom());
    SDL_RenderDrawLine(pDstRenderer, rc.right(), rc.bottom(), rc.left(), rc.bottom());
    SDL_RenderDrawLine(pDstRenderer, rc.left(), rc.bottom(), rc.left(), rc.top());

    int rsize = 2;
    SDL_Rect rcTargetPt = {x-rsize, y-rsize, rsize*2, rsize*2 };
    SDL_RenderFillRect(pDstRenderer, &rcTargetPt);
  }

  // drawing frame regions
  if(DbgSettings().dd_anim_draw_regions)
  {
    SDL_SetRenderDrawColor(pDstRenderer, 255, 0, 0 ,0);

    gfx_region rgn;
    bounding_region(x, y, rgn);

    gfx_region::RegionItem::iterator it = rgn.mItems.begin();
    while(it != rgn.mItems.end())
    {
      ge_rectF& rcS = *it;
      SDL_RenderDrawLine(pDstRenderer, rcS.left(), rcS.top(), rcS.right(), rcS.top());
      
      ++it;
    }
  }
}

void gfx_animation::render( float x, float y, float rotAngle /*= 0.0f*/ )
{
  render( sdl_renderer(), x, y, rotAngle );
}

void gfx_animation::render( float x, float y, float w, float h )
{
  _sprite.render(x, y, w, h);
}
