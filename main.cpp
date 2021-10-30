/*central hub for project*/

#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

int main()
{
	/*testing sql server connectionand handles*/
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

	SQLHANDLE sql_conn_handle; //connnection handle
	SQLHANDLE sql_stmt_handle; //statement handle
	SQLHANDLE sql_env_handle;  //environment handle
	SQLWCHAR ret_conn_string[SQL_RETURN_CODE_LEN];

	sql_conn_handle = NULL;
	sql_stmt_handle = NULL;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sql_env_handle))
		goto COMPLETED;

	if (SQL_SUCCESS != SQLSetEnvAttr(sql_env_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		goto COMPLETED;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sql_env_handle, &sql_conn_handle))
		goto COMPLETED;

	cout << "Connecting to SQL Server...\n";

	//connecting to server
	switch (SQLDriverConnect(sql_conn_handle, NULL,
		(SQLWCHAR*)L"DRIVER=(SQL SERVER);SERVER=72.180.160.215, 1433;DATABASE=expTrackerApp;UID=3340project;PWD=expensetracker;",
		SQL_NTS, ret_conn_string, 1024, NULL, SQL_DRIVER_NOPROMPT)) {

	case SQL_SUCCESS:
		cout << "Connction to SQL Server successful\n";
		break;
		
	case SQL_SUCCESS_WITH_INFO:
		cout << "Connection to SQL Server successful\n";
		break;

	case SQL_INVALID_HANDLE:
		cout << "Unable to connect to SQL Server\n";
		goto COMPLETED;

	case SQL_ERROR:
		cout << "Unable to conect to SQL Server\n";
		goto COMPLETED;

	default:
		break;
	}

	//if cannot connect to server then exit
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sql_conn_handle, &sql_stmt_handle))
		goto COMPLETED;

	cout << "\n\n";

	//exit if query execution fails
	//else display query result
	if (SQL_SUCCESS != SQLExecDirect(sql_stmt_handle, (SQLWCHAR*)L"SELECT @@VERSION", SQL_NTS)) {
		cout << "Error querying SQL Server\n";
		goto COMPLETED;
	}
	else {
		//output variable and pointer
		SQLCHAR sqlVersion[SQL_RESULT_LEN];
		SQLINTEGER ptr_sql_version;

		while (SQLFetch(sql_stmt_handle) == SQL_SUCCESS) {

			SQLGetData(sql_stmt_handle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptr_sql_version);

			//display query result
			cout << "\n Query Result: \n\n";
			cout << sqlVersion << endl;
		}
	}

	//close connection and free resources
COMPLETED:
	SQLFreeHandle(SQL_HANDLE_STMT, sql_stmt_handle);
	SQLDisconnect(sql_conn_handle);
	SQLFreeHandle(SQL_HANDLE_DBC, sql_conn_handle);
	SQLFreeHandle(SQL_HANDLE_ENV, sql_env_handle);


	return 0;
}