#ifndef __connection1_hh__
#define __connection1_hh__

#include <platform.h>

#include <exceptions.h>
#include <query.h>
#include <result.h>

#include <mysql.h>

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>

#ifdef HAVE_EXT_SLIST
#  include <ext/slist>
#else
#  ifdef HAVE_STD_SLIST
#      include <slist>
#  endif
#endif

namespace mysqlpp {

class MysqlChild {
public:
  virtual void parent_leaving() = 0;
  virtual ~MysqlChild() {}
};

//: The main database handle
class Connection {
  friend class ResNSel;
  friend class ResUse;
  friend class Query;
  
private:
  bool throw_exceptions;
  MYSQL mysql;
  bool is_connected;
  bool locked;
  bool Success;
	
public: 
  Connection () : throw_exceptions(true), locked(false) 
    {mysql_init(&mysql);} //:
  Connection (bool te) : throw_exceptions(te), is_connected(false), locked(true), Success(false) //:
    {mysql_init(&mysql);} //:
  Connection (const char *db, const char *host = "", const char *user = "", 
	      const char *passwd = "", bool te = true); //:
  Connection (const char *db, const char *host, const char *user, 
	      const char *passwd, uint port, my_bool compress = 0,
	      unsigned int connect_timeout = 60, bool te = true,
	      cchar *socket_name = "",unsigned int client_flag=0); //:

  bool   real_connect (cchar *db = "", cchar *host = "", 
		       cchar *user = "", cchar *passwd = "", uint port = 0,
		       my_bool compress = 0, unsigned int connect_timeout = 60,
		       cchar *socket_name= "", unsigned int client_flag=0); //:
				
  ~Connection (); //:
  void         close() {mysql_close(&mysql);}	 //:
  std::string       info ();	//:

  bool   connected() const {return is_connected;}
  //: returns true if a successful connection was made

  bool   success() {return Success;}
  //: returns true of the last query was successful

  bool   connect (cchar *db = "", cchar *host = "", 
		  cchar *user = "", cchar *passwd = "");
  //:

  bool   lock() {if (locked) return true; locked = true; return false;}
  void   unlock() {locked = false;}

  void purge (void) {mysql_close(&mysql); }
  //:

  Query query();
  //:

  operator bool () {return success();}                  //: returns success()
  const char* error () {return mysql_error(&mysql);} //: last error message()
	int errnum () {return mysql_errno(&mysql);}
	int   refresh (unsigned int refresh_options){ return mysql_refresh (&mysql,refresh_options); }
	int ping (void) { return mysql_ping(&mysql);}
	int kill (unsigned long pid) { return mysql_kill (&mysql,pid);}
  std::string client_info () {return std::string(mysql_get_client_info());} //:
  
  std::string host_info () {return std::string(mysql_get_host_info(&mysql));} //:
  
  int    proto_info () {return mysql_get_proto_info(&mysql);} //:
  
  std::string server_info () {return std::string(mysql_get_server_info(&mysql));} //:
 
  std::string stat() {return std::string(mysql_stat(&mysql));} //:
 
  Result  store(const std::string &str) {return store(str, throw_exceptions);} //:
  ResUse  use(const std::string &str)   {return use(str, throw_exceptions);} //:
  ResNSel execute(const std::string &str) {return execute(str, throw_exceptions);} //:
	bool exec (const std::string &str);
  Result  store(const std::string &str, bool te); //:
  ResUse  use(const std::string &str, bool te); //:
  ResNSel execute(const std::string &str, bool te); //:
 
  bool   create_db (std::string db) {return !(execute( "CREATE DATABASE " + db ));} //:
  bool   drop_db (std::string db) {return !(execute( "DROP DATABASE " + db ));} //:
  bool   select_db (std::string db) {return select_db(db.c_str());} //:
  bool   select_db (const char *db); //:
  bool   reload(); //:
  bool   shutdown (); //:
	std::string infoo (void) {return info ();}
	st_mysql_options get_options (void) const {return mysql.options;}
	int read_options(enum mysql_option option,const char *arg) {return  mysql_options(&mysql, option,arg);}
  my_ulonglong affected_rows()  {return mysql_affected_rows((MYSQL*) &mysql);}
  my_ulonglong insert_id () {return mysql_insert_id(&mysql);}

  template <class Sequence>
  void storein_sequence(Sequence &, const std::string &); //:
  template <class Set>
  void storein_set(Set &, const std::string &);  //:

  //!dummy: void storein(TYPE &con, const string &s);
  //: Stores the results in TYPE.  
  // Stores the result in TYPE. TYPE must be some sort of STL container.  

  template <class T>
  void storein(std::vector<T> &con, const std::string &s)
    {storein_sequence(con,s);}

  template <class T>
  void storein(std::deque<T> &con, const std::string &s)
     {storein_sequence(con,s);}

  template <class T>
  void storein(std::list<T> &con, const std::string &s)
    {storein_sequence(con,s);}

#if defined(HAVE_EXT_SLIST)
  template <class T>        void storein(__gnu_cxx::slist<T> &con, const std::string &s)
    {storein_sequence(con,s);}
#elif defined(HAVE_STD_SLIST)
  template <class T>        void storein(slist<T> &con, const std::string &s)
    {storein_sequence(con,s);}
#endif

  template <class T>
  void storein(std::set<T> &con, const std::string &s)
    {storein_set(con,s);}
	
  template <class T>
  void storein(std::multiset<T> &con, const std::string &s)
    {storein_set(con,s);}
};

//! with_class = Connection 


template <class Sequence>
void Connection::storein_sequence (Sequence &seq, const std::string &str) {
  ResUse result = use(str);
  while (1) {
	  MYSQL_ROW d = mysql_fetch_row(result.mysql_result());
		if (!d) break;
	  Row row(d,&result,(unsigned int
	  *)mysql_fetch_lengths(result.mysql_result()),true);
		if (!row) break;
    seq.push_back(typename Sequence::value_type(row));
	}
}

template <class Set>
void Connection::storein_set (Set &sett, const std::string &str) {
  ResUse result = use(str);
	while (1) {
	  MYSQL_ROW d = mysql_fetch_row(result.mysql_result());	
		if (!d) return;
	  Row row(d, &result,
	  	(unsigned int *)mysql_fetch_lengths(result.mysql_result()),true);
	  if (!row) break;
    sett.insert(typename Set::value_type(row));
	}
}

}; // end namespace mysqlpp

#endif

