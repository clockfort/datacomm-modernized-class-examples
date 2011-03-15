// $com CC $@ -o $* -lsocket -lnsl -library=iostream
//
// File:	tcp-server.C
// Author:	K. Reek
// Modified by:	Narayan J. Kulkarni
// Description:	Multithreaded Server, full duplex TCP connection
// Usage:	tcp-server port
//		where 'port' is the well-known port number to use.
//
// This program uses TCP to establish connections with clients and process
// requests.
//

#include <stdlib.h>
#include <unistd.h>
#include <iostream.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread.h>

#define	PORT_MIN	1024
#define	PORT_MAX	65535

/* function prototypes and global variables */
void *do_chld(void*);		// Prototype for the function that handles each connection
int	threadCount;		// Global variable thread count

int main( int ac, char **av ){
	int	s, clilen, newsockfd;		// file descriptor for our socket
	struct	sockaddr_in server, cli_addr; // server's IP address
	thread_t chld_thr;
	int	port;		// server's port number
 	
	threadCount=0;



	//
	// Make sure that the port argument was given
	//
	if( ac < 2 ){
		cerr << "Usage: " << av[ 0 ] << " port" << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Get the port number, and make sure that it is legitimate
	//
	port = atoi( av[ 1 ] );
	if( port < PORT_MIN || port > PORT_MAX ){

		cerr << av[ 1 ] << ": invalid port number" << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Create a stream (TCP) socket.  The protocol argument is left 0 to
	// allow the system to choose an appropriate protocol.
	//
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if( s < 0 ){
		cerr << "socket: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Fill in the addr structure with the port number given by
	// the user.  The OS kernel will fill in the host portion of
	// the address depending on which network interface is used.
	//
	bzero( &server, sizeof( server ) );
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	//
	// Bind this port number to this socket so that other programs can
	// connect to this socket by specifying the port number.
	//
	if( bind( s, (struct sockaddr *)&server, sizeof( server ) ) < 0 ){
		cerr << "bind: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}

	//
	/* set the level of thread concurrency we desire */
	   thr_setconcurrency(2);
	// Make this socket passive, that is, one that awaits connections
	// rather than initiating them.
	//
	if( listen( s, 5 ) < 0 ){
		cerr << "listen: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Now start serving clients
	//
	for(;;){

		cout << "WAITING FOR A CONNECTION ..." << endl;
		clilen = sizeof(cli_addr);
		newsockfd = accept(s, (struct sockaddr *) &cli_addr, &clilen); 

		if(newsockfd < 0)
			fprintf(stderr,"server: accept error\n"), exit(0);

		// Create a thread to handle this connection.
		thr_create(NULL, 0, do_chld, (void *) newsockfd, THR_DETACHED, &chld_thr); 

		/* the server is now free to accept another socket request */
		cout<<"NEW SERVER THREAD CREATED."<<endl;
		threadCount++;
		}
	return(0);
}

void *do_chld(void* arg)
{
int 	mysocfd;
char 	buf [100];
int 	i, len;

	mysocfd=(int)arg;
	//Read buffer from stdio
                len = read( mysocfd, buf, sizeof( buf ) );
                while( len > 0 ){
                        //
                        // Print the message we got
			cout<<"Client sent: "<<buf<<endl;
			//
                        // Send back the message length.
                        //
			cout<<"Sendling length to client "<<len<<endl;
                        if( write( mysocfd, (char*)&len, sizeof(len)) !=sizeof(len)) {
                                cerr << "write: " << strerror( errno ) << endl;
                                exit( EXIT_FAILURE );
                        }

                        //
                        // Read the next message
                        //
                        len = read( mysocfd, buf, sizeof( buf ) );
                }

	/* close the socket and exit this thread */
	close(mysocfd);
	threadCount--;
	cout << "Thread exiting... total # of threads running at this time is "<<threadCount<<endl;
	thr_exit((void *)0);
	return 0;
}
