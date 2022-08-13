NAME	=	webserv

SRCS	=	srcs/main.cpp

SRCS_B	= 	\

HEADER	=	./includes/

OBJS	=	$(SRCS:%.c=%.o)

OBJSB	=	$(SRCS_B:%.c=%.o)

CC		=	g++

INC		=	-I includes

CFLAGS	=	-I $(HEADER)

$(NAME)	:	$(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(INC) $(OBJS) -o $(NAME)

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ ${<:.c=.o}

.PHONY	:	all clean fclean re

all		:	$(NAME)

bonus	:	$(OBJSB) $(HEADER)
	$(CC) $(CFLAGS) $(INC) $(OBJSB) -o $(NAME)

clean	:
	@rm -f $(OBJS) $(OBJSB)

fclean	:	clean
	@$(RM) $(NAME)

re		:	fclean all
