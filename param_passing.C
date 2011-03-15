
// Sample code to how how to pass parameters to thread handling routine
//

struct param{
	int socket_id;
	int copy;
	sockaddr_in* client;
};


// In the main program.....

            /* create a new thread to process the incomming request */
            param1.socket_id=newsockfd;
            param1.copy = copy1;
            param1.client=&cli_addr;
            thr_create(NULL, 0, do_chld, (void *) &param1, THR_DETACHED, &chld_thr);    
	....
	....
}

// In the thread handling routine.....

void *do_chld(void* arg)
{

      ns = ((param*)(int)arg)->socket_id;
      copy = ((param*)(int)arg)->copy;
      bzero( &client_addr, sizeof( client_addr ) );
      client_addr.sin_family = (((param*)(int)arg)->client)->sin_family;
      client_addr.sin_port =(((param*)(int)arg)->client)->sin_port; 
      client_addr.sin_addr.s_addr = (((param*)(int)arg)->client)->sin_addr.s_addr;
      

}
