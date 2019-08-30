#define HTTP_PORT 8080

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_handler.h"

int main( int argc, char *argv[ ] )
{
	int listen_sock;
	int connect_sock;
	int on = 1;
	struct sockaddr_in local_addr;
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof( client_addr );
	
	if ( ( listen_sock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		perror( "Unable to create listening socket" );
		exit( 0 );
	}
	if ( setsockopt( listen_sock, SOL_SOCKET, SO_REUSEADDR,
		&on, sizeof( on ) ) == -1 )
	{
		perror( "Setting socket option" );
		exit( 0 );
	}

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons( HTTP_PORT );
	local_addr.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
	//local_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	if ( bind( listen_sock, ( struct sockaddr * ) &local_addr,
		sizeof( local_addr ) ) == -1 )
	{
		perror( "Unable to bind to local address" );
		exit( 0 );
	}
	if ( listen( listen_sock, 5 ) == -1 )
	{
		perror( "Unable to set socket backlog" );
		exit( 0 );
	}
	while ( ( connect_sock = accept( listen_sock,
		( struct sockaddr * ) &client_addr,
		&client_addr_len ) ) != -1 )
	{
		// TODO: ideally, this would spawn a new thread.
		process_http_request( connect_sock );
	}
	if ( connect_sock == -1 )
	{
		perror( "Unable to accept socket" );
	}
	
	return 0;
}
