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
	for (int i = 0; i < PIPELINE; i++)
	{
		cmd[i].infd = 0;
		cmd[i].outfd = 1;
	}
	memset(cmdline, 0, sizeof(cmdline));
	memset(avline, 0, sizeof(avline));
	lineptr = cmdline;
	avptr = avline;
	memset(infile, 0, sizeof(infile));
	memset(outfile, 0, sizeof(outfile));
	cmd_count = 0;
	backgnd = 0;
	append = 0;
}
