#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sched.h>
#include <pthread.h>
#include <wolfssl/ssl.h>
#include <stdlib.h>
#include "unp.h"
#include <dirent.h>
#include <sys/types.h>
#include <getopt.h>
//Global variables shared across threads
struct server_info{
uint32_t ip_int;
char cert_file[255];
int port;
};
struct server_info *host_array;
uint32_t *iparray;
char  (*filename)[255];
int port=443;
int input_flag=0;
//Thread work  loop
void* do_work(unsigned int ipaddress)
{
//wolfSSL variables
//    char msg[]="HTTP/1.1 200OK\r\nServer: wolfSSL Server\r\n";
    WOLFSSL_CTX* ctx;
    int one = 1;
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listen_socket, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(int));
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (input_flag)
    {
	serv_addr.sin_addr.s_addr =  ntohl(host_array[ipaddress].ip_int);
    	serv_addr.sin_port = htons(host_array[ipaddress].port);
    }
    else
    {
	serv_addr.sin_addr.s_addr =  ntohl(iparray[ipaddress]);
    	serv_addr.sin_port = htons(port);

    }
    int ret = bind(listen_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
//Unbinding error can occur when ulimits kernel net variables are not set
    if (ret == -1)
    {
        printf("\nFailed to bind to port\n");
        return NULL;
    }
    listen(listen_socket, 1024);
    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));
    int addr_length = sizeof(cli_addr);
    int cli_sock;

/*    if(input_flag && strlen(host_array[ipaddress].cert_file)< 1)
{
printf("I have come here!! %d\n",strlen(host_array[ipaddress].cert_file));
while(1)
{
	cli_sock = accept(listen_socket, (struct sockaddr *) &cli_addr, (socklen_t *) &addr_length);
        write(cli_sock,"",sizeof(""));
        close(cli_sock);
}
}
else
{*/
//wolfSSL Server context initializations. For TLSv1.2 setup
  if ( (ctx = wolfSSL_CTX_new(wolfSSLv23_server_method())) == NULL){
        printf("wolfSSL_CTX_new error.\n");
        exit(-1);
      }
/*   if ( (ctx = wolfSSL_CTX_new(wolfTLSv1_2_server_method())) == NULL){
        printf("wolfSSL_CTX_new error.\n");
      exit(-1);
    }
*/
//Load Certificate Authority in wolfSSL context
if(input_flag){
    if (wolfSSL_CTX_load_verify_locations(ctx,host_array[ipaddress].cert_file,0) != SSL_SUCCESS) {
                printf("CA Cert Error loading %s"                "please check the file.\n",host_array[ipaddress].cert_file);
}
    printf("\nFile %s loaded successfully",host_array[ipaddress].cert_file);
//Load server certificate into wolfSSL contxt
    if (wolfSSL_CTX_use_certificate_file(ctx,host_array[ipaddress].cert_file,SSL_FILETYPE_PEM) != SSL_SUCCESS) {
        printf( "\nCert Error loading %s "               "please check the file.\n",host_array[ipaddress].cert_file);
        }
// Load server key into wolfSSL context
    if (wolfSSL_CTX_use_PrivateKey_file(ctx,host_array[ipaddress].cert_file,SSL_FILETYPE_PEM) != SSL_SUCCESS) {
          printf( "\nKey Error loading %s "    "please check the file.\n",host_array[ipaddress].cert_file);
        }

}
else
{
    if (wolfSSL_CTX_load_verify_locations(ctx,filename[ipaddress],0) != SSL_SUCCESS) {
		printf("CA Cert Error loading %s"                "please check the file.\n",filename[ipaddress]);
}
    printf("\nFile %s loaded successfully",filename[ipaddress]);
//Load server certificate into wolfSSL contxt
    if (wolfSSL_CTX_use_certificate_file(ctx,filename[ipaddress],SSL_FILETYPE_PEM) != SSL_SUCCESS) {
        printf( "\nCert Error loading %s "               "please check the file.\n",filename[ipaddress]);
        }
// Load server key into wolfSSL context
    if (wolfSSL_CTX_use_PrivateKey_file(ctx,filename[ipaddress],SSL_FILETYPE_PEM) != SSL_SUCCESS) {
          printf( "\nKey Error loading %s "    "please check the file.\n",filename[ipaddress]);
        }
} 
   printf("\nServer setup with SSLv3 - TLSv1.2 support\n");

//SSL socket variables
    WOLFSSL* ssl;
//Infinite loop for accepting client conneections
while(1){
cli_sock = accept(listen_socket, (struct sockaddr *) &cli_addr, (socklen_t *) &addr_length);
//printf("\nConnection from %s, port %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),ntohs(cli_addr.sin_port));
//Initialise SSL session	
       if( (ssl= wolfSSL_new(ctx)) == NULL) {
   	  printf( "wolfSSL_new error.\n");
          break;
       }
//Setup SSL session with listening file descriptor
	wolfSSL_set_fd(ssl, cli_sock);
//Write msg to client connected to SSL session

        wolfSSL_write(ssl, "", sizeof(""));
        //printf("Wrote :%d bytes", wrote);
        //write(cli_sock,msg,sizeof(msg));
//Free the SSL session and close the client socket
//	wolfSSL_shutdown(ssl);
	wolfSSL_free(ssl);
        close(cli_sock);
}

//Free the  SSL Context and close the Server socket
 wolfSSL_CTX_free(ctx);    
 close(listen_socket); 
 //printf("\nI have reached end!!");
 return 0;
}

