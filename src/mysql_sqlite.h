#ifndef MY_SQL_SQLITE_H_
#define MY_SQL_SQLITE_H_


#ifdef __cplusplus
extern "C"
{
#endif

typedef void* HDBSQL;
typedef void* HDBCOL;


HDBSQL mydb_connect(const char* server,const char* user,const char* pass,const char* dbname);
void mydb_close(HDBSQL hdb);

int mydb_exec(HDBSQL hdb,const char* str);
HDBCOL mydb_prepare(HDBSQL hdb, const char* str);
int mydb_step(HDBCOL col);
void mydb_free_result(HDBCOL col);

int mydb_column_int(HDBCOL col,int n);
const char* mydb_column_char(HDBCOL col,int n);


#ifdef __cplusplus
}
#endif

#endif /* MY_SQL_SQLITE_H_ */

