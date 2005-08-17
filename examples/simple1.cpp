/***********************************************************************
 simple1.cpp - Example showing the simplest way to get data from a MySQL
    table with MySQL++.

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

#include "util.h"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;

int
main(int argc, char *argv[])
{
	// Connect to the sample database.
	mysqlpp::Connection con(false);
	if (!connect_to_db(argc, argv, con)) {
		return 1;
	}

	// Retrieve a subset of the sample stock table set up by resetdb
	mysqlpp::Query query = con.query();
	query << "select item from stock";
	vector<mysqlpp::Row> res;
	query.storein(res);

	// Display the result set
	cout << "We have:" << endl;
	vector<mysqlpp::Row>::iterator it;
	for (it = res.begin(); it != res.end(); ++it) {
		cout << '\t' << it->at(0) << endl;
	}

	return 0;
}
