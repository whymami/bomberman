NAME = bomberman

CXX = g++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

GLFLAGS = -lglfw -ldl -lGL #windows için
# GLFLAGS += -lX11 -lpthread -lm -lrt -ldl #linux için

OBJS = $(SRCS:.cpp=.o)

SRCS = main.cpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJS) $(GLFLAGS)


clean:
	rm -f $(OBJS) $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all
