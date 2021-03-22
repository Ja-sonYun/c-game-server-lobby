#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "epoll-kqueue-c/epoll.h"

#define MAX_SERVER 10
#define UNPACKING(buf) \

#define NETWORK_FD 0x01
#define PIPT_FD	   0x02
#define MAX_FD	   100
#define ROOM_LIM   50

struct req
{
	uint8_t req;
};

struct res
{

};

typedef struct
{
	uint8_t type;
	int fd;
} fd_t;

struct room
{
	int id;
	char *name;
};

#define REG_FD(_type, _fd) \
	{ \
		fds[ptr_fds].type = _type; \
		fds[ptr_fds].fd = _fd; \
	}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void error_handling(char *buf);
void read_cntl(char buf[BUF_SIZE], int str_len, int fd);

pid_t childs_pid[MAX_SERVER];
int ptr_childs_pid = 0;
fd_t fds[MAX_FD];
int ptr_fds = 0;
struct room rooms[ROOM_LIM];
int ptr_rooms = 0;

int pipes[2];

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	epoll(0, MULPLEX_INIT);
	int	event_cnt;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]); // this port will be the offset of the room's port.
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	REG_FD(NETWORK_FD, serv_sock)
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	epoll(serv_sock, MULPLEX_CREATE);

	for (;;)
	{
		event_cnt = epoll(0, MULPLEX_GET_SIZE); //wait

		for (i=0; i<event_cnt; i++)
		{
			int fd = epoll(i, MULPLEX_GET);
			if (fd == serv_sock)
			{
				adr_sz = sizeof(clnt_adr);
				clnt_sock= accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				REG_FD(NETWORK_FD, clnt_sock);
				epoll(clnt_sock, MULPLEX_CREATE);
				printf("connected client: %d \n", clnt_sock);

			}
			else
			{
				str_len = read(fd, buf, BUF_SIZE);

				read_cntl(buf, str_len, fd);

					/* write(fd, buf, str_len);	// echo! */
			}
		}
	}

	close(serv_sock);
	/* close(epfd); */
	return 0;
}

void read_cntl(char buf[BUF_SIZE], int str_len, int fd)
{
	printf("%c\n", buf[0]);
	if (buf[0] == 'c')
	{
		int pipes[2];
		if (pipe(pipes) != 0)
			error_handling("pipe error");
		printf("pipes, %d, %d", pipes[0], pipes[1]);

		rooms->id = ptr_rooms++;
		childs_pid[ptr_childs_pid] = fork();

		if (childs_pid[ptr_childs_pid] == 0)
		{
			// child processes
			close(pipes[1]);
		}
		else
		{
			// parents process
			close(pipes[0]);
			epoll(fd, MULPLEX_CLOSE);
			close(fd);
			ptr_childs_pid++;
			return;
		}
	}
}

void error_handling(char *buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
