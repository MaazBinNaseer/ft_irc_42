NAME = ircserv

SRCDIR = src
SRCS = main.cpp Server.cpp Client.cpp Commands.cpp Parse.cpp Channel.cpp

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

.SILENT:

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

$(NAME): $(OBJS)
	c++ -Wall -Wextra -Werror -std=c++98 $(OBJS) -o $(NAME)
	printf "\x1B[32m$(NAME) ready\x1B[0m\n";

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

gpush:
	git add .
	git commit -m sub
	git push

re: fclean all
