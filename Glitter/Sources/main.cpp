#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <glm/gtx/string_cast.hpp>
#include <cmath> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 100.0f));
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

int grid_size;
bool is_video = false;
std::string filename = "bad-apple-resized-short.mp4";

#define MAX_BUFFER_SIZE 1024

const double PI = std::atan(1.0)*4;

std::vector<float> LoadColors() {
    std::vector<float> completeRgbVector;

    std::ifstream myfile("../../rgba.txt");
    if (myfile.is_open() == false) {
        std::cout << "Error: cannot open the file.";
        return completeRgbVector;
    }
    std::string s;
    char buffer[MAX_BUFFER_SIZE];

    bool firstLine = true;
    bool secondLine = true;
    while (myfile.getline(buffer, MAX_BUFFER_SIZE)) {
        std::stringstream ss(buffer);
        ss >> s;

        if (firstLine) {
            grid_size = stoi(s);
            firstLine = false;
            // cout << "grid size: " << grid_size << endl;
            continue;

        } else if (secondLine) {
            if (s == "video") {
                is_video = true;

                myfile.getline(buffer, MAX_BUFFER_SIZE);
                std::stringstream ss(buffer);
                ss >> s;
                filename = s;
                return completeRgbVector;
            }
            secondLine = false;
            continue;
        }
        completeRgbVector.push_back(std::stof(s));
    }

    return completeRgbVector;
}

// Need to call it here - since recreating spheres in the render is intensive
std::vector<float> rgbaVector = LoadColors();

