/***********************************************************************
 simple1.cpp - Example showing the simplest way to get data from a MySQL
    table with MySQL++.

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

#include "cmdline.h"
#include "printdata.h"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;

int
main(int argc, char *argv[])
{
	// Get database access parameters from command line
    const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass)) {
		return 1;
	}

	// Connect to the sample database.
	mysqlpp::Connection conn(false);
	if (conn.connect(db, server, user, pass)) {
		// Retrieve a subset of the sample stock table set up by resetdb
		// and display it.
		mysqlpp::Query query = conn.query("select item from stock");
		if (mysqlpp::StoreQueryResult res = query.store()) {
			cout << "We have:" << endl;
			for (size_t i = 0; i < res.num_rows(); ++i) {
				cout << '\t' << res[i][0] << endl;
			}
		}
		else {
			cerr << "Failed to get item list: " << query.error() << endl;
			return 1;
		}

		return 0;
	}
	else {
		cerr << "DB connection failed: " << conn.error() << endl;
		return 1;
	}
}
