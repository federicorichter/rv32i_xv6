#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void new_process(int pipe_last[2])
{
	int primes[35];
	char aux_buf[1];
	int n;
	int pipe_next[2];
	int i = 0;
	int p;

	if(pipe(pipe_next) == -1)
	{
		fprintf(2, "Pipe failed\n");
    	exit(1);
	}

	memset(primes, 0, sizeof(primes));

	while(read(pipe_last[0],aux_buf, sizeof(int)) != 0)
	{
		primes[i] = aux_buf[0];
		i++;
	}
	close(pipe_last[0]);

	if(i == 0) //its the last iteration
	{
		close(pipe_next[0]);
		close(pipe_next[1]);
		return;
	}

	n = primes[0];
	printf("%d \n", n);

	for(i = 1;i < 35; i++)
	{
		if(primes[i] % n != 0)
		{
			p = primes[i];
			write(pipe_next[1], &p, sizeof(p));
		}
	}

	close(pipe_next[1]);

	int pid = fork();
	if(pid == 0)
	{
		new_process(pipe_next);
	}


}

int main()
{
	int pipe1[2];

	if(pipe(pipe1) == -1)
	{
		fprintf(2, "Pipe failed\n");
    	exit(1);
	}

	printf("%d \n", 2);

	for(int i = 2;i <= 35; i++)
	{
		if(i % 2 != 0)
		{
			int p = i;
			write(pipe1[1], &p, sizeof(p)); //all even number pass here
		}
	}

	close(pipe1[1]);
	new_process(pipe1);

	wait(0);

	return 0;

}