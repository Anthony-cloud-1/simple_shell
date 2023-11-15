#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>

int findPipe(char *input, char **pipedCommands);
void splitBySpace(char *input, char **parsedTokens);
int processInput(char *input, char **parsed, char **parsedPipe);
int handleInternalCommands(char **parsedInput);
void executeCommand(char **cmdTokens, char *programName);
void executePipedCommands(char **firstCmd, char **secondCmd);
void shellHelp(void);
int takeInput(char *userInput, FILE *inputFile);

#endif /* SHELL_H */