int main() {
    std::cout << filename;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.4.camera.vs", "7.4.camera.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // CUBE
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  //
         0.5f, -0.5f, -0.5f,  //
         0.5f,  0.5f, -0.5f,  //
         0.5f,  0.5f, -0.5f,  //
        -0.5f,  0.5f, -0.5f,  //
        -0.5f, -0.5f, -0.5f,  //
                              //
        -0.5f, -0.5f,  0.5f,  //
         0.5f, -0.5f,  0.5f,  //
         0.5f,  0.5f,  0.5f,  //
         0.5f,  0.5f,  0.5f,  //
        -0.5f,  0.5f,  0.5f,  //
        -0.5f, -0.5f,  0.5f,  //
                              //
        -0.5f,  0.5f,  0.5f,  //
        -0.5f,  0.5f, -0.5f,  //
        -0.5f, -0.5f, -0.5f,  //
        -0.5f, -0.5f, -0.5f,  //
        -0.5f, -0.5f,  0.5f,  //
        -0.5f,  0.5f,  0.5f,  //
                              //
         0.5f,  0.5f,  0.5f,  //
         0.5f,  0.5f, -0.5f,  //
         0.5f, -0.5f, -0.5f,  //
         0.5f, -0.5f, -0.5f,  //
         0.5f, -0.5f,  0.5f,  //
         0.5f,  0.5f,  0.5f,  //
                              //
        -0.5f, -0.5f, -0.5f,  //
         0.5f, -0.5f, -0.5f,  //
         0.5f, -0.5f,  0.5f,  //
         0.5f, -0.5f,  0.5f,  //
        -0.5f, -0.5f,  0.5f,  //
        -0.5f, -0.5f, -0.5f,  //
                              //
        -0.5f,  0.5f, -0.5f,  //
         0.5f,  0.5f, -0.5f,  //
         0.5f,  0.5f,  0.5f,  //
         0.5f,  0.5f,  0.5f,  //
        -0.5f,  0.5f,  0.5f,  //
        -0.5f,  0.5f, -0.5f   //
    };

    // EQUILATERAL TRIANGLE
    // float vertices[] = {
    //     0.0f, 0.0f,   0.0f,  //
    //     0.5f, 0.866f, 0.0f,  //
    //     1.0f, 0.0f,   0.0f,  //
    // };                       //

    // PYRAMID
    // float vertices[] = {
    //     0.0f,  1.0f, 0.0f,  //
    //     0.5f,  0.0f, 0.5f,  //
    //     -0.5f, 0.0f, 0.5f,  //
    //
    //     0.0f,  1.0f, 0.0f,   //
    //     -0.5f, 0.0f, 0.5f,   //
    //     -0.5f, 0.0f, -0.5f,  //
    //
    //     0.0f,  1.0f, 0.0f,   //
    //     -0.5f, 0.0f, -0.5f,  //
    //     0.5f,  0.0f, -0.5f,  //
    //
    //     0.0f,  1.0f, 0.0f,   //
    //     0.5f,  0.0f, -0.5f,  //
    //     0.5f,  0.0f, 0.5f,   //
    //
    //     0.5f,  0.0f, 0.5f,   //
    //     -0.5f, 0.0f, 0.5f,   //
    //     -0.5f, 0.0f, -0.5f,  //
    //     -0.5f, 0.0f, -0.5f,  //
    //     0.5f,  0.0f, -0.5f,  //
    //     0.5f,  0.0f, 0.5f    //
    // };                       //

    // world space positions of our cubes
    //     glm::vec3 cubePositions[] = {
    //         glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    //         glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    //         glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    //         glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    //         glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    glm::vec3 cubePositions[grid_size][grid_size];
    glm::vec4 mappedColors[grid_size][grid_size];

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    ourShader.use();

    // std::string filename = "bad-apple-resized.mp4";
    // std::string filename = "bad-apple-resized-short.mp4";
    cv::VideoCapture capture(filename);
    cv::Mat frame;

    if (!capture.isOpened()) throw "Error when reading steam_avi";

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could
        // change every frame)
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 500.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);

        float centre = grid_size/2;
        float depthValue = 0.0f;

        float maxDistFromCentre = sqrt(pow(centre, 2.0) + pow(centre, 2.0));

        for (unsigned int i = 0; i < grid_size; i++) {
            float y = i * -1.0f;
            for (unsigned int j = 0; j < grid_size; j++) {
                float x = j * 1.0f;

                // float depthValue = 1.5 * sin(glfwGetTime() * 2 + j * 100);
                float distFromCentre = sqrt(pow(i - centre, 2.0) + pow(j - centre, 2.0));

                if (glfwGetTime() > distFromCentre/maxDistFromCentre * 10) {
                    // depthValue = 15 * cos(glfwGetTime() + distFromCentre * PI);
                    depthValue = 10 * (maxDistFromCentre - distFromCentre) / maxDistFromCentre * exp(- 0.1 * glfwGetTime()) * cos(glfwGetTime() + distFromCentre/maxDistFromCentre * 10);
                } else {
                    depthValue = 0.0f;
                }

                // depthValue = 5 * (maxDistFromCentre - distFromCentre)/maxDistFromCentre * exp(- 0.1 * glfwGetTime()) * cos(glfwGetTime() + distFromCentre * PI);

                // depthValue = (100/(10 * distFromCentre)) * exp(- 0.1 * glfwGetTime()) * cos(glfwGetTime() + distFromCentre * PI);

                // 𝐴𝑒−𝑏𝑡𝑐𝑜𝑠(𝑤𝑡+𝜙) 

                std::cout << glfwGetTime() << std::endl;
                // std::cout << "Cube(" << i << "," << j << ") distFromCentre: " << distFromCentre << std::endl;

                cubePositions[i][j] = glm::vec3(x, y, depthValue);
            }
        }

        if (is_video == true) {
            capture >> frame;
            if (!frame.empty()) {
                for (unsigned int i = 0; i < grid_size; i++) {
                    for (unsigned int j = 0; j < grid_size; j++) {
                        // calculate the model matrix for each object and pass
                        // it to shader before drawing
                        glm::mat4 model =
                            glm::mat4(1.0f);  // make sure to initialize matrix
                                              // to identity matrix first
                        model = glm::translate(model, cubePositions[i][j]);

                        float blue = frame.at<cv::Vec3b>(i, j)[0];
                        float green = frame.at<cv::Vec3b>(i, j)[1];
                        float red = frame.at<cv::Vec3b>(i, j)[2];

                        glm::vec4 color = glm::vec4(red, green, blue, 1.0f);

                        ourShader.setVec4("ourColor", color);
                        ourShader.setMat4("model", model);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            }
        } else {
            int pixelIndex = 0;
            for (unsigned int i = 0; i < grid_size; i++) {
                for (unsigned int j = 0; j < grid_size; j++) {
                    int firstIdx = pixelIndex * 4;
                    float pixelRgb[4] = {
                        rgbaVector[firstIdx], rgbaVector[firstIdx + 1],
                        rgbaVector[firstIdx + 2], rgbaVector[firstIdx + 3]};

                    glm::vec4 color = glm::vec4(pixelRgb[0], pixelRgb[1], pixelRgb[2], pixelRgb[3]);

                    mappedColors[i][j] = color;
                    pixelIndex++;
                }
            }
            
            int sphereIndex = 0;
            for (unsigned int i = 0; i < grid_size; i++) {
                for (unsigned int j = 0; j < grid_size; j++) {
                    // calculate the model matrix for each object and pass it to
                    // shader before drawing
                    glm::mat4 model =
                        glm::mat4(1.0f);  // make sure to initialize matrix to
                                          // identity matrix first

                    model = glm::translate(model, cubePositions[i][j]);

                    glm::vec3 translation = {-(grid_size/2 * 1.0f), (grid_size/2 * 1.0f), 0.0f};
                    model = glm::translate(model, translation);

                    // std::cout << glm::to_string(model) << std::endl;

                    // float angle = 20.0f * i;
                    // model = glm::rotate(model, glm::radians(angle),
                    //                     glm::vec3(1.0f, 0.3f, 0.5f));

                    glm::vec4 color = mappedColors[i][j];

                    // only render if alpha > 0
                    if (!color[3]==0) {
         
                        ourShader.setVec4("ourColor", color);
                        ourShader.setMat4("model", model);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                    sphereIndex++;
                }
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 20);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 20);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 20);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 20);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
