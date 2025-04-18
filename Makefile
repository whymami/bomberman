NAME = bomberman
SRCS = main.cpp Window.cpp Button.cpp Options.cpp Slider.cpp
OBJS = $(SRCS:.cpp=.o)
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
LDFLAGS = -lglfw -lGL -lGLEW

SILENCE_DEPRECATION = -DGL_SILENCE_DEPRECATION
INCLUDE_PATH = -I/opt/homebrew/include
LIB_PATH = -L/opt/homebrew/lib
MACLDFLAGS = -lGLEW -lGLFW -framework OpenGL

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LDFLAGS)

mac:
	$(CXX) -std=c++17 $(INCLUDE_PATH) $(SILENCE_DEPRECATION) \
		$(SRCS) \
		-o $(NAME) $(LIB_PATH) $(MACLDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) game

re: fclean all

.PHONY: all clean fclean re mac
