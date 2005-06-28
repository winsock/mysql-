/***********************************************************************
 qparms.cpp - Implements the SQLQuery class.

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

#include "qparms.h"

#include "sql_query.h"

using namespace std;

namespace mysqlpp {

SQLString&
SQLQueryParms::operator [](const char* str)
{
	if (parent) {
		return operator [](parent->parsed_nums[str]);
	}
	throw; // only here temporary 
}

const SQLString&
SQLQueryParms::operator[] (const char* str) const
{
	if (parent) {
		return operator [](parent->parsed_nums[str]);
	}
	throw; // only here temporary 
}

} // end namespace mysqlpp
