#ifndef COMMON_H
#define COMMON_H

#include <GLFW/glfw3.h>

unsigned int compile_vertex_shader(const char *path);
unsigned int compile_fragment_shader(const char *path);
unsigned int create_shader_program(unsigned int shaders[], int len);

void handle_window_input(GLFWwindow *window);

#endif
