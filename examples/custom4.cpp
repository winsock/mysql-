#include "util.h"

#include <mysql++.h>
#include <custom.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;
using namespace mysqlpp;

sql_create_5(stock,
	1,	// This number is used to make a SSQLS less-than-comparable.
		// If this number is n then if the first n elements are the 
		// same the two SSQLS are the same.  
		// In this case if two two stock's "item" are the same then
		// the two stock are the same.
	5,	// This number should generally be the same as the number of
	 	// elements in the list unless you have a good reason not to.
	string, item,
	longlong, num,
	double, weight,
	double, price,
	Date, sdate)

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

		// here we are storing the elements in a set not a vector.
		set <stock> res;
		query.storein(res);

		cout.setf(ios::left);
		cout << setw(17) << "Item"
			<< setw(4) << "Num"
			<< setw(7) << "Weight"
			<< setw(7) << "Price" << "Date" << endl << endl;

		// Now we we iterate through the set.  Since it is a set the list will
		// naturally be in order.
		set<stock>::iterator i;
		cout.precision(3);
		for (i = res.begin(); i != res.end(); ++i) {
			cout << setw(17) << i->item.c_str()
				<< setw(4) << i->num
				<< setw(7) << i->weight
				<< setw(7) << i->price << i->sdate << endl;
		}

		i = res.find(stock("Hamburger Buns"));
		if (i != res.end()) {
			cout << "Hamburger Buns found.  Currently " << i->num <<
					" in stock.\n";
		}
		else {
			cout << "Sorry no Hamburger Buns found in stock\n";
		}

		// Now we are using the set's find method to find out how many
		// Hamburger Buns are in stock.

		return 0;
	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (BadConversion& er) {
		// handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}

