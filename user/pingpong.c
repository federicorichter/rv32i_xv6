#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv)
{
	int parent_fd[2];
	int child_fd[2];
	int child;

	char parent;
	char child_char;

	if(pipe(parent_fd) == -1)
	{
		fprintf(2, "Pipe failed\n");
    	exit(1);
	}

	if(pipe(child_fd) == -1)
	{
		fprintf(2, "Pipe failed\n");
    	exit(1);
	}

	child = fork();

	
	if(child == 0)
	{
		child_char = 'c';
		close(parent_fd[1]); 	//close writting end of the pipe
		close(child_fd[0]); 	//close reading end of the pipe

	 	read(parent_fd[0], &parent, 1); //read from parent
		//child process
		printf("%d: received ping \n", getpid());

		close(parent_fd[0]); 	//after reading

		write(child_fd[1], &child_char, 1);
		close(child_fd[1]);
		exit(0);
	}
	else{
		parent = 'p';

		close(parent_fd[0]);
		close(child_fd[1]);

		write(parent_fd[1], &parent, 1);
		close(parent_fd[1]);

		read(child_fd[0], &child_char, 1);
		printf("%d: received pong \n", getpid());

		close(child_fd[0]);

	}

	return 0;

}