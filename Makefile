NAME	= minishell
CC	= gcc
CFLAGS	= -Wall -g
OBJ	= main.o parse.o init.o

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f *.o

fclean : clean
	rm -f $(NAME)

.PHONY : clean fclean
