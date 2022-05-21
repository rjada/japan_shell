#include "parse.h"
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "externs.h"

void	get_command(int i)
{
	int	j;
	int	inword;

	j = 0;
	while (*lineptr != '\0')
	{
		while (*lineptr == ' ' || *lineptr == '\t')
			lineptr++;
		cmd[i].args[j] = avptr;
		while (*lineptr != '\0' && *lineptr != ' ' &&
			*lineptr != '\t' && *lineptr != '>' &&
			*lineptr != '<' && *lineptr != '|' &&
			*lineptr != '&' && *lineptr != '\n')
		{
			*avptr++ = *lineptr++;
			inword = 1;
		}
		*avptr++ = '\0';
		switch (*lineptr)
		{
			case ' ':
			case '\t':
				inword = 0;
				j++;
				break;
			case '<':
			case '>':
			case '|':
			case '&':
			case '\n':
				if (inword == 0)
					cmd[i].args[j] = NULL;
				return;
			default:
				return;
		}
	}
}

int	check(const char *str)
{
	return (0);
}

void	getname(char *name)
{
}

void	shell_loop(void)
{
	while (1)
	{
		printf("[minishell]$ ");
		init();
		fflush(stdout);
		if (read_command() == -1)
			break;
		parse_command();
		execute_command();
	}
	printf("\nexit\n");
}

int	read_command(void)
{
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return (-1);
	return (0);
}

int	parse_command(void)
{
	get_command(0);
	if (check("<"))
		getname(infile);
	int	i;
	for (i = 0; i < PIPELINE; i++)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	if (check(">"))
		getname(outfile);
	if (check("&"))
		backgnd = 1;
	if (check("\n"))
	{
		cmd_count = i;
		return (cmd_count);	
	}
	else
	{
		fprintf(stderr, "Command line syntax error\n");
		return (-1);
	}
}

int	execute_command(void)
{
	/*pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		execvp(cmd.args[0], cmd.args);
	wait(NULL);*/
	return (0);
}
