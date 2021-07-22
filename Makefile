SRCS =			./srcs/Client.cpp \
				./srcs/Server.cpp \
				./srcs/ft_parse_request_1.cpp \
				./srcs/ft_send_response.cpp \
				./srcs/ft_response_to_get.cpp \
				./srcs/ft_response_to_post.cpp \
				./srcs/ft_response_to_delete.cpp \
				./srcs/parser.cpp \
				./srcs/main.cpp

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