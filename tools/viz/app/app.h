#ifndef VIZ_APP_APP_H
#define VIZ_APP_APP_H

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on
#include "draw/context.h"
#include "search_paths.h"
#include <array>
#include <iostream>
#include <vector>

namespace viz {
    namespace app {
        class App {
           public:
            using Ptr = std::unique_ptr<App, std::add_pointer_t<void(App*)>>;

           private:
            App(GLFWwindow* window)
            : window_(window, glfwDestroyWindow)
            {
            }

            static Ptr Create(GLFWwindow* window)
            {
                // Terminate the app using GLFW API on free
                return Ptr(new App(window), [](App* app) { glfwTerminate(); });
            }

            template <class Kernel>
            static Kernel* Get(GLFWwindow* window)
            {
                return static_cast<Kernel*>(glfwGetWindowUserPointer(window));
            }

           public:
            template <class F>
            static int32_t Create(const std::string& title, uint32_t width, uint32_t height, F&& f)
            {
                glfwSetErrorCallback(
                    [](int error, const char* description) { std::cerr << description << "\n"; });

                if (!glfwInit()) {
                    return 1;
                }

                // Configure window
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

                auto app = App(glfwCreateWindow(width, height, title.data(), nullptr, nullptr));

                // Setup window context
                glfwMakeContextCurrent(app.window());
                gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                glfwSwapInterval(1);

                return f(app);
            }

            template <class Kernel>
            int32_t Run(Kernel& kernel)
            {
                if (!window_) {
                    return 1;
                }

                glfwSetWindowUserPointer(window(), &kernel);
                glfwSetKeyCallback(
                    window(), [](auto* window, auto key, auto scancode, auto action, auto mods) {
                        auto* kernel = Get<Kernel>(window);
                        kernel->Key(key, scancode, action, mods);
                    });
                glfwSetMouseButtonCallback(window(),
                                           [](auto* window, auto button, auto action, auto mods) {
                                               auto* kernel = Get<Kernel>(window);
                                               kernel->Mouse(button, action, mods);
                                           });
                glfwSetCursorPosCallback(window(), [](auto* window, auto xpos, auto ypos) {
                    auto* kernel = Get<Kernel>(window);
                    kernel->Cursor(xpos, ypos);
                });
                glfwSetScrollCallback(window(), [](auto* window, auto xoffset, auto yoffset) {
                    auto* kernel = Get<Kernel>(window);
                    kernel->Scroll(xoffset, yoffset);
                });

                while (!glfwWindowShouldClose(window())) {
                    kernel.Draw();
                    glfwSwapBuffers(window());
                    glfwPollEvents();
                }
                return 0;
            }

            std::array<uint32_t, 2> WindowSize()
            {
                int32_t width, height;
                glfwGetFramebufferSize(window(), &width, &height);
                return {uint32_t(width), uint32_t(height)};
            }

            void AddSearchPath(filesystem::path path) { return search_paths().AddSearchPath(path); }
            filesystem::path FindFile(filesystem::path file_name)
            {
                return search_paths().FindFile(file_name);
            }

            std::string LoadFile(filesystem::path file_name)
            {
                return search_paths().LoadFile(file_name);
            }

            void LoadProgram(const std::string& name)
            {
                return draw::Context::Get().RegisterProgram(
                    name,
                    [&](const auto& file_name) { return LoadFile(file_name); },
                    [&](const auto& file_name) { return FindFile(file_name).is_file(); });
            }

           private:
            GLFWwindow* window() { return window_.get(); }
            SearchPaths& search_paths() { return search_paths_; }
            // Destroy window with GLFW API on free
            std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window_;
            SearchPaths search_paths_;
        };
    }
}

#endif
