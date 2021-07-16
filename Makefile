SRCS =		./srcs/Client.cpp

SERVER =	./server.cpp

CLIENT = 	./client_test.cpp

all: 
		@ clang++ -Wall -Wextra -Werror -std=c++98 -c $(SERVER) $(SRCS) && \
		clang++ -Wall -Wextra -Werror -std=c++98 -o server *.o && \
		clang++ -Wall -Wextra -Werror -std=c++98 -c $(CLIENT) && \
		clang++ -Wall -Wextra -Werror -std=c++98 -o client_test client_test.o

clean:
		@ rm *.o server client_test