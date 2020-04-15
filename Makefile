OBJECT = main.o common.o
NAME = scanner

CC		=	gcc -o
CFLAGS	=	-pthread -W -Wall

$(NAME) : $(OBJECT)
		$(CC) $(NAME) $(CFLAGS) $(OBJECT)

main.o		: common.h
common.o	: common.h

.PHONY	: clean

clean	:
		rm -f $(NAME) $(OBJECT)