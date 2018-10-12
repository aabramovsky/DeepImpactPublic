#pragma once
#include <assert.h>

template<typename T>
class RefCountedPtrBase
{ 
public:
  RefCountedPtrBase()
    : m_pRefObject(NULL)
  {}

  RefCountedPtrBase& operator = (const RefCountedPtrBase& refPtr)
  {
    set(refPtr.m_pRefObject);
    return *this;
  }

  explicit RefCountedPtrBase(T* pRefObject)
    : m_pRefObject(pRefObject)
  {
    m_pRefObject->addRef();
  }

  RefCountedPtrBase(const RefCountedPtrBase& refPtr)
  {
    m_pRefObject = refPtr.m_pRefObject;
    
    if(m_pRefObject != NULL)
      m_pRefObject->addRef();
  }

  ~RefCountedPtrBase()
  {
    if(m_pRefObject != NULL)
      m_pRefObject->release();
  }

  template<typename T>
  T* get()
  {
    return dynamic_cast<T*>(m_pRefObject);
  }

  void detach()
  {
    if(m_pRefObject != NULL)
      m_pRefObject->release();

    m_pRefObject = NULL;
  }

  void set(T* pRefObject)
  {
    assert( m_pRefObject == NULL );
    if(m_pRefObject != NULL)
      m_pRefObject->release();

    m_pRefObject = pRefObject;
    m_pRefObject->addRef();
  }

  /*template<typename T>
  operator T*()
  {
    return get<T>();
  }*/

  T& operator*()
  {
    return *m_pRefObject;
  }

  const T& operator*() const
  {
    return *m_pRefObject;
  }

  T* operator -> ()
  {
    return m_pRefObject;
  }

  const T* operator -> () const
  {
    return m_pRefObject;
  }

  bool operator == (const RefCountedPtrBase& refPtr)
  {
    return (m_pRefObject == refPtr.m_pRefObject);
  }

  template<typename P>
  bool operator == (const P* pObject)
  {
    return (m_pRefObject == pObject);
  }

  template<typename P>
  bool operator != (const P* pObject) const
  {
    return (m_pRefObject != pObject);
  }

  template<typename P>
  bool operator != (P* pObject)
  {
    return (m_pRefObject != pObject);
  }

  bool isNull() const
  {
    return m_pRefObject == NULL;
  }

private:
  T* m_pRefObject;
};
