#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <execinfo.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

static const char* BASIC_REPLY = "Hello!";

// Set this so only child process does specific actions
static int parent_pid = 0;

static void set_parent_pid() {
	parent_pid = getpid();
}

static int is_in_child() {
	return getpid() != parent_pid;
}


void segv_handler(int sig) {
	(void)sig;
   void *array[10];
   size_t size;
   size = backtrace(array, 10);
   fputs("CRASH:\n  signal SEGV\n", stderr);
   fputs("backtrace:\n", stderr);
   backtrace_symbols_fd(array, size, STDERR_FILENO);
   exit(2);
}

void abrt_handler(int sig) {
	(void)sig;
   void *array[10];
   size_t size;
   size = backtrace(array, 10);
   fputs("CRASH:\n  signal ABRT\n", stderr);
   fputs("backtrace:\n", stderr);
   backtrace_symbols_fd(array, size, STDERR_FILENO);
   exit(1);
}


void inter_handler(int sig) {
	(void)sig;
	puts("Shutting Down!");
	exit(0);
}

static void register_signal_handlers() {
   signal(SIGSEGV, segv_handler);
   signal(SIGABRT, abrt_handler);
   signal(SIGINT, inter_handler);
}

static void do_handle(int connfd) {
	if (!is_in_child()) {
		return;
	}
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(connfd, (struct sockaddr *)&addr, &addr_size);
    char clientip[20] = {0};
    strcpy(clientip, inet_ntoa(addr.sin_addr));
    printf("Got a connection from %s\n", clientip);
    write(connfd, BASIC_REPLY, strlen(BASIC_REPLY));
	close(connfd);
	exit(0);
}

static void do_serve(long port) {
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        fork();
        do_handle(connfd);
     }
}

int main(int argc, char const *argv[])
{
	puts("Starting up Sum Server");
	if (argc != 2) {
		fprintf(stderr, "Expected listening port, got %d arguments\n", argc);
		exit(7);
	}
	set_parent_pid();
	register_signal_handlers();
	long parsed_port = 0;
	parsed_port = strtol(argv[1], NULL, 10);
	if (parsed_port == 0) {
		fprintf(stderr, "Unable to parse listening port from arg '%s'\n", argv[1]);
		abort();
	} else {
		printf("Will listen on port %ld\n", parsed_port);
	}
	do_serve(parsed_port);
	return 0;
}