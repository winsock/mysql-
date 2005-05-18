/// \file myset.h
/// \brief Declares templates for generating custom containers used
/// elsewhere in the library.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#ifndef MYSQLPP_MYSET_H
#define MYSQLPP_MYSET_H

#include "defs.h"

#include "coldata.h"
#include "stream2string.h"

#include <iostream>
#include <set>
#include <vector>

namespace mysqlpp {

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

template <class T, class value_type = typename T::value_type>
class ListInsert {
private:
  T *object;
public:
  ListInsert(T *o) {object = o;}
  void operator () (const value_type &data) {object->push_back(data);}
};

template <class T, class key_type = typename T::key_type> 
class SetInsert {
private:
  T *object;
public:
  SetInsert(T *o) {object = o;}
  void operator () (const key_type &data) {object->insert(data);}
};

template <class T>
inline SetInsert<std::set<T> > set_insert(std::set<T> *o)
{
  return SetInsert<std::set<T> >(o);
}

template <class T>
inline ListInsert<std::vector<T> > set_insert(std::vector<T> *o)
{
  return ListInsert<std::vector<T> >(o);
}

template <class Insert>
void set2container (const char *str, Insert insert);

/// \endif


/// \brief A special std::set derivative for holding MySQL data sets.
template <class Container = std::set<std::string> >
class Set : public Container {
public:
  Set(const char* str) { set2container(str, set_insert(this)); }
  Set(const std::string &str)
  {
    set2container(str.c_str(), set_insert(this));
  }
  Set(const ColData& str) 
  {
  	set2container(str.c_str(), set_insert(this));
  }
  
  std::ostream& out_stream(std::ostream &s) const;
  
  operator std::string ();
};


/// \brief Inserts a Set object into a C++ stream
template <class Container>
inline std::ostream& operator <<(std::ostream& s, const Set<Container>& d)
{ 
  return d.out_stream(s); 
}


template <class Container> 
inline Set<Container>::operator std::string () 
{
  return stream2string<std::string>(*this);
}


/// \if INTERNAL
// Doxygen will not generate documentation for this section.

template <class Insert>
void set2container(const char* str, Insert insert)
{
  while (1) {
    MutableColData s("");
    while (*str != ',' && *str) {
      s += *str;
      str++;
    }
    insert(s);
    if (!*str) break;
    str++;
  }
}

/// \endif


template <class Container>
std::ostream& Set<Container>::out_stream(std::ostream& s) const
{
  typename Container::const_iterator i = Container::begin();
  typename Container::const_iterator e = Container::end();
  while (true) {
    s << *i;
    i++;
    if (i==e) break;
    s << ",";
  }
  return s;
}

} // end namespace mysqlpp

#endif

