/***************************************************************************************************
*   GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.
****************************************************************************************************/
#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sched.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <getopt.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
//Global variables shared across threads
uint32_t *iparray;
char  (*filename)[255];
int port=443;
//Thread work  loop
void* do_work(unsigned int ipaddress)
{
//OpenSSL variables
    SSL_CTX* ctx;
    int one = 1;
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =  ntohl(iparray[ipaddress]);
    serv_addr.sin_port = htons(port);
    int ret = bind(listen_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
//OpenSSL Server context initializations. For TLSv1.2 setup

   if ( (ctx = SSL_CTX_new(SSLv3_server_method())) == NULL){
        printf("OpenSSL_CTX_new error.\n");
        exit(-1);
      }

//Unbinding error can occur when ulimits kernel net variables are not set
    if (ret == -1)
    {
        printf("Failed to bind to port\n");
        return NULL;
    }
//Load Certificate Authority in OpenSSL context
/*    if (SSL_CTX_load_verify_locations(ctx,filename[ipaddress],0) <= 0) {
		printf("CA Cert Error loading %s"                "please check the file.\n",filename[ipaddress]);
}*/
    printf("\nFile %s loaded successfully",filename[ipaddress]);
//Load server certificate into OpenSSL contxt
    if (SSL_CTX_use_certificate_file(ctx,filename[ipaddress],SSL_FILETYPE_PEM) <= 0) {
        printf( "\nCert Error loading %s "               "please check the file.\n",filename[ipaddress]);
        }
// Load server key into OpenSSL context
    if (SSL_CTX_use_PrivateKey_file(ctx,filename[ipaddress],SSL_FILETYPE_PEM) <= 0) {
          printf( "\nKey Error loading %s "    "please check the file.\n",filename[ipaddress]);
        }

//Listen only 1 open connections per thread socket.
    listen(listen_socket, 1);
    printf("\nServer setup with SSLv3 - TLSv1.2 support\n");
//SSL socket variables
    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));
    int addr_length = sizeof(cli_addr);
    SSL* ssl;
    int cli_sock;
//    char buff[512];
//Infinite loop for accepting client conneections
while(1){
 if((cli_sock = accept(listen_socket, (struct sockaddr *) &cli_addr, (socklen_t *) &addr_length))<0)
{     if (errno == EINTR)
         continue;               /* back to while() */
      else
      printf("accept error\n");
}
//printf("\nConnection from %s, port %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),ntohs(cli_addr.sin_port));
//Initialise SSL session	
       if( (ssl= SSL_new(ctx)) == NULL) {
   	  printf( "OpenSSL_new error.\n");
          break;
       }
//Setup SSL session with listening file descriptor
	SSL_set_fd(ssl, cli_sock);
//Write msg to client connected to SSL session
	SSL_accept(ssl);
        SSL_write(ssl, "", sizeof(""));
//Free the SSL session and close the client socket
	SSL_free(ssl);
        close(cli_sock);
}

//Free the  SSL Context and close the Server socket
 SSL_CTX_free(ctx);    
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
        {"ip", required_argument, NULL, 'i' },
        {"server", required_argument, NULL, 's' },
        {"help", no_argument, NULL, 'h' },
        {0, 0, NULL, 0 }
        };
//Loop until all options are read
	printf("A virtual OpenSSL host spawn program for performance testing.\n Author:Suhail Sullad \n In case of any error mail to suhail.sullad@gmail.com\n\n"); 
	while ((opt = getopt_long(argc, argv,"p:d:i:s:h",
                   long_options, NULL )) != -1) {
        switch (opt) {
             case 'p' :port=atoi(optarg);//initialize port
                 break;
             case 'd' : if (0 != access(optarg, F_OK)){if(ENOENT == errno){printf("Directory Doesn't Exist!!\n");exit(EXIT_FAILURE);}
			if (ENOTDIR == errno){printf("Path is not a directory!!\n");exit(EXIT_FAILURE);}}
			//printf("\ndirectory selected %s",optarg);
			sprintf(cert_dir,"%s",optarg);//Initialize certificate folder
			//printf("\ndirectory selected %s",optarg);
                 break;
	     case 'i' :  ip_int =atoi(optarg);//Initialize ip address in integer format
		break;
             case 's' : max_host=atoi(optarg); // Initialize the number of host  to spawn.
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
printf("Number of processors:%d\n",numberOfProcessors);
//Dynamic memory allocation for  spawning virtual hosts.
	pthread_t tid[max_host];
	iparray =  malloc(max_host * sizeof(uint32_t));
    	filename = malloc(max_host * 255 * sizeof(char));
//Set per thread attribute	
    pthread_attr_t attr;
    cpu_set_t cpus;
    pthread_attr_init(&attr);

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
//Initializing OpenSSL library
	 SSL_library_init();   
    
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
sleep(5);

printf("Something happened.I am terminating!!\n");
//Waiting for threads to complete
    for (int i = 0; i < max_host; i++) {
        pthread_join(tid[i], NULL);
    }

 printf("shutting down");
    return 0;
}

