
// Bibliotecas incluídas
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

// Constantes

const int windowHeight = 600;
const int windowWidth = 800;
const int portHeight = 600;
const int portWidth = 800;

// seletor (seta para cima altera para o proximo) (seta para baixo altera para o anterior)
int seletor = 0;

// Funções de compilação de shader
unsigned int make_module(const std::string& filepath, unsigned int module_type)
{
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while (std::getline(file,line))
    {
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char * shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule,1,&shaderSrc,NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "make module error:\n" << errorLog << std::endl;
    }
    return shaderModule;
}
unsigned int make_shader(const std::string& vertex_filepath,const std::string& fragment_filepath)
{
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules)
    {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    int success;
    glGetShaderiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "make Shader error:\n" << errorLog << std::endl;
    }

    for (unsigned int shaderModule : modules)
    {
        glDeleteShader(shaderModule);
    }

    return shader;
}


// Função para lidar com entrada do teclado
void processInput(GLFWwindow *window)
{
    static bool downPressed = false;
    static bool upPressed = false;

    // Funções para fechar a janela
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !upPressed or glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !downPressed)
    {
        std::cout << seletor << std::endl;
    }


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Função para trocar desenho (seta para cima = proximo) (seta para baixo = anterior)
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !downPressed)
    {
        seletor -= 1;
        downPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !upPressed)
    {
        seletor += 1;
        upPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        downPressed = false;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
        upPressed = false;

    if (seletor == 0)
        glViewport(0, 0, portWidth, portHeight);
    if (seletor == 1)
        glViewport(0, 0, 10, 10);
    if (seletor == 2)
        glViewport(600, 0, 800, 0);
    if (seletor == 3)
        glViewport(0, 0, 800, 600);
    if (seletor == 4)
        glViewport(400, 300, 400, 300);
    if (seletor == 5)
        glViewport(400, 300, 400, 300);
    if (seletor < 0)
        seletor = 0;
    else if (seletor > 14)
        seletor = 14;
}


// Função main
int main(int, char **)
{
    // Inicialização do GLFW
    GLFWwindow *window;
    if (!glfwInit())
    {
        return -1;
    }

    // Configurar versão OpenGL e perfil de compatibilidade
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criação da janela GLFW
    window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Triângulo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Carregamento das funções OpenGL via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Não foi possível carregar a biblioteca OpenGL." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Define as dimensões da viewport para a janela criada
    glViewport(0, 0, portWidth, portHeight);

    unsigned int shader = make_shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");

    // Configura a matriz de projeção ortográfica para coordenadas normalizadas de -1 a 1.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // Configura a matriz de visualização do modelo para a identidade
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    // Define os vértices para desenhar triângulos e pontos
    float vertices[] = {
        // (x, y, z) - posição
        // (r, g, b) - cor
        // (x, y, z, r, g, b) - posição > cor
        -0.8f, -0.9f, 0.0f, 0.0f, 0.0f, 0.0f, //00
        +0.8f, -0.9f, 0.0f, 0.0f, 0.0f, 0.0f, //01
        +0.0f, +0.8f, 0.0f, 0.0f, 0.0f, 0.0f, //02
    };
    
    // Define os indices para os vértices
    float indices[] =
    {

    };

    // Criação dos buffers e arrays de vértices
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Configuração dos buffers e arrays de vértices
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura atributos de Posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Configura atributos de Cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configura a cor do fundo
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        // Verifica se alguma tecla foi pressionada
        processInput(window);

        // Limpa o buffer de cor
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        // Desenha apenas o triangulo
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

        // Troca os buffers da janela
        glfwSwapBuffers(window);

        // Verifica e chama eventos
        glfwPollEvents();
    }

    // Deleta os buffers e finaliza o GLFW
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}