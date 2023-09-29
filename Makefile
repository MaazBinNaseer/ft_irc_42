NAME = ircserv
OBJDIR = obj
SRCS = main.cpp Server.cpp

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

CC = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98 -fsanitize=address

RM = rm -fr

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re
