NAME = ircserv

FILES = \
	main.cpp \
	test/test.cpp \

INPUT = ${addprefix sources/, ${FILES}}

OUTPUT = ${addprefix objects/, ${FILES:.cpp=.o}}

all: ${NAME}

objects/%.o: sources/%.cpp
	@mkdir -p ${dir $@}
	c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

${NAME}: ${OUTPUT}
	c++ -Wall -Wextra -Werror -std=c++98 ${OUTPUT} -o ${NAME}

clean:
	rm -rf objects

fclean:	clean
	rm -f ${NAME}

re:		fclean all

x: re
	rm -rf objects

test: $(NAME)
	./$(NAME)