all: 
		clang++ -Wall -Wextra -Werror -std=c++98 -c server.cpp && clang++ -Wall -Wextra -Werror -std=c++98 -o server server.o && clang++ -Wall -Wextra -Werror -std=c++98 -c client.cpp && clang++ -Wall -Wextra -Werror -std=c++98 -o client client.o
clean:
		rm *.o server client