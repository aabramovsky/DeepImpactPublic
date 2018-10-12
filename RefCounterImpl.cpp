#include "stdafx.h"
#include "RefCounterImpl.h"
#include <assert.h>


RefCounterImpl::RefCounterImpl() : m_nRefCounter(0)
{

}

RefCounterImpl::~RefCounterImpl()
{
  assert(m_nRefCounter == 0);
}

void RefCounterImpl::addRef()
{
  ++m_nRefCounter;
}

void RefCounterImpl::release()
{
  assert(m_nRefCounter > 0);

  if (!(--m_nRefCounter))
  {
    delete this;
  }
}

int RefCounterImpl::numRefs() const
{
  return m_nRefCounter;
}
