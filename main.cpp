/*central hub for project*/

#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;


//trying sql connection with simpler function
//this function diagnoses handles and prints errors
void showSQLError(unsigned int handleType, const SQLHANDLE& handle) {
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];

	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		cout << "SQL driver message: " << message << "\nSQL State: " << SQLState << endl;
}


int main()
{
	
	SQLHANDLE sql_conn_handle = NULL; //connnection handle
	SQLHANDLE sql_stmt_handle = NULL; //statement handle
	SQLHANDLE sql_env_handle = NULL;  //environment handle
	SQLRETURN retCode = 0;
	char SQLQuery[] = "SELECT * FROM app_user";	//query which selects all data from table app_user from database

	do {
		//allocate environment handle for sql
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sql_env_handle))
			break;

		//set attributes that govern environment
		if (SQL_SUCCESS != SQLSetEnvAttr(sql_env_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			break;

		//Allocate server connection
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sql_env_handle, &sql_conn_handle))
			break;

		//set attributes of connection
		if (SQL_SUCCESS != SQLSetConnectAttr(sql_conn_handle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			break;

		//variable for connection string
		SQLCHAR retConString[1024];
		//function connecting to SQL Server
		switch (SQLDriverConnect(sql_conn_handle, NULL, (SQLCHAR*)"DRIVER=(SQL SERVER); SERVER=72.180.160.215, 1433; DATABASE=expTrackerApp; UID=3340project; PWD=expensetracker;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
		
		case SQL_SUCCESS:
			break;

		case SQL_SUCCESS_WITH_INFO:
			break;

		case SQL_NO_DATA_FOUND:
			showSQLError(SQL_HANDLE_DBC, sql_conn_handle);
			retCode = -1;
			break;

		case SQL_INVALID_HANDLE:
			showSQLError(SQL_HANDLE_DBC, sql_conn_handle);
			retCode = -1;
			break;

		case SQL_ERROR:
			showSQLError(SQL_HANDLE_DBC, sql_conn_handle);
			retCode = -1;
			break;

		default:
			break;
		}


		//if an error occurs break do cycle
		if (retCode == -1)
			break;
		 
		//if output is not successful break cycle
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sql_conn_handle, &sql_stmt_handle))
			break;

		if (SQL_SUCCESS != SQLExecDirect(sql_stmt_handle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
			showSQLError(SQL_HANDLE_STMT, sql_stmt_handle);
			break;
		}
		else {
			char username[256];

			//retrieves username from column 1 
			while (SQLFetch(sql_stmt_handle) == SQL_SUCCESS) {
				SQLGetData(sql_stmt_handle, 1, SQL_C_DEFAULT, &username, sizeof(username), NULL);
				cout << username << endl;
			}
		}
			
		

	}while (FALSE);

	//freeing handles and discconnecting from server
	SQLFreeHandle(SQL_HANDLE_STMT, sql_stmt_handle);
	SQLDisconnect(sql_conn_handle);
	SQLFreeHandle(SQL_HANDLE_DBC, sql_conn_handle);
	SQLFreeHandle(SQL_HANDLE_ENV, sql_env_handle);
	

	
	//testing sql server connection and handles
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

	SQLHANDLE sql_conn_handle; //connnection handle
	SQLHANDLE sql_stmt_handle; //statement handle
	SQLHANDLE sql_env_handle;  //environment handle
	SQLCHAR ret_conn_string[SQL_RETURN_CODE_LEN];

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
		(SQLCHAR*)L"DRIVER=(SQL SERVER);SERVER=72.180.160.215, 1433;DATABASE=expTrackerApp;UID=3340project;PWD=expensetracker;",
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
	if (SQL_SUCCESS != SQLExecDirect(sql_stmt_handle, (SQLCHAR*)L"SELECT @@VERSION", SQL_NTS)) {
		cout << "Error querying SQL Server\n";
		goto COMPLETED;
	}
	else {
		//output variable and pointer
		SQLCHAR sqlVersion[SQL_RESULT_LEN];
		SQLLEN ptr_sql_version;

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
	

	getchar();
	return 0;
}