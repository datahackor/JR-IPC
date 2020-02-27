// main.cpp //////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "load_ipc.h"

//////////////////////////////////////////////////


int main( int argc, char **argv )
{
	if ( ! IPC_LoadDLL() ) return 1;

	printf("\n    Client started\n");
	printf("\n    Try to connect to server...\n"); 
	HIPCCONNECTION hConnection1 = IPC_Connect( "ipc_test_server", INFINITE );
	if(	hConnection1 == (HIPCCONNECTION)0 || hConnection1 == (HIPCCONNECTION)IPC_RC_TIMEOUT ) {
		printf("    Can't connect to Server\n"); 
		IPC_FreeDLL();
		return 1;
	}
	printf("    Connected to server\n"); 

//	bool boRes = IPC_FreeDLL();
//	return 1;

//	HANDLE h = CreateEvent(NULL, FALSE, FALSE, NULL);

//	IPC_SetEvents(hConnection, &h, 1, NULL);

	char * p = new char[ 64 * 1024 ];
	for ( DWORD i = 0; i <= 300000; i++ )
	{
		strcpy( p, "ClientSendData" );
		DWORD rc = IPC_Send( hConnection1, p, 64 * 1024, INFINITE );
		if( rc == -1 || rc == -2 ) 
			printf( "\nerror\n" );
// 		else
// 			printf( "IPC_Send Count: %d\n" ,i);
		rc = IPC_Recv(hConnection1, p, 64 * 1024, INFINITE);
		if( rc == -1 || rc == -2 ) 
			printf( "\nerror\n" );
// 		else
// 			printf( "IPC_Recv Count: %d\n" ,i);
	}

//	rc = IPC_Send(hConnection, p, 1024, INFINITE);

	BOOL r1 = IPC_CloseConnection( hConnection1 );

	delete[] p;
	IPC_FreeDLL();
	getch();
	return 0;
}

// eof ///////////////////////////////////////////