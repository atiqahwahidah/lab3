#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

int child()
{
	int num;
	printf("Please enter a number: ");
	scanf("%d", &num);
	return num;
}

void sigint(int sig)
{
	printf("Do not press ctrl c\n");
}

int main(void)
{
	int fd[2];
	int s = 2;
	pipe(fd);
	pid_t pid = fork();

	if (signal(SIGINT, sigint) == SIG_ERR)
	{
		perror("signal");
		exit (1);
	}

	if (pid > 0)
	{
		wait(NULL);
		close(0);
		close(fd[1]);
		dup(fd[0]);
		int number = 0;
		read(fd[0], &number, sizeof(number));
		while(s < INT_MAX / 2 && s * s < number)
		{
			if (number % s == 0 || number < 2)
			{
				printf("It is not a prime number\n");
				exit(EXIT_SUCCESS);
			}
			s++;
		}
		printf("This is a prime number\n");
	}
	else if (pid == 0)
        {
		dup(fd[1]);
                int num = child();
                write(fd[1], &num, sizeof(num));
                close(fd[0]);
                close(1);
                exit(EXIT_SUCCESS);
        }

	exit (EXIT_SUCCESS);
}
