SRCS =			./srcs/Client.cpp \
				./ft_parse_request_1.cpp \
				./ft_create_response.cpp \
				./server.cpp

OBJS =			$(SRCS:.cpp=.o)

CC = 			clang++

FLAGS =			-Wall -Wextra -Werror -std=c++98

NAME =			server

%.o:			%.cpp
				@ $(CC) -c $(FLAGS) $< -o $(<:.cpp=.o)

all: 			$(NAME)

$(NAME):		$(OBJS)
				@ $(CC) -o $(NAME) $(OBJS) 
				@ echo 'server built, run ./server'

clean:
				@ rm -f $(OBJS)

fclean:			clean
				@ rm -f $(NAME)

re:				fclean all

.PHONY:			all clean fclean re