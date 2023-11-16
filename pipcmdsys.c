#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include <libgen.h>
#include<sys/types.h>
#include<sys/wait.h>

/**
 * executePipedCommands - execute piped cmds
 * @firstCmd:first command
 * @secondCmd: second command
 *
 * Return: Always 0
 */
void executePipedCommands(char **firstCmd, char **secondCmd);

void executePipedCommands(char **firstCmd, char **secondCmd)
{
	int pipefd[2];
	pid_t child1, child2;

	if (pipe(pipefd) < 0)
	{
		fprintf(stderr, "%s: Pipe initialization failed\n", basename(firstCmd[0]));
		return;
	}
	child1 = fork();
	if (child1 < 0)
	{
		fprintf(stderr, "%s: Forking child process failed\n", basename(firstCmd[0]));
		return;
	}

	if (child1 == 0)
	{
		/*
		 * Child 1 executing the first command
		 *It only needs to write at the write end
		 */
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(firstCmd[0], firstCmd) < 0)
		{
			fprintf(stderr, "%s: Failed to execute\n", basename(firstCmd[0]));
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/*
		 * Parent executing
		 */
		child2 = fork();

		if (child2 < 0)
		{
			fprintf(stderr, "%s: Forking process failed\n", basename(firstCmd[0]));
			return;
		}

		/*
		 * Child 2 executing the second command
		 *It only needs to read at the read end
		 */
		if (child2 == 0)
		{
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(secondCmd[0], secondCmd) < 0)
			{
				fprintf(stderr, "%s: Failed to execute\n", basename(secondCmd[0]));
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/*
			 * Parent executing, waiting for two children
			 */
			close(pipefd[0]);
			close(pipefd[1]);
			wait(NULL);
			wait(NULL);
		}
	}
}
