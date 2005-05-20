/***********************************************************************
 connection.cpp - Implements the Connection class.

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

#define MYSQLPP_NOT_HEADER
#include "platform.h"

#include "connection.h"

#include "query.h"
#include "result.h"

#if defined(HAVE_MYSQL_SHUTDOWN_LEVEL_ARG)
#	define SHUTDOWN_ARG ,SHUTDOWN_DEFAULT
#else
#	define SHUTDOWN_ARG
#endif

using namespace std;

namespace mysqlpp {

Connection::Connection() :
throw_exceptions(true),
locked(false) 
{
	mysql_init(&mysql);
}

Connection::Connection(bool te) :
throw_exceptions(te),
is_connected(false),
locked(true),
Success(false)
{
	mysql_init(&mysql);
}

Connection::Connection(const char* db, const char* host,
		const char* user, const char* passwd, bool te) :
throw_exceptions(te),
locked(false)
{
	mysql_init(&mysql);
	if (real_connect(db, host, user, passwd, 3306, 0, 60, NULL, 0)) {
		locked = false;
		Success = is_connected = true;
	}
	else {
		locked = false;
		Success = is_connected = false;
		if (throw_exceptions) {
			throw BadQuery(error());
		}
	}
}


Connection::Connection(const char* db, const char* host,
		const char* user, const char* passwd, uint port,
		my_bool compress, unsigned int connect_timeout,
		bool te, cchar* socket_name, unsigned int client_flag) :
throw_exceptions(te),
locked(false)
{
	mysql_init(&mysql);
	if (real_connect(db, host, user, passwd, port, compress,
			connect_timeout, socket_name, client_flag)) {
		locked = false;
		Success = is_connected = true;
	}
	else {
		locked = false;
		Success = is_connected = false;
		if (throw_exceptions) {
			throw BadQuery(error());
		}
	}
}

bool Connection::real_connect(cchar* db, cchar* host, cchar* user,
		cchar* passwd, uint port, my_bool compress,
		unsigned int connect_timeout, cchar* socket_name,
		unsigned int client_flag)
{
	mysql.options.compress = compress;
	mysql.options.connect_timeout = connect_timeout;
	locked = true;

	mysql_options(&mysql, MYSQL_READ_DEFAULT_FILE, "my");

	if (mysql_real_connect(&mysql, host, user, passwd, db, port,
			socket_name, client_flag)) {
		locked = false;
		Success = is_connected = true;
	}
	else {
		locked = false;
		Success = is_connected = false;
		if (throw_exceptions) {
			throw BadQuery(error());
		}
	}

	if (Success && db && db[0]) {
		Success = select_db(db);
	}

	return Success;
}

Connection::~Connection()
{
	mysql_close(&mysql);
}

bool Connection::select_db(const char *db)
{
	bool suc = !(mysql_select_db(&mysql, db));
	if (throw_exceptions && !suc) {
		throw BadQuery(error());
	}
	else {
		return suc;
	}
}

bool Connection::reload()
{
	bool suc = !mysql_reload(&mysql);
	if (throw_exceptions && !suc) {
		throw BadQuery(error());
	}
	else {
		return suc;
	}
}

bool Connection::shutdown()
{
	bool suc = !(mysql_shutdown(&mysql SHUTDOWN_ARG));
	if (throw_exceptions && !suc) {
		throw BadQuery(error());
	}
	else {
		return suc;
	}
}

bool Connection::connect(cchar* db, cchar* host, cchar* user,
		cchar* passwd)
{
	locked = true;
	
	mysql_options(&mysql, MYSQL_READ_DEFAULT_FILE, "my");

	if (mysql_real_connect(&mysql, host, user, passwd, db, 3306, NULL, 0)) {
		locked = false;
		Success = is_connected = true;
	}
	else {
		locked = false;
		if (throw_exceptions)
			throw BadQuery(error());
		Success = is_connected = false;
	}

	if (Success && db && db[0]) {
		Success = select_db(db);
	}

	return Success;
}

string Connection::info()
{
	const char* i = mysql_info(&mysql);
	if (!i) {
		return string();
	}
	else {
		return string(i);
	}
}

ResNSel Connection::execute(const string& str, bool throw_excptns)
{
	Success = false;
	if (lock()) {
		if (throw_excptns) {
			throw BadQuery("lock failed");
		}
		else {
			return ResNSel();
		}
	}

	Success = !mysql_query(&mysql, str.c_str());
	unlock();
	if (Success) {
		return ResNSel(this);
	}
	else {
		if (throw_excptns) {
			throw BadQuery(error());
		}
		else {
			return ResNSel();
		}
	}
}

bool Connection::exec(const string& str)
{
	Success = !mysql_query(&mysql, str.c_str());
	if (!Success && throw_exceptions)
		throw BadQuery(error());
	return Success;
}

Result Connection::store(const string& str, bool throw_excptns)
{
	Success = false;

	if (lock()) {
		if (throw_excptns) {
			throw BadQuery("lock failed");
		}
		else {
			return Result();
		}
	}

	Success = !mysql_query(&mysql, str.c_str());
	if (Success) {
		MYSQL_RES* res = mysql_store_result(&mysql);
		if (res) {
			unlock();
			return Result(res);
		}
	}
	unlock();

	// One of the mysql_* calls failed, so decide how we should fail.
	if (throw_excptns) {
		throw BadQuery(error());
	}
	else {
		return Result();
	}
}

ResUse Connection::use(const string& str, bool throw_excptns)
{
	Success = false;
	if (lock()) {
		if (throw_excptns) {
			throw BadQuery("lock failed");
		}
		else {
			return ResUse();
		}
	}

	Success = !mysql_query(&mysql, str.c_str());
	if (Success) {
		MYSQL_RES* res = mysql_use_result(&mysql);
		if (res) {
			unlock();
			return ResUse(res, this);
		}
	}
	unlock();

	// One of the mysql_* calls failed, so decide how we should fail.
	if (throw_excptns) {
		throw BadQuery(error());
	}
	else {
		return ResUse();
	}
}

Query Connection::query()
{
	return Query(this, throw_exceptions);
}

} // end namespace mysqlpp

