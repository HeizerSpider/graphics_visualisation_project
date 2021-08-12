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
void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 100.0f));
int camera_speed = 20;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool leftMouseButtonHold = false;

// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;
float startRipple;

#define MAX_BUFFER_SIZE 1024
const double PI = std::atan(1.0) * 4;

// Effects
int displayMode = 0;
int shapeMode = 0;
bool transparent = false;
int grid_size;

bool is_video = false;
std::string filename = "bad-apple-resized-short.mp4";

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
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Display Matrix", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  //* show cursor

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
    float cube[] = {
        -0.5f, -0.5f, -0.5f,  //
        0.5f, -0.5f, -0.5f,   //
        0.5f, 0.5f, -0.5f,    //
        0.5f, 0.5f, -0.5f,    //
        -0.5f, 0.5f, -0.5f,   //
        -0.5f, -0.5f, -0.5f,  //
                              //
        -0.5f, -0.5f, 0.5f,   //
        0.5f, -0.5f, 0.5f,    //
        0.5f, 0.5f, 0.5f,     //
        0.5f, 0.5f, 0.5f,     //
        -0.5f, 0.5f, 0.5f,    //
        -0.5f, -0.5f, 0.5f,   //
                              //
        -0.5f, 0.5f, 0.5f,    //
        -0.5f, 0.5f, -0.5f,   //
        -0.5f, -0.5f, -0.5f,  //
        -0.5f, -0.5f, -0.5f,  //
        -0.5f, -0.5f, 0.5f,   //
        -0.5f, 0.5f, 0.5f,    //
                              //
        0.5f, 0.5f, 0.5f,     //
        0.5f, 0.5f, -0.5f,    //
        0.5f, -0.5f, -0.5f,   //
        0.5f, -0.5f, -0.5f,   //
        0.5f, -0.5f, 0.5f,    //
        0.5f, 0.5f, 0.5f,     //
                              //
        -0.5f, -0.5f, -0.5f,  //
        0.5f, -0.5f, -0.5f,   //
        0.5f, -0.5f, 0.5f,    //
        0.5f, -0.5f, 0.5f,    //
        -0.5f, -0.5f, 0.5f,   //
        -0.5f, -0.5f, -0.5f,  //
                              //
        -0.5f, 0.5f, -0.5f,   //
        0.5f, 0.5f, -0.5f,    //
        0.5f, 0.5f, 0.5f,     //
        0.5f, 0.5f, 0.5f,     //
        -0.5f, 0.5f, 0.5f,    //
        -0.5f, 0.5f, -0.5f    //
    };

    // EQUILATERAL TRIANGLE
    float triangle[] = {
        0.0f, 0.0f,   0.0f,  //
        0.5f, 0.866f, 0.0f,  //
        1.0f, 0.0f,   0.0f,  //
    };                       //

    // PYRAMID
    float pyramid[] = {
        0.0f,  1.0f, 0.0f,  //
        0.5f,  0.0f, 0.5f,  //
        -0.5f, 0.0f, 0.5f,  //

        0.0f,  1.0f, 0.0f,   //
        -0.5f, 0.0f, 0.5f,   //
        -0.5f, 0.0f, -0.5f,  //

        0.0f,  1.0f, 0.0f,   //
        -0.5f, 0.0f, -0.5f,  //
        0.5f,  0.0f, -0.5f,  //

        0.0f,  1.0f, 0.0f,   //
        0.5f,  0.0f, -0.5f,  //
        0.5f,  0.0f, 0.5f,   //

        0.5f,  0.0f, 0.5f,   //
        -0.5f, 0.0f, 0.5f,   //
        -0.5f, 0.0f, -0.5f,  //
        -0.5f, 0.0f, -0.5f,  //
        0.5f,  0.0f, -0.5f,  //
        0.5f,  0.0f, 0.5f    //
    };                       //

    glm::vec3 cubePositions[grid_size][grid_size];
    float depthValues[grid_size][grid_size];
    glm::vec4 mappedColors[grid_size][grid_size];

    unsigned int VBO, VAO;
    cv::VideoCapture capture(filename);
    cv::Mat frame;

    if (!capture.isOpened()) throw "Error when reading steam_avi";

    while (!glfwWindowShouldClose(window)) {
        float vertices[sizeof(cube) / sizeof(float)];

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Non-optimal for speed
        if (shapeMode == 0) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
        } else if (shapeMode == 1) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle,
                         GL_STATIC_DRAW);
        } else if (shapeMode == 2) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid,
                         GL_STATIC_DRAW);
        }

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        ourShader.use();

        // render loop
        // -----------

        // while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        glfwSetKeyCallback(window, key_callback);

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
            0.1f, 1000.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);

        if (is_video == true) {
            capture >> frame;
        }

        for (unsigned int i = 0; i < grid_size; i++) {
            float y = i * -1.0f;
            for (unsigned int j = 0; j < grid_size; j++) {
                float x = j * 1.0f;
                // float depthValue = 0.0f;

                // std::cout << "Display mode: " << displayMode << std::endl;
                if (displayMode == 0) {
                    depthValues[i][j] = 0.0f;
                } else if (displayMode == 1) {
                    if (is_video == true) {
                        depthValues[i][j] = frame.at<cv::Vec3b>(i, j)[2];
                    } else {
                        depthValues[i][j] = rgbaVector[4 * (i * 100 + j)] * 10;
                    }

                } else if (displayMode == 2) {
                    if (is_video == true) {
                        depthValues[i][j] = frame.at<cv::Vec3b>(i, j)[1];
                    } else {
                        depthValues[i][j] =
                            rgbaVector[4 * (i * 100 + j) + 1] * 10;
                    }
                } else if (displayMode == 3) {
                    if (is_video == true) {
                        depthValues[i][j] = frame.at<cv::Vec3b>(i, j)[0];
                    } else {
                        depthValues[i][j] =
                            rgbaVector[4 * (i * 100 + j) + 2] * 10;
                    }

                } else if (displayMode == 4) {
                    depthValues[i][j] = 1.5 * sin(glfwGetTime() * 2 + j * 100);
                } else if (displayMode == 5) {
                    float centre = grid_size / 2;

                    float distFromCentre =
                        sqrt(pow(i - centre, 2.0) + pow(j - centre, 2.0));

                    float maxDistFromCentre =
                        sqrt(pow(centre, 2.0) + pow(centre, 2.0));

                    float rippleTime = glfwGetTime() - startRipple;

                    if (rippleTime > distFromCentre / maxDistFromCentre * 10) {
                        depthValues[i][j] =
                            10 * (maxDistFromCentre - distFromCentre) /
                            maxDistFromCentre * exp(-0.1 * rippleTime) *
                            cos(rippleTime +
                                distFromCentre / maxDistFromCentre * 10);
                    }

                } else if (displayMode > 5 && displayMode < 10) {
                    float x_origin;
                    float y_origin;

                    if (displayMode == 6) {  // key press R
                        x_origin = grid_size / 4;
                        y_origin = grid_size / 4;
                    } else if (displayMode == 7) {  // key press T
                        x_origin = 3 * grid_size / 4;
                        y_origin = grid_size / 4;
                    } else if (displayMode == 8) {  // key press F
                        x_origin = grid_size / 4;
                        y_origin = 3 * grid_size / 4;
                    } else if (displayMode == 9) {  // key press G
                        x_origin = 3 * grid_size / 4;
                        y_origin = 3 * grid_size / 4;
                    }

                    float distFromCentre =
                        sqrt(pow(j - x_origin, 2.0) + pow(i - y_origin, 2.0));
                    // std::cout << "distFromCentre: " << distFromCentre <<
                    // std::endl;
                    float maxDistFromCentre =
                        sqrt(pow(grid_size - x_origin, 2.0) +
                             pow(grid_size - y_origin, 2.0));

                    float rippleTime = glfwGetTime() - startRipple;

                    if (rippleTime > distFromCentre / maxDistFromCentre * 10) {
                        // depth6 = 10 * (maxDistFromCentre - distFromCentre) /
                        // maxDistFromCentre * exp(- 0.1 * rippleTime) *
                        // cos(rippleTime + distFromCentre/maxDistFromCentre *
                        // 10);
                        depthValues[i][j] =
                            10 * (maxDistFromCentre - distFromCentre) /
                            maxDistFromCentre * exp(-0.1 * rippleTime) *
                            cos(rippleTime +
                                distFromCentre / maxDistFromCentre * 10);
                    }
                } else if (displayMode == 10) {
                    float centre = grid_size / 2;

                    float distFromCentre =
                        sqrt(pow(i - centre, 2.0) + pow(j - centre, 2.0));

                    float maxDistFromCentre =
                        sqrt(pow(centre, 2.0) + pow(centre, 2.0));

                    float rippleTime = glfwGetTime() - startRipple;

                    if (rippleTime > distFromCentre / maxDistFromCentre * 10) {
                        depthValues[i][j] +=
                            10 * (maxDistFromCentre - distFromCentre) /
                            maxDistFromCentre * exp(-0.1 * rippleTime) *
                            cos(rippleTime +
                                distFromCentre / maxDistFromCentre * 10);
                    }
                }

                cubePositions[i][j] = glm::vec3(x, y, depthValues[i][j]);
            }
        }

        if (is_video == true) {
            // capture >> frame;
            if (!frame.empty()) {
                for (unsigned int i = 0; i < grid_size; i++) {
                    for (unsigned int j = 0; j < grid_size; j++) {
                        // calculate the model matrix for each object and pass
                        // it to shader before drawing
                        glm::mat4 model =
                            glm::mat4(1.0f);  // make sure to initialize matrix
                                              // to identity matrix first
                        model = glm::translate(model, cubePositions[i][j]);

                        glm::vec3 translation = {-(grid_size / 2 * 1.0f),
                                                 (grid_size / 2 * 1.0f), 0.0f};
                        model = glm::translate(model, translation);

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

                    glm::vec4 color = glm::vec4(pixelRgb[0], pixelRgb[1],
                                                pixelRgb[2], pixelRgb[3]);

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

                    glm::vec3 translation = {-(grid_size / 2 * 1.0f),
                                             (grid_size / 2 * 1.0f), 0.0f};
                    model = glm::translate(model, translation);

                    // float angle = 20.0f * i;
                    // model = glm::rotate(model, glm::radians(angle),
                    //                     glm::vec3(1.0f, 0.3f, 0.5f));

                    glm::vec4 color = mappedColors[i][j];

                    // only render if alpha > 0
                    if (color[3] == 0) {
                        if (!transparent) {
                            ourShader.setVec4("ourColor", color);
                            ourShader.setMat4("model", model);
                            glDrawArrays(GL_TRIANGLES, 0, 36);
                        }
                    } else {
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

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods) {
    if (action == GLFW_RELEASE) return;  // only handle press events

    // Toggle Transparency
    if (key == GLFW_KEY_Y) transparent = !transparent;
    // Change Effect
    if (key == GLFW_KEY_I) displayMode = (displayMode + 4) % 5;
    if (key == GLFW_KEY_O) displayMode = (displayMode + 1) % 5;

    // Change Shape
    if (key == GLFW_KEY_K) shapeMode = (shapeMode + 2) % 3;
    if (key == GLFW_KEY_L) shapeMode = (shapeMode + 1) % 3;

    if (key == GLFW_KEY_0) displayMode = 0;
    if (key == GLFW_KEY_1) displayMode = 1;
    if (key == GLFW_KEY_2) displayMode = 2;
    if (key == GLFW_KEY_3) displayMode = 3;
    if (key == GLFW_KEY_4) displayMode = 4;

    // Ripples
    if (key == GLFW_KEY_5) {
        startRipple = glfwGetTime();
        displayMode = 5;
    }
    if (key == GLFW_KEY_R) {
        startRipple = glfwGetTime();
        displayMode = 6;
    }
    if (key == GLFW_KEY_T) {
        startRipple = glfwGetTime();
        displayMode = 7;
    }
    if (key == GLFW_KEY_F) {
        startRipple = glfwGetTime();
        displayMode = 8;
    }
    if (key == GLFW_KEY_G) {
        startRipple = glfwGetTime();
        displayMode = 9;
    }

    if (key == GLFW_KEY_MINUS) displayMode = 10;

    std::cout << "displayMode: " << displayMode << std::endl;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * camera_speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * camera_speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * camera_speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * camera_speed);
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

// glfw: whenever the mouse left button is clicked
// -------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // double xpos, ypos;
        // getting cursor position
        // glfwGetCursorPos(window, &xpos, &ypos);
        // std::cout << "Cursor Position at (" << xpos << " : " << ypos <<
        //")"           << std::endl;

        leftMouseButtonHold = true;
    } else {
        leftMouseButtonHold = false;
        firstMouse = true;
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (leftMouseButtonHold) {
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
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
