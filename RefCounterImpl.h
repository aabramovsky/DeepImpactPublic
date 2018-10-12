#pragma once

class RefCounterImpl
{
public:
  RefCounterImpl();
  virtual ~RefCounterImpl();  

  void addRef();
  void release();

  int numRefs() const;

private:
  mutable int m_nRefCounter;
};