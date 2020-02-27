// main.cpp //////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include "load_ipc.h"
//#include "DateTime.h"

//////////////////////////////////////////////////

int main( int argc, char * * argv )
{
	if ( !IPC_LoadDLL() ) return 1;

	HIPCSERVER hServer = IPC_ServerStart(__TEXT("ipc_test_server"));
	if ( !hServer ) return 1;
	printf("\n    Server started\n");
	printf("\n    Server wait for connection...\n");
	HIPCCONNECTION hConnection1 = IPC_ServerWaitForConnection( hServer, IPC_TIMEOUT_INFINITE,NULL);
	if( hConnection1 == (HIPCCONNECTION)0 || hConnection1 == (HIPCCONNECTION)IPC_RC_TIMEOUT )
	{
		printf( "    Can't connect with Client\n" );
		IPC_ServerStop( hServer );
		IPC_FreeDLL();
		return 1;
	}
	printf("    Client connected!\n");

	//////////////////////////////////////////////
//	bool boRes = IPC_FreeDLL();
//	return 1;


	char * p = new char[ 64 * 1024 ];
	if ( ! p )
	{
		IPC_CloseConnection( hConnection1 );
		IPC_ServerStop( hServer );
		IPC_FreeDLL();
		return 1;
	}

	//////////////////////////////////////////////

// 	HANDLE h = CreateEvent( 0, TRUE, FALSE, 0 );
// 	IPC_SetEvents( hConnection1, &h, 1, 0 );

	DWORD dwTickCount1 =  GetTickCount();
	for ( DWORD i = 0; i <= 300000; i++ )
	{
		DWORD rc = IPC_Recv( hConnection1, p, 64 * 1024, INFINITE );
 		if ( rc == -1 || rc == -2 ) 
			printf( "\nerror\n" );
// 		else
// 			printf( "IPC_Recv Count: %d\n" ,i);

		strcpy( p, "ServerSendData" );
		rc = IPC_Send( hConnection1, p, 64 * 1024, INFINITE );
		if ( rc == -1 || rc == -2 ) 
			printf( "\nerror\n" );
// 		else
// 			printf( "IPC_Send Count: %d\n" ,i);
// 		DWORD dwRes = 0;
// 		BOOL boRes = IPC_GetEvents( hConnection1, &dwRes, 0 );
	}
	DWORD dwTickCount2 = GetTickCount();
	printf("\nTickCount=%ld\n", dwTickCount2 - dwTickCount1);
	float ftime = (dwTickCount2 - dwTickCount1)/1000.0;
	printf("300000 items Recv and Send with 64K data per item \nUse time :%f seconds\n",ftime);

	//////////////////////////////////////////////

	delete[] p;

	BOOL r1 = IPC_CloseConnection( hConnection1 );

	IPC_ServerStop( hServer );
	IPC_FreeDLL();
	getchar();
	return 0;
}	

// eof ///////////////////////////////////////////