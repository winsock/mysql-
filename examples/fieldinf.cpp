/***********************************************************************
 fieldinf.cpp - Shows how to request information about the fields in a 
    table, such as their SQL and C++-equivalent types.

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

#include <iostream>
#include <iomanip>

using namespace std;


// Access the flag that's set when running under the dtest framework, so
// we modify our output to be testable.
extern bool dtest_mode;


int
main(int argc, char *argv[])
{
	// Get database access parameters from command line
    const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass)) {
		return 1;
	}

	try {
		// Establish the connection to the database server.
		mysqlpp::Connection con(db, server, user, pass);

		// Get contents of main example table
		mysqlpp::Query query = con.query("select * from stock");
		mysqlpp::Result res = query.store();

		// Show info about each field in that table
		char widths[] = { 8, 15, 57 };
		cout.setf(ios::left);
		cout << setw(widths[0]) << "Field" <<
				setw(widths[1]) << "SQL Type" <<
				setw(widths[2]) << "Equivalent C++ Type" <<
				endl;
		for (int i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i) {
			cout << string(widths[i] - 1, '=') << ' ';
		}
		cout << endl;
		
		for (size_t i = 0; i < res.field_names()->size(); i++) {
			// Suppress C++ type name outputs when run under dtest,
			// as they're system-specific.
			const char* cname = dtest_mode ? "n/a" : res.field_type(i).name();
			cout << setw(widths[0]) << res.field_name(i).c_str() <<
					setw(widths[1]) << res.field_type(i).sql_name() <<
					setw(widths[2]) << cname <<
					endl;
		}
		cout << endl;

		// Simple type check
		if (res.field_type(0) == typeid(string)) {
			cout << "SQL type of 'item' field most closely resembles "
					"the C++ string type." << endl;
		}

		// Tricky type check: the 'if' path shouldn't happen because the
		// description field has the NULL attribute.  We need to dig a
		// little deeper if we want to ignore this in our type checks.
		if (res.field_type(5) == typeid(string)) {
			cout << "Should not happen! Type check failure." << endl;
		}
		else if (res.field_type(5) == typeid(mysqlpp::Null<mysqlpp::String>)) {
			cout << "SQL type of 'description' field resembles "
					"a nullable variant of the C++ string type." << endl;
		}
		else {
			cout << "Weird: fifth field's type is now " <<
					res.field_type(5).name() << endl;
			cout << "Did something recently change in resetdb?" << endl;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}