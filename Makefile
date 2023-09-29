NAME = ircserv

SRCS = main.cpp Server.cpp

OBJS = $(SRCS:.cpp=.o)

CC = c++

CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98 -fsanitize=address

RM = rm -fr

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re