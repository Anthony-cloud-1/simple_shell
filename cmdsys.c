#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include <libgen.h>
#include<sys/types.h>
#include<sys/wait.h>
/**
 * executeCommand - execute user cmds
 * @cmdTokens: cmd tokens
 * @programName: to display prog name
 * Return: void
 */
void executeCommand(char **cmdTokens, char *programName)
{
	pid_t childPid = fork();

	if (childPid == -1)
	{
		fprintf(stderr, "%s: Failed forking child process\n", programName);
		return;
	}
	else if (childPid == 0)
	{
		/*
		 * Redirect stderr to stdout in the child process
		 */
		dup2(STDOUT_FILENO, STDERR_FILENO);

		if (cmdTokens[0][0] == '"' && cmdTokens[0][strlen(cmdTokens[0]) - 1] == '"')
		{
			memmove(cmdTokens[0], cmdTokens[0] + 1, strlen(cmdTokens[0]) - 2);
			cmdTokens[0][strlen(cmdTokens[0]) - 2] = '\0';
		}

		if (execvp(cmdTokens[0], cmdTokens) < 0)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", programName, cmdTokens[0]);
		}
		exit(EXIT_FAILURE);
	}
	else
	{
		/*
		 * Wait for the child process to terminate
		 */
		wait(NULL); /*wait(&status)*/
		return;

	}
}
