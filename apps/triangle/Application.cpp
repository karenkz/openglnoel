#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
    glm::vec2 position;
    glm::vec3 color;

    Vertex(glm::vec2 position, glm::vec3 color):
        position(position), color(color)
    {}
};

int Application::run()
{
  const auto program = glmlv::compileProgram({ m_ShadersRootPath / m_AppName / "forward.vs.glsl",
    m_ShadersRootPath / m_AppName / "forward.fs.glsl"});
    program.use();

  glClearColor(1, 0, 0, 1);
  Vertex triangleVertices[] = {
      Vertex { glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0) },
      Vertex { glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0) },
      Vertex { glm::vec2(0, 0.5), glm::vec3(0, 0, 1) }
  };

  GLuint m_triangleVBO, m_triangleVAO;

  glGenBuffers(1, &m_triangleVBO);

  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

  glBufferStorage(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &m_triangleVAO);

  // Vertex attrib locations are defined in the vertex shader (we can also use glGetAttribLocation(program, attribname) with attribute names after program compilation in order to get these numbers)
  const GLint positionAttrLocation = 0;
  const GLint colorAttrLocation = 1;

  glBindVertexArray(m_triangleVAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

  glEnableVertexAttribArray(positionAttrLocation);
  glVertexAttribPointer(positionAttrLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, position));

  glEnableVertexAttribArray(colorAttrLocation);
  glVertexAttribPointer(colorAttrLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();


        // Put here rendering code
		const auto fbSize = m_GLFWHandle.framebufferSize();

    program.use();
		glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);


        // GUI code:
		glmlv::imguiNewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

		glmlv::imguiRenderFrame();

        glfwPollEvents(); // Poll for and process events


        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {

            // Put here code to handle user interactions
        }

		m_GLFWHandle.swapBuffers(); // Swap front and back buffers

    }

    return 0;
}


Application::Application(int argc, char** argv):
    m_AppPath { glmlv::fs::path{ argv[0] } },
    m_AppName { m_AppPath.stem().string() },
    m_ImGuiIniFilename { m_AppName + ".imgui.ini" },
    m_ShadersRootPath { m_AppPath.parent_path() / "shaders" }

{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file
    // Put here code to run before rendering loop

}
