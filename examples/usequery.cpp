/***********************************************************************
 usequery.cpp - Same as simple3 example, only with exceptions enabled.
	The end of the result set is signalled differently in this case.

 Copyright (c) 2005-2007 by Educational Technology Resources, Inc.
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

#include "cmdline.h"
#include "printdata.h"

#include <mysql++.h>

#include <iostream>

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

		// Build query to retrieve the entire stock table
		mysqlpp::Query query = con.query("select * from stock");

		// Execute the query, but don't save results in memory
		mysqlpp::ResUse res = query.use();
		if (!res) {
			std::cerr << "Result set is empty!" << std::endl;
			return 1;
		}

		// Iterate through result set, printing each row.
		mysqlpp::Row r;
		while (r = res.fetch_row()) {
			print_stock_row(r);
		}
	}
	catch (const mysqlpp::BadQuery& e) {
		// Something went wrong with the SQL query.
		std::cerr << "Query failed: " << e.what() << std::endl;
		return 1;
	}
	catch (const mysqlpp::EndOfResults&) {
		// Last query result received.  Exit normally.
		return 0;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		std::cerr << "Error: " << er.what() << std::endl;
		return 1;
	}

	// Shouldn't happen!  Program should either error out through one of
	// the "return 1" cases above, or successfully walk off the end of
	// the result set and go through the EndOfResults path above.
	return 2;
}
