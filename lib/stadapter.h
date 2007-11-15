/// \file stadapter.h
/// \brief Declares the SQLTypeAdapter class

/***********************************************************************
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

#if !defined(MYSQLPP_SQL_TYPE_ADAPTER_H)
#define MYSQLPP_SQL_TYPE_ADAPTER_H

#include "common.h"

#include "datetime.h"
#include "null.h"
#include "sql_types.h"

#include <stdexcept>
#include <string>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
class RefCountedBuffer;
#endif

/// \brief Converts many different data types to strings suitable for
/// use in SQL queries.
///
/// This class provides implicit conversion between many C++ types and
/// SQL-formatted string representations of that data without losing
/// important type information.  This class is not for direct use
/// outside MySQL++ itself.  It exists for those interfaces in MySQL++
/// that need to accept a value of any reasonable data type which it
/// will use in building a query string.
///
/// One major use for this is in the Query class interfaces for building
/// template queries: they have to be generic with respect to argument
/// type, but because we know we want the data in some kind of string
/// form eventually, we don't need to templatize it. The interface can 
/// just use SQLTypeAdapter, which lets callers pass any reasonable data
/// type. The adapter converts the passed value implicitly.
///
/// The other major use for this type is the quoting and escaping logic
/// in Query's stream interface: rather than overload the << operators
/// and the manipulators for every single type we know the rules for \e a 
/// \e priori, we just specialize the manipulators for SQLTypeAdapter.
/// The conversion to SQLTypeAdapter stringizes the data, which we needed
/// anyway for stream insertion, and holds enough type information so
/// that the manipulator can decide whether to do automatic quoting
/// and/or escaping.

class MYSQLPP_EXPORT SQLTypeAdapter
{
public:
	typedef size_t size_type;	///< size of length values

	/// \brief Default constructor; empty string
	SQLTypeAdapter();

	/// \brief Copy ctor
	///
	/// \param other the other SQLTypeAdapter object
	///
	/// This ctor only copies the pointer to the other SQLTypeAdapter's
	/// data buffer and increments its reference counter.  If you need a
	/// deep copy, use one of the ctors that takes a string.
	SQLTypeAdapter(const SQLTypeAdapter& other);

	/// \brief Create a copy of a C++ string
	SQLTypeAdapter(const std::string& str, bool processed = false);

	/// \brief Create a copy of a null-terminated C string
	SQLTypeAdapter(const char* str, bool processed = false);

	/// \brief Create a copy of an arbitrary block of data
	SQLTypeAdapter(const char* str, int len, bool processed = false);

	/// \brief Create a single-character string
	///
	/// If you mean for \c c to be treated as a small integer, you
	/// should be using mysqlpp::tiny_int instead.  It avoids the
	/// confusion in C++ between integer and character.  See the
	/// documentation for tiny_int.h for details.
	SQLTypeAdapter(char c);

	/// \brief Create a string representation of SQL \c TINYINT
	SQLTypeAdapter(sql_tinyint i);

	/// \brief Create a string representation of SQL \c TINYINT
	/// \c UNSIGNED
	SQLTypeAdapter(sql_tinyint_unsigned i);

	/// \brief Create a string representation of a \c short \c int value
	SQLTypeAdapter(short i);

	/// \brief Create a string representation of an \c unsigned
	/// \c short \c int value
	SQLTypeAdapter(unsigned short i);

	/// \brief Create a string representation of an \c int value
	SQLTypeAdapter(int i);

	/// \brief Create a string representation of an \c unsigned \c int
	/// value
	SQLTypeAdapter(unsigned i);

	/// \brief Create a string representation of a \c longlong value
	SQLTypeAdapter(longlong i);

	/// \brief Create a string representation of an \c unsigned
	/// \c longlong value
	SQLTypeAdapter(ulonglong i);

	/// \brief Create a string representation of a \c float value
	SQLTypeAdapter(float i);

	/// \brief Create a string representation of a \c double value
	SQLTypeAdapter(double i);

	/// \brief Create a SQL string representation of a date
	SQLTypeAdapter(const Date& d);

	/// \brief Create a SQL string representation of a date and time
	SQLTypeAdapter(const DateTime& dt);

	/// \brief Create a SQL string representation of a time
	SQLTypeAdapter(const Time& t);

	/// \brief Create object representing SQL NULL
	SQLTypeAdapter(const null_type& i);

	/// \brief Standard assignment operator
	///
	/// Detaches this object from its internal buffer and attaches
	/// itself to the other object's buffer, with reference counting
	/// on each side.  If you need a deep copy, assign a string instead.
	SQLTypeAdapter& operator =(const SQLTypeAdapter& rhs);

	/// \brief Copy a C string into this object
	SQLTypeAdapter& operator =(const char* str);

	/// \brief Copy a C++ \c string into this object
	SQLTypeAdapter& operator =(const std::string& str);

	/// \brief Returns the character at a given position within the
	/// string buffer.
	///
	/// \throw out_of_range if the internal buffer is not initialized
	/// (default ctor called, and no assignment operator subsequently)
	/// or if there are not at least i + 1 characters in the buffer
	char at(size_type i) const throw(std::out_of_range);

	/// \brief Compare the internal buffer to the given string
	///
	/// Works just like string::compare(const std::string&).
	int compare(const SQLTypeAdapter& other) const;

	/// \brief Compare the internal buffer to the given string
	///
	/// Works just like string::compare(const std::string&).
	int compare(const std::string& other) const;

	/// \brief Compare the internal buffer to the given string
	///
	/// Works just like string::compare(size_type, size_type, 
	/// std::string&).
	int compare(size_type pos, size_type num, std::string& other) const;

	/// \brief Compare the internal buffer to the given string
	///
	/// Works just like string::compare(const char*).
	int compare(const char* other) const;

	/// \brief Compare the internal buffer to the given string
	///
	/// Works just like string::compare(size_type, size_type, 
	/// const char*).
	int compare(size_type pos, size_type num, const char* other) const;

	/// \brief Return pointer to raw data buffer
	const char* data() const;

	/// \brief Returns true if we were initialized with a data type
	/// that must be escaped when used in a SQL query
	bool escape_q() const;

	/// \brief Returns true if the internal 'processed' flag is set.
	///
	/// This is an implementation detail of template queries, used to
	/// prevent repeated processing of values.
	bool is_processed() const { return is_processed_; }

	/// \brief Return number of bytes in data buffer
	size_type length() const;
	size_type size() const { return length(); } ///< alias for length()

	/// \brief Returns true if we were initialized with a data type
	/// that must be quoted when used in a SQL query
	bool quote_q() const;

	/// \brief Returns the type ID of the buffer's data
	///
	/// Values from type_info.h.  At the moment, these are the same as
	/// the underlying MySQL C API type IDs, but it's not a good idea
	/// to count on this remaining the case.
	int type_id() const;

	/// \brief Turns on the internal 'is_processed_' flag.
	///
	/// This is an implementation detail of template queries, used to
	/// prevent repeated processing of values.
	void set_processed() { is_processed_ = true; }

#if !defined(DOXYGEN_IGNORE)
	// Parallel interface for Null<>-wrapped versions of types we
	// support above.  No need for parallel documentation.
	SQLTypeAdapter(const Null<std::string>& str, bool processed = false);
	SQLTypeAdapter(Null<char> c);
	SQLTypeAdapter(Null<sql_tinyint> i);
	SQLTypeAdapter(Null<sql_tinyint_unsigned> i);
	SQLTypeAdapter(Null<short> i);
	SQLTypeAdapter(Null<unsigned short> i);
	SQLTypeAdapter(Null<int> i);
	SQLTypeAdapter(Null<unsigned> i);
	SQLTypeAdapter(Null<longlong> i);
	SQLTypeAdapter(Null<ulonglong> i);
	SQLTypeAdapter(Null<float> i);
	SQLTypeAdapter(Null<double> i);
	SQLTypeAdapter(const Null<Date>& d);
	SQLTypeAdapter(const Null<DateTime>& dt);
	SQLTypeAdapter(const Null<Time>& t);
	SQLTypeAdapter& operator =(const Null<std::string>& str);
	SQLTypeAdapter& operator =(const null_type& n);
#endif // !defined(DOXYGEN_IGNORE)

private:
	/// \brief Decrement the buffer's reference count
	///
	/// Called by dtor and by operator=()s before replacing the buffer's
	/// contents.  If ref count falls to 0, deallocates the buffer.
	void dec_ref_count();
 
	/// \brief Our internal string buffer
	RefCountedBuffer* buffer_;

	/// \brief If true, one of the MySQL++ manipulators has processed
	/// the string data.
	///
	/// "Processing" is escaping special SQL characters, and/or adding
	/// quotes.  See the documentation for manip.h for details.
	///
	/// This flag is used by the template query mechanism, to prevent a
	/// string from being re-escaped or re-quoted each time that query
	/// is reused.  The flag is reset by operator=, to force the new
	/// parameter value to be re-processed.
	bool is_processed_;
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SQL_TYPE_ADAPTER_H)
