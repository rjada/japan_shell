#include "init.h"
#include "externs.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

void	sigint_handler(int sig)
{
	printf("\n[minishell]$ ");
	fflush(stdout);
}

void	setup(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_handler);
}

void	init(void)
{
	memset(cmd, 0, sizeof(cmd));
	memset(&cmdline, 0, sizeof(cmdline));
	lineptr = cmdline;
	avptr = avline;
}
