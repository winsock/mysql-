#include "util.hh"

#include <mysql++.h>

#include <iostream>

using namespace std;

int main (int argc, char *argv[]) {
  try { // the entire main block is one big try block;
	Connection con(use_exceptions);
	connect_sample_db(argc, argv, con, "");
    
    try {
      con.select_db("mysql_cpp_data");
    } catch (BadQuery&) {
      // if it couldn't connect to the database assume that it doesn't exist
      // and try created it.  If that does not work exit with an error.
      con.create_db("mysql_cpp_data");
      con.select_db("mysql_cpp_data");
    }
    
    Query query = con.query();  // create a new query object
    
    try { // ignore any errors here
          // I hope to make this simpler soon
      query.execute("drop table stock");
    } catch (BadQuery&) {}
    
    query << "create table stock  (item char(20) not null, num bigint,"
	  << "weight double, price double, sdate date)";
    query.execute(RESET_QUERY);
    // send the query to create the table and execute it.  The
    // RESET_QUERY tells the query object to reset it self after
    // execution
    
    query << "insert into %5:table values (%0q, %1q, %2, %3, %4q)";
    query.parse();
    // set up the template query I will use to insert the data.  The
    // parse method call is important as it is what lets the query
    // know that this is a template and not a literal string
    
    query.def["table"] = "stock";
    // This is setting the parameter named table to stock.
    
    query.execute ("Hamburger Buns", 56, 1.25, 1.1, "1998-04-26");
    query.execute ("Hotdogs' Buns"   ,65, 1.1 , 1.1, "1998-04-23");
    query.execute ("Dinner Roles"  , 75,  .95, .97, "1998-05-25");
    query.execute ("White Bread"   , 87, 1.5, 1.75, "1998-09-04");
    // The last parameter "table" is not specified here.  Thus
    // the default value for "table" is used which is "stock".

  } catch (BadQuery &er) { // handle any connection or
                          // query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
    cerr << "Error: " << er.what() << endl;
#else
    cerr << "Error: " << er.error << endl;
#endif
    return -1;
  } catch (BadConversion &er) { // handle bad conversions
#ifdef USE_STANDARD_EXCEPTION
    cerr << "Error: " << er.what() << "\"." << endl
         << "retrieved data size: " << er.retrieved
         << " actual data size: " << er.actual_size << endl;
#else
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
         << er.type_name << "\"." << endl;
#endif
    return -1;
#ifdef USE_STANDARD_EXCEPTION		
  } catch (exception &er) {
    cerr << "Error: " << er.what() << endl;
    return -1;
#endif		
  }
}

