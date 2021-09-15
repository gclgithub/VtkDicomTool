#pragma once
#include <memory>
#include <vector>
#include <exception>
#include <map>
#include <string>
#include <sstream>
#include <array>
#include <algorithm>
#include <iterator>
#include <set>
#include <tuple>
#include <thread>
#include <atomic>
#include <functional>
#include <tuple>
#include <list>
#include <type_traits>
#include <assert.h>
#include <fstream>
#include <future>
#include <math.h>
#include <limits>
#include <stdio.h>
#include <iomanip>
#include <mutex>
#include <shared_mutex>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

template<typename Type>
using PTR = std::shared_ptr<Type>;

template<typename Type>
using WEAKPTR = std::weak_ptr<Type>;

#define VarMacro(type, var_name)\
protected:\
type  m_##var_name{};\

#define GetInnerRefMacro(type, var_name)\
public:\
type&  get_##var_name(){return m_##var_name;}\

#define GetInnerConstRefMacro(type, var_name)\
public:\
const type&  get_##var_name()const{return  m_##var_name;}\

#define GetInnerCopyMacro(type, var_name)\
public:\
type get_##var_name()const{return m_##var_name;}\

#define GetInnerMacro(type, var_name)\
GetInnerRefMacro(type, var_name)\
GetInnerConstRefMacro(type, var_name)


#define SetInnerMacro(type, var_name)\
public:\
void set_##var_name(const type& var_name){ m_##var_name = var_name;}

#define SetMacro(type, var_name)\
VarMacro(type, var_name)\
SetInnerMacro(type, var_name)

#define GetMacro(type, var_name)\
VarMacro(type, var_name)\
GetInnerCopyMacro(type, var_name)

#define SetGetMacro(type, var_name)\
VarMacro(type, var_name)\
GetInnerMacro(type, var_name)\
SetInnerMacro(type, var_name)