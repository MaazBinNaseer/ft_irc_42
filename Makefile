NAME = ircserv

SRCDIR = src
SRCS = main.cpp Server.cpp ServerFuncs.cpp \
		Channel.cpp ChannelFuncs.cpp \
		Commands.cpp\
		Client.cpp Courier.cpp Bot.cpp \
		Parse.cpp

GREEN   =   \033[1;32m
RESET   =   \033[0m


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
	@echo "$(GREEN)\033[1mCompilation complete: IRC Server Ready$(RESET)"

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

gpush:
	make fclean
	rm -rf Severlog.txt .vscode
	git add .
	git commit -m "Merged Ruhan and Ammar's work"
	git push

irssi:
	docker run -it --rm --name irssi --network host irssi

re: fclean all
