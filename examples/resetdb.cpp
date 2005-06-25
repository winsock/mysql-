/***********************************************************************
 resetdb.cpp - (Re)initializes the example database, mysql_cpp_data.
 	You must run this at least once before running most of the other
	examples, and it is helpful sometimes to run it again, as some of
	the examples modify the table in this database.

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

using namespace std;

int
main(int argc, char *argv[])
{
	// Connect to database server
	mysqlpp::Connection con;
	try {
		cout << "Connecting to database server..." << endl;
		connect_to_db(argc, argv, con, "");
	}
	catch (exception& er) {
		cerr << "Connection failed: " << er.what() << endl;
		return 1;
	}
	
	// Create new sample database, or re-create it.  We suppress
	// exceptions, because it's not an error if DB doesn't yet exist.
	bool new_db = false;
	{
		mysqlpp::NoExceptions ne(con);
		mysqlpp::Query query = con.query();
		if (con.select_db(kpcSampleDatabase)) {
			// Toss old table, if it exists.  If it doesn't, we don't
			// really care, as it'll get created next.
			cout << "Dropping existing stock table..." << endl;
			query.execute("drop table stock");
		}
		else {
			// Database doesn't exist yet, so create and select it.
			if (con.create_db(kpcSampleDatabase) &&
					con.select_db(kpcSampleDatabase)) {
				new_db = true;
			}
			else {
				cerr << "Error creating DB: " << con.error() << endl;
				return 1;
			}
		}
	}

	// Create sample data table within sample database.
	cout << "Creating new stock table..." << endl;
	try {
		// Send the query to create the table and execute it.
		mysqlpp::Query query = con.query();
		query << "create table stock (item char(20) not null, "
				"num bigint, weight double, price double, sdate date)";
		query.execute();

		// Set up the template query to insert the data.  The parse()
		// call tells the query object that this is a template and
		// not a literal query string.
		query << "insert into %5:table values (%0q, %1q, %2, %3, %4q)";
		query.parse();

		// Set the template query parameter "table" to "stock".
		query.def["table"] = "stock";

		// Notice that we don't give a sixth parameter in these calls,
		// so the default value of "stock" is used.  Also notice that
		// the first row is a UTF-8 encoded Unicode string!  All you
		// have to do to store Unicode data in recent versions of MySQL
		// is use UTF-8 encoding.
		cout << "Populating stock table..." << endl;
		query.execute("Nürnberger Brats", 92, 1.5, 8.79, "2005-03-10");
		query.execute("Pickle Relish", 87, 1.5, 1.75, "1998-09-04");
		query.execute("Hot Mustard", 75, .95, .97, "1998-05-25");
		query.execute("Hotdog Buns", 65, 1.1, 1.1, "1998-04-23");

		cout << (new_db ? "Created" : "Reinitialized") <<
				" sample database successfully." << endl;
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return 1;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		return 1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return 1;
	}

	return 0;
}
