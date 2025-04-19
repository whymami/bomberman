#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

// Vertex shader source
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    out vec3 Normal;
    out vec3 FragPos;
    
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
    }
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 Normal;
    in vec3 FragPos;
    
    out vec4 FragColor;
    
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform vec3 objectColor;
    
    void main()
    {
        // Ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
        
        // Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
        
        vec3 result = (ambient + diffuse + specular) * objectColor;
        FragColor = vec4(result, 1.0);
    }
)";

struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;
};

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh newMesh;
        
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            // Position
            newMesh.vertices.push_back(mesh->mVertices[j].x);
            newMesh.vertices.push_back(mesh->mVertices[j].y);
            newMesh.vertices.push_back(mesh->mVertices[j].z);
            
            // Normal
            if (mesh->mNormals) {
                newMesh.vertices.push_back(mesh->mNormals[j].x);
                newMesh.vertices.push_back(mesh->mNormals[j].y);
                newMesh.vertices.push_back(mesh->mNormals[j].z);
            }
        }
        
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                newMesh.indices.push_back(face.mIndices[k]);
            }
        }
        
        // Generate buffers
        glGenVertexArrays(1, &newMesh.VAO);
        glGenBuffers(1, &newMesh.VBO);
        glGenBuffers(1, &newMesh.EBO);
        
        glBindVertexArray(newMesh.VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, newMesh.vertices.size() * sizeof(float), &newMesh.vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.indices.size() * sizeof(unsigned int), &newMesh.indices[0], GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        meshes.push_back(newMesh);
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, meshes);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "TNT Model Viewer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Load the TNT model
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("tnt.fbx", 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return -1;
    }
    
    std::vector<Mesh> meshes;
    processNode(scene->mRootNode, scene, meshes);
    
    // Set up camera
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 2.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        glUseProgram(shaderProgram);
        
        // Set uniforms
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 2.0f, 2.0f, 2.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 2.0f, 5.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.0f, 0.0f);
        
        // Draw meshes
        for (const auto& mesh : meshes) {
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    for (auto& mesh : meshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
    
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    
    return 0;
}
