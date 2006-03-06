#include <mysql++.h>
#include <custom.h>

#include <string>

// The following is calling a very complex macro which will create
// "struct stock", which has the member variables:
//
//   string item
//   ...
//   Date sdate
//
// plus methods to help populate the class from a MySQL row.  See the
// user manual for further details.
sql_create_5(stock,
	1, 5, // The meaning of these values is covered in the user manual
	std::string, item,
	mysqlpp::longlong, num,
	double, weight,
	double, price,
	mysqlpp::Date, sdate)

