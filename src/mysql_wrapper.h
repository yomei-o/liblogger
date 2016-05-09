/*
Copyright (c) 2016, Yomei Otani <yomei.otani@gmai.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

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

