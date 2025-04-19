NAME = bomberman
SRC_DIR = src
SRCS =  test/test.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(addprefix bin/,$(notdir $(SRCS:.cpp=.o)))
GLAD_OBJ = bin/gl.o
CXX = g++
CXXFLAGS = -std=c++17 -I./include -I./lib -I./lib/glad/include
JSON_LIB_DIR = lib/json
GLAD_DIR = lib/glad
GLAD_URL = https://gen.glad.sh/generated/tmps9st6gpcglad/glad.zip
JSON_LIB_URL = https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
STB_EASY_FONT_URL = https://raw.githubusercontent.com/nothings/stb/master/stb_easy_font.h
STB_IMAGE_URL = https://raw.githubusercontent.com/nothings/stb/master/stb_image.h

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	LDFLAGS = -lglfw -ldl -lGL -lassimp
endif
ifeq ($(UNAME_S), Darwin)
	LDFLAGS = -lGLEW -lGLFW -lassimp -framework  OpenGL 
	CXXFLAGS += -DGL_SILENCE_DEPRECATION -I/opt/homebrew/include
	LIB_PATH = -L/opt/homebrew/lib
endif
ifeq ($(OS), Windows_NT)
	LDFLAGS = -lglfw3 -lgdi32 -lopengl32 -lassimp
endif

all: bin lib setup_glad setup_json $(NAME)

bin:
	mkdir -p bin

lib:
	mkdir -p lib
	# Download STB libraries if they don't exist
	test -f lib/stb_easy_font.h || curl -o lib/stb_easy_font.h $(STB_EASY_FONT_URL)
	test -f lib/stb_image.h || curl -o lib/stb_image.h $(STB_IMAGE_URL)

setup_glad: lib
	@if [ ! -d "$(GLAD_DIR)" ]; then \
		echo "Downloading GLAD library..."; \
		curl -L $(GLAD_URL) -o glad.zip; \
		mkdir -p $(GLAD_DIR); \
		unzip -q glad.zip -d $(GLAD_DIR); \
		rm glad.zip; \
		echo "GLAD library has been set up successfully."; \
	else \
		echo "GLAD library is already set up."; \
	fi

setup_json: lib
	@if [ ! -d "$(JSON_LIB_DIR)" ]; then \
		echo "Downloading JSON library..."; \
		curl -L $(JSON_LIB_URL) -o json.hpp; \
		mkdir -p $(JSON_LIB_DIR); \
		mv json.hpp $(JSON_LIB_DIR); \
		echo "JSON library has been set up successfully."; \
	else \
		echo "JSON library is already set up."; \
	fi

$(NAME): $(OBJS) $(GLAD_OBJ)
	$(CXX) $(OBJS) $(GLAD_OBJ) -o $(NAME) $(LIB_PATH) $(LDFLAGS)

$(GLAD_OBJ): setup_glad
	$(CXX) $(CXXFLAGS) -c $(GLAD_DIR)/src/gl.c -o $(GLAD_OBJ)

bin/%.o: */%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(GLAD_OBJ)
	rm -rf bin
	rm -f lib/stb_easy_font.h lib/stb_image.h
	rm -rf $(GLAD_DIR)
	rm -rf $(JSON_LIB_DIR)

fclean: clean
	rm -f $(NAME) game
	rm -rf lib

re: fclean all

.PHONY: all clean fclean re bin lib setup_glad json_lib