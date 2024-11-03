#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <freetype2/ft2build.h>
#include "./common.h"
#include FT_FREETYPE_H

// #define DEBUG

#include "aids.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

struct Point {
    unsigned int x, y;
};

struct Character {
    unsigned int texture_id;  // ID handle of the glyph texture
    struct Point size;       // Size of glyph
    struct Point bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

void renderText() {
}

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    /*
    const char *foo = glfwGetVersionString();
    printf("HELLO: %s\n", foo);
    fflush(stdout);
    */
    
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init FreeType Library");
        exit(EXIT_FAILURE);
    }
    
    FT_Face face;

    if (FT_New_Face(ft, "RobotoCondensed-Regular.ttf", 0, &face)) {
        fprintf(stderr, "Could not init FreeType Library");
        exit(EXIT_FAILURE);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
        fprintf(stderr, "Failed to load FreeType glyph");
        exit(EXIT_FAILURE);
    }

    GLFWwindow *window = glfwCreateWindow(
        800,
        600,
        "GLFWWindow",
        NULL,
        NULL
    );

    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    struct Character characters[128] = {0};

    for (unsigned char c = 0; c < 128; c += 1) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "Failed to load glyph");
            continue;
        }

        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        characters[c] = (struct Character) {
            .texture_id = texture,
            .size = {
                .x = face->glyph->bitmap.width,
                .y = face->glyph->bitmap.rows
            },
            .bearing = {
                .x = face->glyph->bitmap_left,
                .y = face->glyph->bitmap_top
            },
            .advance = face->glyph->advance.x
        };
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

    unsigned int vertex_shader = compile_vertex_shader("./shaders/main.vert");
    unsigned int fragment_shader = compile_fragment_shader("./shaders/main.frag");

    unsigned int shaders[] = { vertex_shader, fragment_shader };

    unsigned int shader_program = create_shader_program(
        shaders,
        sizeof(shaders) / sizeof(unsigned int)
    );

    float projection[16] = {
        2.0f / 800.0f, 0.0f,           0.0f, -1.0f,
        0.0f,          2.0f / 600.0f,  0.0f, -1.0f,
        0.0f,          0.0f,          -1.0f,  0.0f,
        0.0f,          0.0f,           0.0f,  1.0f
    };

    GLuint projectionLoc = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    int viewport_width, viewport_height;

    glfwGetWindowSize(window, &viewport_width, &viewport_height);

    int width = viewport_width / 4;
    int x = viewport_width / 2 - width / 2,
        y = viewport_height / 2 - width / 2;
    int border_thickness = 25;

    float gborder_width = remap(
        0, viewport_width,
        0.0f, 2.0f,
        border_thickness
    );

    float gborder_height = remap(
        0, viewport_height,
        0.0f, 2.0f,
        border_thickness
    );

    float gx = remap(
        0, viewport_width,
        -1.0f, 1.0f,
        x
    );

    float gy = remap(
        0, viewport_height,
        -1.0f, 1.0f,
        y
    );

    float gwidth = remap(
        0, viewport_width,
        0.0f, 2.0f,
        width
    );

    float gheight = remap(
        0, viewport_height,
        0.0f, 2.0f,
        width
    );

    float top = gy + gheight,
          bottom = gy,
          left = gx,
          right = gx + gwidth;

    float vertices[] = {
        // Bottom border
        left, bottom + gborder_height, right, bottom + gborder_height,
        left, bottom,                  right, bottom, 

        // Left border
        left, top - gborder_height,    left + gborder_width, top - gborder_height,
        left, bottom + gborder_height, left + gborder_width, bottom + gborder_height,

        // Top border
        left, top,                  right, top,
        left, top - gborder_height, right, top - gborder_height,


        // Left border
        right, top - gborder_height,    right - gborder_width, top - gborder_height,
        right, bottom + gborder_height, right - gborder_width, bottom + gborder_height,
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,

        4, 5, 6,
        5, 6, 7,

        8,  9, 10,
        9, 10, 11,

        12, 13, 14,
        13, 14, 15,
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // BIND VAO
    glBindVertexArray(VAO);

    // BIND Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void *) 0
    );
    glEnableVertexAttribArray(0);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Unbind VAO
    glBindVertexArray(0);


    glUseProgram(shader_program);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (! glfwWindowShouldClose(window)) {
        // Handle inputs
        process_input(window);


        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);


        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
    glfwTerminate();

    return 0;
}
