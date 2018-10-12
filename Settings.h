#pragma once
#include <map>
#include "RefCounterImpl.h"
#include "RefCountedPtrBase.h"
#include "LuaScript.h"
#include <memory>


namespace Game
{


struct typeinfo_compare 
{
  bool operator ()(const std::type_info* a, const std::type_info* b) const 
  {
    return ((a->before(*b)) != 0);
  }
};

class EntriesCollectionBase
{
public:
  virtual ~EntriesCollectionBase()
  {}

  virtual EntriesCollectionBase* clone() const = 0;
};

template<typename T>
class EntriesCollection : public EntriesCollectionBase
{
  typedef std::map<std::string, T> EntriesMap;
  EntriesMap m_entries;

public:
  EntriesCollection()
  {}
  EntriesCollection(const EntriesCollection<T>& other)
  {
    m_entries = other.m_entries;
  }
  virtual ~EntriesCollection()
  {}

  virtual EntriesCollectionBase* clone() const
  {
    EntriesCollection<T>* pc = new EntriesCollection<T>(*this);
    return pc;
  }

  T Get(const char* name, const T& defVal) const
  {
    std::string key(name);
    EntriesMap::const_iterator it = m_entries.find(key);
    if(it == m_entries.end())
      return defVal;

    return it->second;
  }

  void Set(const char* name, const T& val)
  {
    m_entries[name] = val;
  }

  bool HasEntry(const char* name) const
  {
    std::string key(name);
    EntriesMap::const_iterator it = m_entries.find(key);
    if(it == m_entries.end())
      return false;

    return true;
  }

  void CopyEntry(const char* thisName, const char* toName, EntriesCollection<T>& copyTo) const
  {
    if(!HasEntry(thisName))
      return;

    copyTo.Set( toName, Get(thisName, T()) );
  }
};

class Settings
{
  typedef std::map<const std::type_info*, EntriesCollectionBase*, typeinfo_compare> SettingsMap;
  SettingsMap m_map;

protected:
  template<typename T>
  const EntriesCollection<T>* FindCollection() const
  {
    const std::type_info* ti = &typeid(T);

    SettingsMap::const_iterator it = m_map.find(ti);
    if(it != m_map.end())
      return (const EntriesCollection<T>*)it->second;

    return NULL;
  }

  template<typename T>
  EntriesCollection<T>* FindCollection() 
  {
    const std::type_info* ti = &typeid(T);

    SettingsMap::iterator it = m_map.find(ti);
    if(it != m_map.end())
      return (EntriesCollection<T>*)it->second;

    return NULL;
  }

  template<typename T>
  EntriesCollection<T>* CreateCollection()
  {
    EntriesCollection<T>* pc = FindCollection<T>();
    if(pc != NULL)
      return pc;

    EntriesCollection<T>* pheap = new EntriesCollection<T>();
    const std::type_info* ti = &typeid(T);
    m_map[ti] = pheap;

    return pheap;
  }

public:
  Settings()
  {
  }

  Settings(const Settings& s)
  {
    CopyFrom(s);
  }
  Settings& operator =(const Settings& s)
  { 
    CopyFrom(s);
    return*this;
  }

  virtual ~Settings()
  {
    Clear();
  }

  void Clear()
  {
    EntriesCollectionBase* ptr = NULL;
    SettingsMap::iterator it = m_map.begin();
    while (it != m_map.end())
    {
      ptr = it->second;
      delete ptr;

      ++it;
    }

    m_map.clear();
  }

  void CopyFrom(const Settings& other)
  {
    Clear();

    const SettingsMap& otherMap = other.m_map;

    EntriesCollectionBase* ptr = NULL;
    SettingsMap::const_iterator it = otherMap.begin();
    while (it != otherMap.end())
    {
      ptr = it->second;
      m_map[it->first] = ptr->clone();
      
      ++it;
    }    
  }
  
  template<typename T>
  T Get(const char* name, const T& defVal) const
  {
    const EntriesCollection<T>* pCollection = FindCollection<T>();
    if(pCollection == NULL)
      return defVal;

    return pCollection->Get(name, defVal);
  }

  template<typename T>
  void Set(const char* name, const T& val)
  {
    EntriesCollection<T>* pCollection = CreateCollection<T>();
    if(pCollection == NULL)
      return;

    return pCollection->Set(name, val);
  }

  template<typename T>
  bool HasEntry(const char* name) const
  {
    const EntriesCollection<T>* pCollection = FindCollection<T>();
    if(pCollection == NULL)
      return false;

    return pCollection->HasEntry(name);
  }

  template<typename T>
  void CopyEntry(const char* thisName, const char* toName, Settings& copyTo) const
  {
    if(!HasEntry<T>(thisName))
      return;

    copyTo.Set( toName, Get(thisName, T()) );
  }

  // specialized functions
  std::string GetS(const char* name, const std::string& defVal) const
  {
    return Get(name, defVal);
  }
  void  SetS(const char* name, const std::string& val)
  {
    return Set(name, val);
  }

  const char* GetC(const char* name, const char* defVal) const
  {
    return Get<const char*>(name, defVal);
  }
  void  SetC(const char* name, const char* val)
  {
    return Set<const char*>(name, val);
  }
};


} // nanmespace Game