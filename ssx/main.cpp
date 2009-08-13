/***********************************************************************
 ssx/main.cpp - Main driver module for ssqlsxlat, which does several
 	data translations related to the SSQLSv2 mechanism of MySQL++.  The
	primary one is SSQLSv2 language files (*.ssqls) to C++ source code,
	but there are others.  Run "ssqlsxlat -?" to get a complete list.

 Copyright (c) 2009 by Educational Technology Resources, Inc., and
 (c) 2009 by Warren Young.  Others may also hold copyrights on code
 in this file.  See the CREDITS.txt file in the top directory of the
 distribution for details.

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

#include "parsev2.h"

#include <cmdline.h>

#include <iostream>

using namespace std;
using namespace mysqlpp::ssqlsxlat;


//// parse_ssqls2 //////////////////////////////////////////////////////
// We were given the name of a putative SSQLS v2 source file; try to
// parse it.

static int
parse_ssqls2(const char* file_name)
{
	try {
		ParseV2 p(file_name);
		cout << file_name << " parsed successfully, " <<
				(p.end() - p.begin()) << " interesting lines." << endl;
		return 0;
	}
	catch (const ParseV2::FileException& e) {
		cerr << file_name << ":0" << 
				": file I/O error in SSQLS v2 parse: " <<
				e.what() << endl;
	}
	catch (const ParseV2::ParseException& e) {
		cerr << e.file_name() << ':' << e.line() << ':' <<
				e.what() << endl;
	}
	catch (const std::exception& e) {
		cerr << file_name << ":0" << 
				": critical error in SSQLS v2 parse: " <<
				e.what() << endl;
	}
	return 3;
}


//// main //////////////////////////////////////////////////////////////

int
main(int argc, char* argv[])
{
	// Parse the command line
	CommandLine cmdline(argc, argv);
	if (cmdline) {
		switch (cmdline.input_source()) {
			case CommandLine::is_ssqls2:
				return parse_ssqls2(cmdline.input());

			default:
				cerr << "Sorry, I don't yet know what to do with input "
						"source type " << int(cmdline.input_source()) <<
						'!' << endl;
				return 2;
		}
	}
	else {
		return 1;
	}
}
