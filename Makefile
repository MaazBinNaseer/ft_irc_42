NAME = ircserv

SRCDIR = src
SRCS = main.cpp Server.cpp ServerFuncs.cpp \
		Channel.cpp ChannelFuncs.cpp \
		Client.cpp Courier.cpp \
		Parse.cpp Commands.cpp
		

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

.SILENT:

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	c++ -Wall -Wextra -Werror -g3 -std=c++98 -c $< -o $@

$(NAME): $(OBJS)
	c++ -Wall -Wextra -Werror -std=c++98 $(OBJS) -o $(NAME)
	printf "\x1B[32m$(NAME) ready\x1B[0m\n";

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

gpush:
	make fclean
	git add .
	git commit -m "Merged Ruhan and Ammar's work"
	git push

re: fclean all
