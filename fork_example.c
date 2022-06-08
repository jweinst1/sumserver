#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


static void print_my_pid() {
	printf("I am %d\n", getpid());
}

int main()
{
    print_my_pid(); // one process is active
    fork();
    print_my_pid(); 
    fork();
    print_my_pid(); 
    fork();
    print_my_pid(); 
    return 0;
}