//Main Init loop of the program
// Usage: <program_name> -p <port_number> -d <directory> -i <ip_adress_in_integer> -s <number_of_hosts>
int main(int argc, const char *argv[])
{
//Main initializations for reading port , directory , ip address and number of hosts
	DIR *dir;
        struct dirent *ent;
        unsigned int i=0,ip_int=0,max_host=0;
        char cert_dir[255];
	int opt;
	static struct option long_options[] = {
        {"port", required_argument, NULL, 'p' },
        {"directory", required_argument, NULL, 'd' },
	{"file",required_argument,NULL,'f'},
        {"ip", required_argument, NULL, 'i' },
        {"server", required_argument, NULL, 's' },
        {"help", no_argument, NULL, 'h' },
        {0, 0, NULL, 0 }
        };
	FILE *fp;
//Loop until all options are read
	printf("A virtual wolfSSL host spawn program for performance testing.\n Author:Suhail Sullad \n In case of any error mail to suhail.sullad@gmail.com\n\n"); 
	while ((opt = getopt_long(argc, argv,"p:d:i:f:s:h",
                   long_options, NULL )) != -1) {
        switch (opt) {
             case 'p' :port=atoi(optarg);//initialize port
                 break;
             case 'd' : if (0 != access(optarg, F_OK)){if(ENOENT == errno){printf("Directory Doesn't Exist!!\n");exit(EXIT_FAILURE);}
			if (ENOTDIR == errno){printf("Path is not a directory!!\n");exit(EXIT_FAILURE);}}
			//printf("\ndirectory selected %s",optarg);
			sprintf(cert_dir,"%s",optarg);//Initialize certificate folder
			printf("\ndirectory selected %s",optarg);
                 break;
	     case 'i' :  ip_int =atoi(optarg);//Initialize ip address in integer format
			printf("this is ip  int %d",ip_int);
		break;
	     case 'f':  if( access( optarg, R_OK ) != -1 ) {
			fp = fopen(optarg,"r");
			input_flag=1;
			printf("\nfile selected %s",optarg);
			}
			 else {
			printf("File doesn't exist!!");
			exit(EXIT_FAILURE);
			}
			break;
             case 's' : max_host=atoi(optarg); // Initialize the number of host  to spawn.
			printf("\nthis is max host : %d",max_host);
			if(max_host<1){printf("No hosts specified!!");exit(EXIT_FAILURE);}
                 	break;
             case 'h' : printf("/n Usage: <program_name> -p <port_number> -d <directory> -i <ip_adress_in_integer> -s <number_of_hosts> /n ");exit(EXIT_FAILURE);
             default: printf("Invalid arguments ");
	              printf("/n Usage: <program_name> -p <port_number> -d <directory> -i <ip_adress_in_integer> -s <number_of_hosts> /n ");
		      exit(EXIT_FAILURE);
        }
}	
//Get number of active processors
int numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);
printf("\nNumber of processors:%d\n",numberOfProcessors);
numberOfProcessors-=1;
//Dynamic memory allocation for  spawning virtual hosts.
	pthread_t tid[max_host];
	//Set per thread attribute
	    pthread_attr_t attr;
	    cpu_set_t cpus;
	    pthread_attr_init(&attr);

	if(input_flag)
    	{
	host_array = malloc(max_host * sizeof(struct server_info));
	char tmp_cert[255];
	for(unsigned int i=0;i<max_host;i++)
	{
		fscanf(fp,"%u,%[^,],%d",&host_array[i].ip_int,&tmp_cert,&host_array[i].port);
		sprintf(host_array[i].cert_file,"%s%s.pem",cert_dir,tmp_cert);
		
	}
	fclose(fp);
	// for(unsigned int i=0;i<max_host;i++)
     // {
//	printf("\n%u - %s - %d\n",host_array[i].ip_int,host_array[i].cert_file,host_array[i].port);
//	}
//	 exit(EXIT_SUCCESS);
	}
	else
	{
	iparray =  malloc(max_host * sizeof(uint32_t));
    	filename = malloc(max_host * 255 * sizeof(char));

//Dynamically loading  .pem certificates under a folder
	if ((dir = opendir (cert_dir)) != NULL) {
		  while ((ent = readdir (dir)) != NULL && i < max_host) {
		   	//printf("Current file :%s%s\n",cert_dir,ent->d_name);
			
			if(strstr( ent->d_name, ".pem") != NULL)
			{
			sprintf (filename[i],"%s%s",cert_dir, ent->d_name);
			i++; 
			}
}
	if(i==0)
	{
	printf("\nFiles not loaded\nExitting program now!!");
	exit(EXIT_FAILURE); 
	}
	  closedir (dir);
	} else {
	 printf("\nCould not open the directory\n");
	 exit(EXIT_FAILURE);
}

//Initializing ip address array
	for(unsigned int i=0;i<max_host;i++)
	iparray[i]=ip_int+i;
//Initializing wolfSSL library
	wolfSSL_Init();
}
//Spawning virtual host threads on multiple cores.
    for ( unsigned int i = 0; i < max_host;)
	{
	 CPU_ZERO(&cpus);
	 CPU_SET(i%numberOfProcessors, &cpus);
	 if(pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus)==0)
	{
	 printf("CPU allocated: %d\n",i%numberOfProcessors);
	 pthread_create(&tid[i], &attr, do_work, i);
	 i++;
	}
	}
//Infinite Sleep
while(1)
sleep(1);

printf("Something happened.I am terminating!!\n");
//Waiting for threads to complete
    for (int i = 0; i < max_host; i++) {
        pthread_join(tid[i], NULL);
    }
//Cleaning wolfSSL library
 wolfSSL_Cleanup();

 printf("shutting down");
    return 0;
}

