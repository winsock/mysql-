/***********************************************************************
 row.cpp - Implements the Row class.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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

#include "row.h"
#include "result.h"
#include "exceptions.h"

namespace mysqlpp {

Row::Row(MYSQL_ROW d, const ResUse* r, unsigned long* lengths,
		bool throw_exceptions) :
OptionalExceptions(throw_exceptions),
initialized_(false),
size_(r ? r->num_fields() : 0)
{
	if (r) {
		field_names_ = r->field_names();
	}
	
	if (d && r) {
		data_.reserve(size_);
		for (size_type i = 0; i < size_; ++i) {
			bool is_null = d[i] == 0;
			data_.push_back(value_type(
					is_null ? "NULL" : d[i],
					is_null ? 4 : lengths[i],
					r->field_type(i),
					is_null));
		}

		initialized_ = true;
	}
	else if (throw_exceptions) {
		throw ObjectNotInitialized("ROW or RES is NULL");
	}
}


const Row::value_type&
Row::operator [](const char* field) const
{
	size_type si = field_num(field);
	if (si < size()) {
		return at(si);
	}
	else {
		throw BadFieldName(field);
	}
}


Row::size_type
Row::field_num(const char* name) const
{
	if (field_names_) {
		return (*field_names_)[name];
	}
	else {
		if (throw_exceptions()) {
			throw BadFieldName(name);
		}
		else {
			return 0;
		}
	}
}


value_list_ba<FieldNames, do_nothing_type0>
Row::field_list(const char* d) const
{
	return value_list_ba<FieldNames, do_nothing_type0>
			(*field_names_, d, do_nothing);
}

template <class Manip>
value_list_ba<FieldNames, Manip>
Row::field_list(const char *d, Manip m) const
{
	return value_list_ba<FieldNames, Manip>(*field_names_, d, m);
}

template <class Manip>
value_list_b<FieldNames, Manip>
Row::field_list(const char *d, Manip m, const std::vector<bool>& vb) const
{
	return value_list_b<FieldNames, Manip>(*field_names_, vb, d, m);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(const char* d, const std::vector<bool>& vb) const
{
	return value_list_b<FieldNames, quote_type0>(*field_names_,
			vb, d, quote);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(const std::vector<bool>& vb) const
{
	return value_list_b<FieldNames, quote_type0>(*field_names_,
			vb, ",", quote);
}

template <class Manip> value_list_b<FieldNames, Manip>
Row::field_list(const char* d, Manip m, bool t0, bool t1, bool t2,
		bool t3, bool t4, bool t5, bool t6, bool t7, bool t8, bool t9,
		bool ta, bool tb, bool tc) const
{
	std::vector<bool> vb;
	create_vector(field_names_->size(), vb, t0, t1, t2, t3, t4,
			t5, t6, t7, t8, t9, ta, tb, tc);
	return value_list_b<FieldNames, Manip>(*field_names_, vb, d, m);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(const char *d, bool t0, bool t1, bool t2, bool t3,
		bool t4, bool t5, bool t6, bool t7, bool t8, bool t9, bool ta,
		bool tb, bool tc) const
{
	std::vector<bool> vb;
	create_vector(field_names_->size(), vb, t0, t1, t2, t3, t4,
			t5, t6, t7, t8, t9, ta, tb, tc);
	return value_list_b<FieldNames, quote_type0>(*field_names_,
			vb, d, quote);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(bool t0, bool t1, bool t2, bool t3, bool t4, bool t5,
		bool t6, bool t7, bool t8, bool t9, bool ta, bool tb,
		bool tc) const
{
	std::vector<bool> vb;
	create_vector(field_names_->size(), vb, t0, t1, t2, t3, t4,
			t5, t6, t7, t8, t9, ta, tb, tc);
	return value_list_b<FieldNames, quote_type0>(*field_names_,
			vb, ",", quote);
}

equal_list_ba<FieldNames, Row, quote_type0>
Row::equal_list(const char* d, const char* e) const
{
	return equal_list_ba<FieldNames, Row, quote_type0>(
			*field_names_, *this, d, e, quote);
}

template <class Manip>
equal_list_ba<FieldNames, Row, Manip>
Row::equal_list(const char* d, const char* e, Manip m) const 
{
	return equal_list_ba<FieldNames, Row, Manip>(
			*field_names_, *this, d, e, m);
}

} // end namespace mysqlpp

