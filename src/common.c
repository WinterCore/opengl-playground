
#include <stdlib.h>
#include <stdbool.h>

#include "./aids.h"
#include "glad/glad.h"

#include "./common.h"

unsigned int compile_vertex_shader(const char *path) {
    Kyle shader_source = kyle_from_file(path);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    int shader_lengths[] = { shader_source.length };
    glShaderSource(vertex_shader, 1, &shader_source.data, shader_lengths);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        exit(EXIT_FAILURE);
    }

    DEBUG_PRINT("VERTEX SHADER COMPILED SUCCESSFULLY");

    return vertex_shader;
}

unsigned int compile_fragment_shader(const char *path) {
    Kyle shader_source = kyle_from_file(path);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    int shader_lengths[] = { shader_source.length };
    glShaderSource(fragment_shader, 1, &shader_source.data, shader_lengths);
    glCompileShader(fragment_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAG::COMPILATION_FAILED\n%s\n", infoLog);
        exit(EXIT_FAILURE);
    }

    DEBUG_PRINT("FRAGMENT SHADER COMPILED SUCCESSFULLY");

    return fragment_shader;
}

unsigned int create_shader_program(unsigned int shaders[], int len) {
    unsigned int shader_program = glCreateProgram();

    for (int i = 0; i < len; i += 1) {
        glAttachShader(shader_program, shaders[i]);
    }

    glLinkProgram(shader_program);

    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (! success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINK_ERROR\n%s\n", infoLog);
        exit(EXIT_FAILURE);
    }

    DEBUG_PRINT("SHADER PROGRAM CREATED SUCCESSFULLY");

    return shader_program;
}

void handle_window_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}
