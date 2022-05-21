#ifndef EXTERNS_H
# define EXTERNS_H

# include "def.h"

extern char	    cmdline[MAXLINE + 1];
extern char	    avline[MAXLINE + 1];
extern char	    *lineptr;
extern char	    *avptr;
extern char	    infile[MAXLINE + 1];
extern char	    outfile[MAXLINE + 1];
extern COMMAND  cmd[PIPELINE];

extern int	cmd_count;
extern int	backgnd;
extern int  append;

#endif
