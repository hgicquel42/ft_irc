NAME = ircserv

FILES = \
	utils/chars.cpp \
	utils/colors.cpp \
	utils/errors.cpp \
	utils/numbers.cpp \
	utils/sockets.cpp \
	params.cpp \
	poll.cpp \
	main.cpp \

INPUT = ${addprefix sources/, ${FILES}}

OUTPUT = ${addprefix objects/, ${FILES:.cpp=.o}}

all: ${NAME}

objects/%.o: sources/%.cpp
	@mkdir -p ${dir $@}
	c++ -Wall -Wextra -Werror -std=c++98 -g -I sources -c $< -o $@

${NAME}: ${OUTPUT}
	c++ -Wall -Wextra -Werror -std=c++98 -g -I sources ${OUTPUT} -o ${NAME}

clean:
	rm -rf objects

fclean:	clean
	rm -f ${NAME}

re:		fclean all

run: $(NAME)
	./$(NAME)

x: re
	rm -rf objects