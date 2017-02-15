#ifndef APP_APP_H
#define APP_APP_H

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <array>
#include <iostream>

namespace app {
    template <class Kernel>
    class App {
       public:
        using Ptr = std::unique_ptr<App, std::add_pointer_t<void(App*)>>;

       private:
        App(GLFWwindow* window, const Kernel& kernel)
        : window_(window, glfwDestroyWindow)
        , kernel_(kernel)
        {
        }

        static Ptr Create(GLFWwindow* window, const Kernel& kernel)
        {
            // Terminate the app using GLFW API on free
            return Ptr(new App(window, kernel), [](App* app) { glfwTerminate(); });
        }

        static App* Get(GLFWwindow* window)
        {
            return static_cast<App*>(glfwGetWindowUserPointer(window));
        }

       public:
        static Ptr Create(const Kernel& kernel,
                          const std::string& title,
                          uint32_t width,
                          uint32_t height)
        {
            glfwSetErrorCallback(
                [](int error, const char* description) { std::cerr << description << "\n"; });

            if (!glfwInit()) {
                return Create(nullptr, Kernel());
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            return Create(glfwCreateWindow(width, height, title.data(), nullptr, nullptr), kernel);
        }

        int32_t Run()
        {
            if (!window_) {
                return 1;
            }

            glfwSetWindowUserPointer(window(), this);
            glfwMakeContextCurrent(window());
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            glfwSwapInterval(1);

            kernel().Init(this);

            glfwSetKeyCallback(window(),
                               [](auto* window, auto key, auto scancode, auto action, auto mods) {
                                   auto* app = Get(window);
                                   app->kernel().Key(app, key, scancode, action, mods);
                               });

            glfwSetMouseButtonCallback(window(),
                                       [](auto* window, auto button, auto action, auto mods) {
                                           auto* app = Get(window);
                                           app->kernel().Mouse(app, button, action, mods);
                                       });

            glfwSetCursorPosCallback(window(), [](auto* window, auto xpos, auto ypos) {
                auto* app = Get(window);
                app->kernel().Cursor(app, xpos, ypos);
            });

            while (!glfwWindowShouldClose(window())) {
                kernel().Draw(this);
                glfwSwapBuffers(window());
                glfwPollEvents();
            }
            return 0;
        }

        std::array<int32_t, 2> WindowSize()
        {
            int32_t width, height;
            glfwGetFramebufferSize(window(), &width, &height);
            return {width, height};
        }

       private:
        // Get the raw GLFW window pointer
        GLFWwindow* window() { return window_.get(); }
        Kernel& kernel() { return kernel_; }
        // Destroy window with GLFW API on free
        std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window_;
        Kernel kernel_;
    };
}

#endif
