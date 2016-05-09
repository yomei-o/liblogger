//#define I_USE_MYSQL
#define I_USE_MYSQL_WRAPPER
#define I_USE_SQLITE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef I_USE_MYSQL
#ifdef I_USE_MYSQL_WRAPPER
#include "mysql_wrapper.h"
#else
#include <mysql/mysql.h>
#endif
#endif

#ifdef I_USE_SQLITE
#include "sqlite3.h"
#endif 

#include "mysql_sqlite.h"




HDBSQL mydb_connect(const char* server, const char* user, const char* pass, const char* dbname)
{
	HDBSQL ret = NULL;
	if (server == NULL || user == NULL || pass == NULL || dbname == NULL)goto end;
#ifdef I_USE_MYSQL
	MYSQL *conn = NULL;
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, pass, dbname, 0, NULL, 0)) {
		goto end;
	}
	ret = (HDBSQL)conn;
#endif
#ifdef I_USE_SQLITE
	struct sqlite3 *conn = NULL;
	if(SQLITE_OK != sqlite3_open("db_test", &conn)){
		goto end;
	}
	ret = (HDBSQL)conn;
#endif
end:
	return ret;
}
void mydb_close(HDBSQL hdb)
{
	if (hdb == NULL)return;
#ifdef I_USE_MYSQL
	mysql_close((MYSQL*)hdb);
#endif
#ifdef I_USE_SQLITE
	sqlite3_close((sqlite3*)hdb);
#endif
}

int mydb_exec(HDBSQL hdb, const char* str)
{
	int ret = -1;
	if (hdb == NULL || str==NULL)return ret;
#ifdef I_USE_MYSQL
	if (mysql_query((MYSQL*)hdb, str)){
		//error
		ret = -1;
	}else {
		//ok
		ret = 0;
	}
#endif
#ifdef I_USE_SQLITE
	if (SQLITE_OK != sqlite3_exec((sqlite3*)hdb, str, NULL, NULL, NULL))
	{
		ret = -1;
	}else {
		ret = 0;
	}

#endif
	return ret;
}


struct hdb {
	void* resp;
	void* row;
};

HDBCOL mydb_prepare(HDBSQL hdb, const char* str)
{
	HDBCOL ret = NULL;
	if (hdb == NULL || str == NULL)goto end;

#ifdef I_USE_MYSQL
	ret = (HDBCOL)malloc(sizeof(struct hdb));
	if (ret == NULL)goto end;
	memset(ret, 0, sizeof(struct hdb));

	if (mysql_query((MYSQL*)hdb, str)) {
		//error
		free(ret);
		goto end;
	}
	((struct hdb*)ret)->resp=(HDBCOL)mysql_use_result((MYSQL*)hdb);
#endif
#ifdef I_USE_SQLITE
	if (SQLITE_OK != sqlite3_prepare((sqlite3*)hdb, str, strlen(str), (sqlite3_stmt**)(&ret), NULL))
	{
		ret = NULL;
	}
#endif
end:
	return ret;
}

void mydb_free_result(HDBCOL col)
{
	if (col == NULL)return;
#ifdef I_USE_MYSQL
	mysql_free_result((MYSQL_RES*)(((struct hdb*)col)->resp));
	free(col);
#endif
#ifdef I_USE_SQLITE
	sqlite3_finalize((sqlite3_stmt*)col);
#endif
}

int mydb_step(HDBCOL col)
{
	int ret = 0;
	if (col == NULL)return ret;
#ifdef I_USE_MYSQL
	((struct hdb*)col)->row= mysql_fetch_row((MYSQL_RES*)(((struct hdb*)col)->resp));
	if (((struct hdb*)col)->row == NULL)return ret;
	ret = 1;
#endif
#ifdef I_USE_SQLITE
	if (SQLITE_ROW == sqlite3_step((sqlite3_stmt*)col))ret = 1;
#endif
	return ret;
}

int mydb_column_int(HDBCOL col,int n)
{
	int ret = 0;
	const char* p = NULL;
	if (col == NULL)return ret;
#ifdef I_USE_MYSQL
	if (((struct hdb*)col)->row == NULL)return ret;
	p=((MYSQL_ROW)(((struct hdb*)col)->row))[n];
	if (p == NULL)p = "0";
	ret = atoi(p);
#endif
#ifdef I_USE_SQLITE
	ret= sqlite3_column_int((sqlite3_stmt*)col, n);
#endif
	return ret;

}
const char* mydb_column_char(HDBCOL col,int n)
{
	const char* ret = NULL;
#ifdef I_USE_MYSQL
	if (((struct hdb*)col)->row == NULL)return ret;
	if (((struct hdb*)col)->row == NULL)return ret;
	ret = ((MYSQL_ROW)(((struct hdb*)col)->row))[n];
#endif
#ifdef I_USE_SQLITE
	ret = sqlite3_column_text((sqlite3_stmt*)col, n);
#endif
	return ret;
}

#if 0
int main(void) {
	MYSQL *conn = NULL;
	MYSQL_RES *resp = NULL;
	MYSQL_ROW row;
	char sql_str[255];
	char *sql_serv = "localhost";
	char *user = "root";
	char *passwd = "wanted";
	char *db_name = "db_test";

	memset(&sql_str[0], 0x00, sizeof(sql_str));

	// mysql接続
	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, sql_serv, user, passwd, db_name, 0, NULL, 0)) {
		// error
		exit(-1);
	}

	// クエリ実行
	snprintf(&sql_str[0], sizeof(sql_str) - 1, "select * from tb_test");
	if (mysql_query(conn, &sql_str[0])) {
		// error
		mysql_close(conn);
		exit(-1);
	}

	// レスポンス
	resp = mysql_use_result(conn);

	while ((row = mysql_fetch_row(resp)) != NULL) {
		printf("%d : %s\n", atoi(row[0]), row[1]);
	}

	// 後片づけ
	mysql_free_result(resp);
	mysql_close(conn);
	return 0;
}

#endif
