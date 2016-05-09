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