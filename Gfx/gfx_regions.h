#pragma once
#include <map>
#include "RefCounterImpl.h"
#include "gerect.h"
#include <vector>
#include "RefCountedPtrBase.h"

//////////////////////////////////////////////////////////////////////////
//
struct gfx_region
{
  typedef std::vector<ge_rectF> RegionItem;
  RegionItem mItems;

  ge_rectF mBoundingBox;

public:
  gfx_region()
  {}

  gfx_region(const gfx_region& rgn)
  {
    CopyFrom(rgn);
  }

  gfx_region& operator = (const gfx_region& rgn)
  {
    CopyFrom(rgn);
    return *this;
  }

  void CopyFrom(const gfx_region& rgn)
  {
    mItems = rgn.mItems;
    mBoundingBox = rgn.mBoundingBox;
  }

  void Push(const ge_rectF& rc)
  {
    UpdateBoundingBoxWith(rc);

    mItems.push_back(rc);
  }

  bool IsEmpty() const
  {
    return mItems.empty();
  }

  size_t Size() const
  {
    return mItems.size();
  }

  void UpdateBoundingBoxWith( const ge_rectF& rc )
  {
    if(mBoundingBox.isEmpty())
    {
      mBoundingBox = rc;
      return;
    }

    if(mBoundingBox.left() > rc.left())
      mBoundingBox.set_left(rc.left());

    if(mBoundingBox.right() < rc.right())
      mBoundingBox.set_right(rc.right());

    if(mBoundingBox.top() > rc.top())
      mBoundingBox.set_top(rc.top());

    if(mBoundingBox.bottom() < rc.bottom())
      mBoundingBox.set_bottom(rc.bottom());
  }

};

class FrameRegions : public RefCounterImpl
{
public:
  typedef std::map<int, gfx_region> FrameToRegionMap;

  FrameRegions()
  {}

  FrameRegions(const FrameRegions& frgn)
  {
    m_regions = frgn.m_regions;
  }

  FrameRegions& operator = (const FrameRegions& frgn)
  {
    m_regions = frgn.m_regions;
    return *this;
  }

  gfx_region& operator[](int key)
  {
    return m_regions[key];
  }

  gfx_region& region(int frame)
  {
    return m_regions[frame];
  }

  int frames_count() const
  {
    return (int)m_regions.size();
  }

  FrameToRegionMap m_regions;
};

typedef RefCountedPtrBase<FrameRegions> FrameRegionsPtr;