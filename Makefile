NAME = bomberman
SRCS = main.cpp src/Window.cpp src/Button.cpp src/Options.cpp src/Slider.cpp
OBJS = $(addprefix bin/,$(notdir $(SRCS:.cpp=.o)))
CXX = g++
CXXFLAGS =  -std=c++17 -I./include -I./lib

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

all: bin $(NAME)

bin:
	mkdir -p bin

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LIB_PATH) $(LDFLAGS)

bin/%.o: */%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -rf bin

fclean: clean
	rm -f $(NAME) game

re: fclean all

.PHONY: all clean fclean re bin