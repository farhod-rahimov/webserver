SRCS =			./srcs/Client.cpp \
				./srcs/Server.cpp \
				./srcs/ft_parse_request_1.cpp \
				./srcs/ft_send_response.cpp \
				./srcs/ft_parse.cpp \
<<<<<<< HEAD
				./srcs/cgi.cpp \
=======
				./srcs/ft_read_file.cpp \
				./srcs/ft_response_to_get.cpp \
				./srcs/ft_show_current_dir_files.cpp \
>>>>>>> ab236cabeb92d413a9469bf22c461c5cede8413c
				./srcs/main.cpp

				# ./srcs/ft_response_to_post.cpp
				# ./srcs/ft_response_to_delete.cpp
				

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