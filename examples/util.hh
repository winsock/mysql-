#ifndef _util_hh_
#define _util_hh_

#include <mysql++.h>

void print_stock_table(Query& query);
void connect_sample_db(int argc, char *argv[], Connection& con,
		const char* kdb = "mysql_cpp_data");

#endif
