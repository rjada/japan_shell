#ifndef DEF_H
# define DEF_H

# define MAXLINE 1024
# define MAXARG 20
# define PIPELINE 5
# define MAXNAME 100

typedef struct	command
{
	char	*args[MAXARG + 1];
	int		infd;
	int		outfd;
} 				COMMAND;

#endif
