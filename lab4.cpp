#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <glm/glm.hpp>          //dodanie bibliotek
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/System/Time.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

const GLchar* vertexSource = R"glsl(
#version 150 core

uniform mat4 model;     //deklaracja zmiennych
uniform mat4 view;
uniform mat4 proj;

in vec3 position;
in vec3 color;
in vec2 aTexCoord;
in vec3 aNormal;        //dodane

out vec2 TexCoord;
out vec3 Color;
out vec3 Normal;        //dodane
out vec3 FragPos;       //dodane

void main() {
    TexCoord = aTexCoord;
    Color = color;
    Normal = aNormal;       //dodane
    gl_Position = proj * view * model * vec4(position, 1.0);    //określenie położenia punktów
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core

out vec4 outColor;

in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;         //dodane
in vec3 FragPos;        //dodane

uniform vec3 lightPos;      //dodane
uniform sampler2D texture1;
uniform float ambientStrength;      //dodane

//float ambientStrength = 0.1;        //dodane

void main() {
    vec3 ambientlightColor = vec3(1.0,1.0,1.0);
    vec4 ambient = ambientStrength * vec4(ambientlightColor,1.0);
    vec3 difflightColor = vec3(1.0,1.0,1.0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * difflightColor;

    //outColor = vec4(Color, 1.0);
    //outColor = texture(texture1, TexCoord);
    outColor = (ambient+vec4(diffuse,1.0)) * texture(texture1, TexCoord);
}
)glsl";

struct Vertex {
    float x, y, z, r, g, b, u, v;
};

// Funkcja do generowania sześcianu, macierz
vector<Vertex> generateCube() {
    vector<Vertex> vertices;

    float verticesData[] = {

            //tył
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f,

            //przód
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            //lewo
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            //prawo 
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            // dół
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            // góra
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f, 1.0f,
            };


    int vertexCount = sizeof(verticesData) / (8 * sizeof(float));  // Oblicz ilość wierzchołków

    for (int i = 0; i < vertexCount; ++i) {
        Vertex vertex;
        vertex.x = verticesData[i * 8 + 0];
        vertex.y = verticesData[i * 8 + 1];
        vertex.z = verticesData[i * 8 + 2];
        vertex.r = verticesData[i * 8 + 3];
        vertex.g = verticesData[i * 8 + 4];
        vertex.b = verticesData[i * 8 + 5];
        vertex.u = verticesData[i * 8 + 6];
        vertex.v = verticesData[i * 8 + 7];
        vertices.push_back(vertex);
    }

    return vertices;
}



void CheckShaderCompilation(GLuint shader, const string& shaderName) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, log.data());
        cerr << "Error compiling " << shaderName << ":\n" << log.data() << endl;
    }
    else {
        cout << "Compilation " << shaderName << " OK" << endl;
    }
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    double lastX = 400, lastY = 300;
    bool firstMouse = true;
    double yaw = -90; //obrót względem osi Y
    double pitch = 0; //obrót względem osi X
    sf::Vector2i localPosition;
    sf::Vector2i lastPosition;

    // Ustawienia V-sync
    bool vsync = true;  // fals - aby wyłączyć V-sync

    // Okno renderingu
    sf::Window window(sf::VideoMode(1200, 800, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setMouseCursorGrabbed(true); //przechwycenie kursora myszy w oknie 
    window.setMouseCursorVisible(false); //ukrycie kursora myszy 

    // Włącz lub wyłącz V-sync
    window.setVerticalSyncEnabled(vsync);

    // Inicjalizacja GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    // Utworzenie VAO (Vertex Array Object)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Utworzenie VBO (Vertex Buffer Object)
    // i skopiowanie do niego danych wierzchołkowych
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //przechowywanie wierzcholkow szescianu 
    vector<Vertex> vertices = generateCube();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Utworzenie i skompilowanie shadera wierzchołków
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader, "vertex shader");

    // Utworzenie i skompilowanie shadera fragmentów
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader, "fragment shader");

    // Zlinkowanie obu shaderów w jeden wspólny program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specifikacja formatu danych wierzchołkowych
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(3 * sizeof(float)));

    GLint TexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
    glEnableVertexAttribArray(TexCoord);
    glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    //dodane 6 linijek pod
    GLint NorAttrib = glGetAttribLocation(shaderProgram, "aNormal");
    glEnableVertexAttribArray(NorAttrib);
    glVertexAttribPointer(NorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    //ustalenie położenia światła
    glm::vec3 lightPos(1.2f, 2.0f, 2.0f);
    GLint uniLightPos = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(uniLightPos, 1, &lightPos[0]);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("texture.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);

    // macierze projekcji, widoku i modelu
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);

    // zmienne kontrolujące kamerę
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    //glm::vec3 lightPos(1.2f, 2.0f, 2.0f);       //dodane
    float ambientStrength = 0.3;                //dodane

    // wysyłanie macierzy do shadera
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    // zmienne kontrolujące kamerę
    float cameraSpeed = 0.05f;
    float rotationSpeed = 0.01f;



    sf::Clock clock;
    sf::Time time;
    sf::Clock zegarKlatek;
    unsigned int licznikKlatek = 0;
    //float cameraSpeed = 0.000002f * time;

    window.setFramerateLimit(20);
    // Rozpoczęcie pętli zdarzeń
    bool running = true;
    GLenum prymityw = GL_TRIANGLES;
    while (running) {
        time = clock.getElapsedTime();
        clock.restart();

        float cameraSpeed = 0.000002f * time.asMicroseconds();

        licznikKlatek++;

        if (zegarKlatek.getElapsedTime().asSeconds() >= 1.0) {
            // aktualizacja tytułu okna z liczbą klatek na sekundę
            window.setTitle("OpenGL | FPS: " + to_string(licznikKlatek));

            // reesetowanie licznika klatek i zrestartowanie zegara
            licznikKlatek = 0;
            zegarKlatek.restart();
        }
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
            case sf::Event::Closed:
                running = false;
                break;

            case sf::Event::KeyPressed:
                switch (windowEvent.key.code) {
                case sf::Keyboard::Escape:
                    running = false;
                    break;

                case sf::Keyboard::Num1:
                    cout << "1";
                    ambientStrength += 0.1f;
                    if (ambientStrength >= 1.0f) {
                        ambientStrength = 1.0f;
                    }
                    cout << ambientStrength << " ";
                    glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), ambientStrength);
                    break;

                case sf::Keyboard::Num2:
                    cout << "2";
                    ambientStrength -= 0.1f;
                    if (ambientStrength <= 0.0f) {
                        ambientStrength = 0.0f;
                    }
                    cout << ambientStrength << " ";
                    glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), ambientStrength);

                    break;

                case sf::Keyboard::Num3:
                    glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), 0.0f);
                    break;

                case sf::Keyboard::Num4:
                    glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), 1.0f);
                    break;

                case sf::Keyboard::Num5:
                    prymityw = GL_TRIANGLES;
                    break;

                case sf::Keyboard::Num6:
                    prymityw = GL_TRIANGLE_STRIP;
                    break;

                case sf::Keyboard::Num7:
                    prymityw = GL_TRIANGLE_FAN;
                    break;

                case sf::Keyboard::Num8:
                    prymityw = GL_QUADS;
                    break;

                case sf::Keyboard::Num9:
                    prymityw = GL_QUAD_STRIP;
                    break;
                case sf::Keyboard::Num0:
                    prymityw = GL_POLYGON;
                    break;

                }
                break;
            case sf::Event::MouseMoved:
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);    //Wyznaczenie lokalnej pozycji kursora myszy

                if (firstMouse) {
                    lastX = localPosition.x;
                    lastY = localPosition.y;
                    firstMouse = false;
                }
                double xoffset = localPosition.x - lastX;   //Wyznaczenie zmiany położenia i zapamiętanie ostatniej pozycji
                double yoffset = localPosition.y - lastY;
                lastX = localPosition.x;
                lastY = localPosition.y;

                constexpr double sensitivity = 0.1;
                xoffset *= sensitivity;             //Aktualizacja kątów ustawienia kamery
                yoffset *= sensitivity;
                yaw += xoffset;
                pitch -= yoffset;

                if (pitch > 89.0f)      //Nałożenie ograniczeń na maksymalne wartości kątów i wyznaczenie nowych wartości
                    pitch = 89.0f;      //wektora pochylenia kamery(tzw.kąty Eulera)
                if (pitch < -89.0f)
                    pitch = -89.0f;
                glm::vec3 front;
                front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                front.y = sin(glm::radians(pitch));
                front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                cameraFront = glm::normalize(front);

                glm::mat4 view;     //Tworzenie aktualnej macierzy widoku
                view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
                glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

                break;
            }
        }

        // Obsługa ruchu kamery
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {  //kamera przesuwa się do przodu w kierunku, w którym patrzy 
            cameraPos += cameraSpeed * cameraFront;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {  //do tyłu, przeciwnie do kierunku, w którym patrzy
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {  //przesuwa kamerę w lewo w płaszczyźnie poziomej
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {  //przesuwa kamerę w prawo w płaszczyźnie poziomej
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {   //kamera obraca się w lewo
            cameraFront = glm::mat3(glm::rotate(glm::mat4(1.0f), rotationSpeed, cameraUp)) * cameraFront;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {  //kamera obraca się w prawo
            cameraFront = glm::mat3(glm::rotate(glm::mat4(1.0f), -rotationSpeed, cameraUp)) * cameraFront;
        }*/
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {   //kamera obraca się w lewo
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {  //kamera obraca się w prawo
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture1);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Wymiana buforów tylni/przedni
        window.display();
    }

    // Kasowanie programu i czyszczenie buforów
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);


    // Zamknięcie okna renderingu
    window.close();

    return 0;
}