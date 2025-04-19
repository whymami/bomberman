# bomberman

### openGL linux'a kurmak için:
    sudo apt install libglfw3-dev

### derlemek için için bu flaglerin kullanılması lazım
    -lglfw -ldl -lGL
### ör:
    g++ main.cpp -o bomberman -lglfw -ldl -lGL

### makefile osType türüne göre kurulum yapacak 

assimp kurldu


glfw = "https://www.glfw.org/download.html"

cd glfw-3.x.x
cmake .
make
sudo make install

GLEW
İndirme linki: https://glew.sourceforge.net/

cd glew-2.x.x
make
sudo make install

Assimp
İndirme linki: https://github.com/assimp/assimp/releases
cd assimp-5.x.x
cmake .
make
sudo make install


GLM
İndirme linki: https://github.com/g-truc/glm/releases


sudo apt-get install libglfw3-dev libglew-dev libassimp-dev libglm-dev
g++ test.cpp -o tnt_viewer -I/usr/local/include -L/usr/local/lib -lglfw -lGLEW -lassimp -lGL