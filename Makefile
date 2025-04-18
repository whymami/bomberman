NAME = bomberman
SRCS = main.cpp Window.cpp Button.cpp Options.cpp Slider.cpp
OBJS = $(SRCS:.cpp=.o)
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	LDFLAGS = -lglfw -ldl -lGL
endif
ifeq ($(UNAME_S), Darwin)
	LDFLAGS = -lGLEW -lGLFW -framework OpenGL
	CXXFLAGS += -DGL_SILENCE_DEPRECATION -I/opt/homebrew/include
	LIB_PATH = -L/opt/homebrew/lib
endif
ifeq ($(OS), Windows_NT)
	LDFLAGS = -lglfw3 -lgdi32 -lopengl32
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LIB_PATH) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) game

re: fclean all

.PHONY: all clean fclean re
