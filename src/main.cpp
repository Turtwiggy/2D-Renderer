
#include <iostream>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/freetype/imgui_freetype.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Core.hpp"

/* Window Data */
const char *WINDOW_NAME = "Gamey Game Game Game";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

/* Matrices */
glm::vec3 cam_position = glm::vec3(0.0f, 1.0f, 1.2f);
glm::vec3 cam_look_at = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 world_matrix = glm::mat4(1.0f);
glm::mat4 view_matrix = glm::lookAt(cam_position, cam_look_at, cam_up);
glm::mat4 projection_matrix = glm::perspectiveFov(glm::radians(60.0f), float(WINDOW_WIDTH), float(WINDOW_HEIGHT), 0.1f, 10.0f);

void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* init()
{
    glfwSetErrorCallback(glfw_error_callback);

    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("Could not init glfw");

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        ENGINE_CORE_ASSERT(false, "GLFW didn't init!");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Bad Glew");

    /* Set the viewport */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    return window;
}

void render(float time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw our scene */
    world_matrix = glm::rotate(glm::mat4(1.0f), time * glm::radians(-90.0f), glm::vec3(0, 1, 0));

    // level.window_pos = {wxpos, wypos};
    // vec2f screen_absolute_pos = {wxpos, wypos};
    // auto mpos = (vec2f){io.MousePos.x, io.MousePos.y};

    // shader->setUniformMatrix4fv("world", world_matrix);
    // shader->setUniformMatrix3fv("normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));
}

void game_logic()
{

}

void update(GLFWwindow* window)
{
    float startTime = static_cast<float>(glfwGetTime());
    float newTime = 0.0f;
    float gameTime = 0.0f;
    int display_w = 0;
    int display_h = 0;
    int wxpos = 0;
    int wypos = 0;

    glfwGetFramebufferSize(window, &display_w, &display_h);
    glfwGetWindowPos(window, &wxpos, &wypos);
    ImGuiIO &io = ImGui::GetIO();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        printf("Hello\n");

        /* Update game time 
        
        value */
        newTime = static_cast<float>(glfwGetTime());
        gameTime = newTime - startTime;

        /* Poll for and process events */
        glfwPollEvents();
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        printf("Im here\n");

        if (ImGui::IsKeyDown(GLFW_KEY_N))
        {
            std::cout << ImGui::GetIO().DeltaTime << std::endl;
        }

        glfwGetWindowPos(window, &wxpos, &wypos);

        ImGui::Begin("Test");

        if (ImGui::Button("BUTTON"))
        {
            printf("james is poo\n");
        }

        ImGui::End();

        printf("Here2\n");

        //This should include "imgui rendering" and any game logic
        game_logic();

        ImGui::Render();

        glfwMakeContextCurrent(window);
        glViewport(0, 0, display_w, display_h);

        /* Render here */
        render(gameTime);

        printf("Here3\n");

        //Imgui actually render
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }
}

void init_imgui(bool no_viewports, const char *glsl_version, GLFWwindow* window)
{
    ImFontAtlas atlas = {};
    ImGui::CreateContext(&atlas);

    printf("ImGui create context\n");
    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (!no_viewports)
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    //ImGui::SetStyleLinearColor(true);

    ImGui::PushSrgbStyleColor(ImGuiCol_WindowBg, ImVec4(30 / 255., 30 / 255., 30 / 255., 1));

    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 0;
    style.WindowRounding = 0;
    style.ChildRounding = 0;
    style.ChildBorderSize = 0;
    style.FrameBorderSize = 0;
    style.WindowBorderSize = 1;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    ImGuiFreeType::BuildFontAtlas(&atlas, 0, 1);

    //ImGui::SetStyleLinearColor(true);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    printf("Init ogl\n");
}

int main(int argc, char *argv[])
{
    bool no_viewports = false;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            std::string sarg = argv[i];

            if (sarg == "-noviewports" || sarg == "-noviewport")
            {
                no_viewports = true;

                printf("Viewports are disabled\n");
            }
        }
    }
    
    GLFWwindow *window = init();

    const char *glsl_version = "#version 130";
    init_imgui(no_viewports, glsl_version, window);

    std::cout << "Hello world!";

    update(window);

    glfwTerminate();

    return 0;
}
