#include "util.h"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace mysqlpp;

int
main(int argc, char *argv[])
{
	try {
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		Query query = con.query();
		query << "select * from stock";
		cout << "Query: " << query.preview() << endl;

		Result res = query.store();
		cout << "Records Found: " << res.size() << endl << endl;

		cout << "Query Info:\n";
		cout.setf(ios::left);

		for (unsigned int i = 0; i < res.names().size(); i++) {
			cout << setw(2) << i
					// this is the name of the field
					<< setw(15) << res.names(i).c_str()
					// this is the SQL identifier name
					// Result::types(unsigned int) returns a mysql_type_info which in many
					// ways is like type_info except that it has additional sql type
					// information in it. (with one of the methods being sql_name())
					<< setw(15) << res.types(i).sql_name()
					// this is the C++ identifier name which most closely resembles
					// the sql name (its is implementation defined and often not very readable)
					<< setw(20) << res.types(i).name()
					<< endl;
		}

		cout << endl;

		if (res.types(0) == typeid(string)) {
			// this is demonstrating how a mysql_type_info can be 
			// compared with a C++ type_info.
			cout << "Field 'item' is of an SQL type which most "
					"closely resembles\nthe C++ string type\n";
		}

		if (res.types(1) == typeid(longlong)) {
			cout << "Field 'num' is of an SQL type which most "
					"closely resembles\nC++ long long int type\n";
		}
		else if (res.types(1).base_type() == typeid(longlong)) {
			// you have to be careful as if it can be null the actual
			// type is Null<TYPE> not TYPE.  So you should always use
			// the base_type method to get at the underlying type.
			// If the type is not null than this base type would be
			// the same as its type.
			cout << "Field 'num' base type is of an SQL type which "
					"most closely\nresembles the C++ long long int type\n";
		}

		return 0;
	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << endl;
#else
		cerr << "Error: " << er.error << endl;
#endif
		return -1;
	}
	catch (BadConversion& er) {
		// handle bad conversions
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
#else
		cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
			<< er.type_name << "\"." << endl;
#endif
		return -1;
	}
#ifdef USE_STANDARD_EXCEPTION
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
#endif
}

