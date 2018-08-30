#include	<sys/socket.h>	/* basic socket definitions */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<errno.h>
#include	<arpa/inet.h>
#include	<signal.h>

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */

/* Following shortens all the typecasts of pointer arguments: */
#define	SA	struct sockaddr

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Define some port number that can be used for our examples */
#define	SERV_PORT		 6443			/* TCP and UDP */

void	 Close(int);
void	 Listen(int, int);
void	 Bind(int, const SA *, socklen_t);
void	 Connect(int, const SA *, socklen_t);

/* prototypes for our own library wrapper functions */
void	 Inet_pton(int, const char *, void *);

/* prototypes for our stdio wrapper functions: see {Sec errors} */
char	*Fgets(char *, int, FILE *);
void	 Fputs(const char *, FILE *);

int		 Socket(int, int, int);

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);
