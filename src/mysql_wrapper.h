#ifndef MY_SQL_WRAPPER_H_
#define MY_SQL_WRAPPER_H_

#include <mysql/mysql.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern MYSQL * (STDCALL *pf_mysql_init)(MYSQL *mysql);
extern void (STDCALL *pf_mysql_close)(MYSQL *sock);
extern MYSQL * (STDCALL *pf_mysql_real_connect)(MYSQL *mysql, const char *host,
	const char *user,const char *passwd,const char *db,
	unsigned int port,const char *unix_socket,unsigned long clientflag);
extern int (STDCALL *pf_mysql_query)(MYSQL *mysql, const char *q);
extern MYSQL_RES * (STDCALL *pf_mysql_use_result)(MYSQL *mysql);
extern void (STDCALL *pf_mysql_free_result)(MYSQL_RES *result);
extern MYSQL_ROW(STDCALL *pf_mysql_fetch_row)(MYSQL_RES *result);


#ifdef __cplusplus
}
#endif

#ifndef MY_SQL_WRAOPPER_C

#define mysql_init pf_mysql_init
#define mysql_close pf_mysql_close
#define mysql_real_connect pf_mysql_real_connect
#define mysql_query pf_mysql_query
#define mysql_use_result pf_mysql_use_result
#define mysql_free_result pf_mysql_free_result
#define mysql_fetch_row pf_mysql_fetch_row

#endif /* MY_SQL_WRAOPPER_C */

#endif /* MY_SQL_WRAPPER_H_ */

