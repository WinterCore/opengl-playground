#include "glad/glad.h"

#include "./simple_borders.h"
#include "aids.h"
#include "./common.h"

void simple_borders(GLFWwindow *window) {
    unsigned int vertex_shader = compile_vertex_shader("./shaders/simple_borders/main.vert");
    unsigned int fragment_shader = compile_fragment_shader("./shaders/simple_borders/main.frag");

    unsigned int shaders[] = { vertex_shader, fragment_shader };

    unsigned int shader_program = create_shader_program(
        shaders,
        sizeof(shaders) / sizeof(unsigned int)
    );

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    
    int viewport_width, viewport_height;

    glfwGetWindowSize(window, &viewport_width, &viewport_height);
    
    int width = viewport_width / 4;
    int x = viewport_width / 2 - width / 2,
        y = viewport_height / 2 - width / 2;
    int border_thickness = 4;

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

    float vertices[] = {
        // Top border
        gx, gy,                  gx + gwidth, gy,
        gx, gy + gborder_height, gx + gwidth, gy + gborder_height
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
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
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) 0
    );
    glEnableVertexAttribArray(0);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Unbind VAO
    glBindVertexArray(0);


    // TODO: Maybe it needs to be moved into the loop?
    glUseProgram(shader_program);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (! glfwWindowShouldClose(window)) {
        // Handle inputs
        handle_window_input(window);


        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
}
