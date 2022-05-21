#include "parse.h"
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>
#include "externs.h"

void print_command()
{
	int	i;
	int	j;

	printf("cmd_count = %d\n", cmd_count);
	if (infile[0] != '\0')
		printf("infile = [%s]\n", infile);
	if (outfile[0] != '\0')
		printf("outfile = [%s]\n", outfile);
	i = 0;
	while (i < cmd_count)
	{
		j = 0;
		while (cmd[i].args[j] != NULL)
		{
			printf("[%s] ", cmd[i].args[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

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
	char	*p;

	while (*lineptr == ' '|| *lineptr == '\t')
		lineptr++;
	p = lineptr;
	while (*str != '\0'&& *str == *p)
	{
		str++;
		p++;
	}
	if (*str == '\0')
	{
		lineptr = p;
		return (1);
	}
	return (0);
}

void	getname(char *name)
{
	while (*lineptr == ' ' || *lineptr == '\t')
		lineptr++;

	while (*lineptr != '\0' && *lineptr != ' ' &&
		*lineptr != '\t' && *lineptr != '>' &&
		*lineptr != '<' && *lineptr != '|' &&
		*lineptr != '&' && *lineptr != '\n')
	{
		*name++ = *lineptr++;
	}
	*name = '\0';
}

void	shell_loop(void)
{
	while (1)
	{
		printf("[minishell]$ ");
		init();
		// fflush(stdout);
		if (read_command() == -1)
			break;
		parse_command();
		print_command();
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
	if (check("\n"))
		return (0);
	get_command(0);
	if (check("<"))
		getname(infile);
	int	i;
	for (i = 1; i < PIPELINE; i++)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	if (check(">"))
	{
		if (check(">"))
			append = 1;
		getname(outfile);
	}
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

void	forkexec(COMMAND *pcmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		wait(NULL);
	}
	if (pid == 0)
	{
		if (pcmd->infd != 0)
		{
			close(0);
			dup(pcmd->infd);
		}
		if (pcmd->outfd != 1)
		{
			close(1);
			dup(pcmd->outfd);
		}
		for (int i = 3; i < OPEN_MAX; i++)
			close(i);
		execvp(pcmd->args[0], pcmd->args);
		exit(EXIT_FAILURE);
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
	int	i;
	int	fd;
	int	fds[2];

	for (i = 0; i < cmd_count; i++)
	{
		if (i < cmd_count - 1)
		{
			pipe(fds);
			cmd[i].outfd = fds[1];
			cmd[i + 1].infd = fds[0];
		}
		forkexec(&cmd[i]);
		if ((fd = cmd[i].infd) != 0)
			close(fd);
		if ((fd = cmd[i].outfd) != 1)
			close(fd);
	}
	return (0);
}
