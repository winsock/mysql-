#include <mysql++.h>
#include <custom.h>

#include <string>

// The following is calling a very complex macro which will create
// "struct stock", which has the member variables:
//
//   sql_char item;
//   ...
//   sql_date sdate;
//
// plus methods to help populate the class from a MySQL row.  See the
// SSQLS sections in the user manual for further details.
sql_create_5(stock,
	1, 5, // The meaning of these values is covered in the user manual
	mysqlpp::sql_char, item,
	mysqlpp::sql_bigint, num,
	mysqlpp::sql_double, weight,
	mysqlpp::sql_double, price,
	mysqlpp::sql_date, sdate)

