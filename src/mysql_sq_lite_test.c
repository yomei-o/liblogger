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

#define I_USE_MYSQL_WRAPPER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysql_sqlite.h"


int main(void) {
	HDBSQL conn = NULL;
	HDBCOL resp = NULL;

	char sql_str[255];
	char *sql_serv = "localhost";
	char *user = "root";
	char *passwd = "wanted";
	char *db_name = "db_test";


	conn = mydb_connect(sql_serv,user,passwd,db_name);
	if (conn == NULL) {
		printf("mydb_connect error\n");
		exit(-1);
	}

	memset(&sql_str[0], 0x00, sizeof(sql_str));
	sprintf(&sql_str[0], "select * from tb_test");
	resp = mydb_prepare(conn, sql_str);
	if (resp==NULL) {
		mydb_close(conn);
		exit(-1);
	}

	while (mydb_step(resp)){
		printf("%d : %s\n", mydb_column_int(resp,0),mydb_column_char(resp,1));
	}
	mydb_free_result(resp);
	mydb_close(conn);

	return 0;
}