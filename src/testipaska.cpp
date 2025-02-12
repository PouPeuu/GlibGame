#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Window dimensions
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// Shader program
unsigned int shaderProgram;
unsigned int VAO, VBO, EBO;

// Game objects
struct GameObject {
    glm::vec2 position;
    glm::vec2 size;
    unsigned int texture;
};

struct Bullet {
    glm::vec2 position;
    glm::vec2 velocity;
};

struct Enemy {
    glm::vec2 position;
    glm::vec2 velocity;
};

// Game state
GameObject player;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
float enemySpawnTimer = 0.0f;

// Matrices
glm::mat4 projection;

const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D texture1;
void main() {
    FragColor = texture(texture1, TexCoord);
}
)glsl";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }
    return id;
}

unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    return textureID;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 0.02f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.position.y += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.position.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.position.x += speed;

    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        // Convert screen coordinates to NDC
        glm::vec2 mousePos(
            (xpos / SCR_WIDTH) * 2.0f - 1.0f,
            1.0f - (ypos / SCR_HEIGHT) * 2.0f
        );
        
        glm::vec2 direction = glm::normalize(mousePos - player.position);
        float bulletSpeed = 0.03f;
        bullets.push_back({player.position, direction * bulletSpeed});
        spacePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) spacePressed = false;
}

void updateGame() {
    // Update bullets
    for (auto& bullet : bullets) {
        bullet.position += bullet.velocity;
    }
    
    // Spawn enemies
    enemySpawnTimer -= 0.01f;
    if (enemySpawnTimer <= 0.0f) {
        int edge = rand() % 4;
        glm::vec2 position;
        switch (edge) {
            case 0: position = glm::vec2(-1.1f, glm::linearRand(-1.0f, 1.0f)); break;
            case 1: position = glm::vec2(1.1f, glm::linearRand(-1.0f, 1.0f)); break;
            case 2: position = glm::vec2(glm::linearRand(-1.0f, 1.0f), 1.1f); break;
            case 3: position = glm::vec2(glm::linearRand(-1.0f, 1.0f), -1.1f); break;
        }
        
        glm::vec2 direction = glm::normalize(player.position - position);
        enemies.push_back({position, direction * 0.01f});
        enemySpawnTimer = 2.0f;
    }

    // Update enemies
    for (auto& enemy : enemies) {
        enemy.position += enemy.velocity;
    }

    // Collision detection
    auto bullet = bullets.begin();
    while (bullet != bullets.end()) {
        bool collided = false;
        auto enemy = enemies.begin();
        while (enemy != enemies.end()) {
            if (glm::distance(bullet->position, enemy->position) < 0.1f) {
                enemy = enemies.erase(enemy);
                collided = true;
                break;
            } else {
                ++enemy;
            }
        }
        
        if (collided || glm::abs(bullet->position.x) > 1.2f || glm::abs(bullet->position.y) > 1.2f) {
            bullet = bullets.erase(bullet);
        } else {
            ++bullet;
        }
    }
}

void renderObject(const GameObject& obj) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(obj.position, 0.0f));
    model = glm::scale(model, glm::vec3(obj.size, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, obj.texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void initializeGraphics() {
    // Set up vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void kakka() {
    stbi_set_flip_vertically_on_load(true);
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Game", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    initializeGraphics();
    shaderProgram = createShaderProgram();
    glUseProgram(shaderProgram);
    
    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    // Load textures
    player.texture = loadTexture("res/player.png");
    unsigned int bulletTexture = loadTexture("res/bullet.png");
    unsigned int enemyTexture = loadTexture("res/enemy.png");
    
    player.position = glm::vec2(0.0f, 0.0f);
    player.size = glm::vec2(0.1f, 0.1f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        updateGame();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);

        // Draw player
        renderObject(player);

        // Draw bullets
        for (auto& bullet : bullets) {
            GameObject bulletObj{bullet.position, glm::vec2(0.05f), bulletTexture};
            renderObject(bulletObj);
        }

        // Draw enemies
        for (auto& enemy : enemies) {
            GameObject enemyObj{enemy.position, glm::vec2(0.1f), enemyTexture};
            renderObject(enemyObj);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
}