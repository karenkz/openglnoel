#include "Application.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

int Application::run()
{
	float clearColor[3] = { 0.1, 0.9,0.3 };
	glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.f);

    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        // Put here rendering code
        const auto viewportSize = m_GLFWHandle.framebufferSize();
        glViewport(0, 0, viewportSize.x, viewportSize.y);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projMatrix = glm::perspective(70.f, float(viewportSize.x) / viewportSize.y, 0.01f, 100.f);
        glm::mat4 viewMatrix = m_viewController.getViewMatrix();

        glUniform3fv(m_uDirectionalLightDirLocation, 1, glm::value_ptr(glm::vec3(viewMatrix * glm::vec4(glm::normalize(m_DirLightDirection), 0))));
        glUniform3fv(m_uDirectionalLightIntensityLocation, 1, glm::value_ptr(m_DirLightColor * m_DirLightIntensity));

        glUniform3fv(m_uPointLightPositionLocation, 1, glm::value_ptr(glm::vec3(viewMatrix * glm::vec4(m_PointLightPosition, 1))));
        glUniform3fv(m_uPointLightIntensityLocation, 1, glm::value_ptr(m_PointLightColor * m_PointLightIntensity));


            glm::mat4 modelMatrix = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(-2, 0, 0)), 0.2f * float(seconds), glm::vec3(0, 1, 0));

            glm::mat4 mvMatrix = viewMatrix * modelMatrix;
            glm::mat4 mvpMatrix = projMatrix * mvMatrix;
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mvMatrix));

            glUniformMatrix4fv(m_uModelViewProjMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(m_uModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
            glUniformMatrix4fv(m_uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

            glUniform3fv(m_uKdLocation, 1, glm::value_ptr(m_CubeKd));

            glBindVertexArray(cubeVAO);
            glDrawElements(GL_TRIANGLES, cube.indexBuffer.size(), GL_UNSIGNED_INT, nullptr);


        /*
            glm::mat4 modelMatrix = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(2, 0, 0)), 0.2f * float(seconds), glm::vec3(0, 1, 0));

            glm::mat4 mvMatrix = viewMatrix * modelMatrix;
            glm::mat4 mvpMatrix = projMatrix * mvMatrix;
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mvMatrix));

            glUniformMatrix4fv(m_uModelViewProjMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(m_uModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
            glUniformMatrix4fv(m_uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

            glUniform3fv(m_uKdLocation, 1, glm::value_ptr(m_SphereKd));

            glBindVertexArray(sphereVAO);
            glDrawElements(GL_TRIANGLES, sphere.indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
        */

        // GUI code:
		glmlv::imguiNewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

          if (ImGui::ColorEdit3("clearColor", clearColor)) {
						glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.f);
            }
            if (ImGui::CollapsingHeader("Directional Light"))
            {
                ImGui::ColorEdit3("DirLightColor", glm::value_ptr(m_DirLightColor));
                ImGui::DragFloat("DirLightIntensity", &m_DirLightIntensity, 0.1f, 0.f, 100.f);
                if (ImGui::DragFloat("Phi Angle", &m_DirLightPhiAngleDegrees, 1.0f, 0.0f, 360.f) ||
                    ImGui::DragFloat("Theta Angle", &m_DirLightThetaAngleDegrees, 1.0f, 0.0f, 180.f)) {
                    m_DirLightDirection = computeDirectionVector(glm::radians(m_DirLightPhiAngleDegrees), glm::radians(m_DirLightThetaAngleDegrees));
                }
            }

            if (ImGui::CollapsingHeader("Point Light"))
            {
                ImGui::ColorEdit3("PointLightColor", glm::value_ptr(m_PointLightColor));
                ImGui::DragFloat("PointLightIntensity", &m_PointLightIntensity, 0.1f, 0.f, 16000.f);
                ImGui::InputFloat3("Position", glm::value_ptr(m_PointLightPosition));
            }

            if (ImGui::CollapsingHeader("Materials"))
            {
                ImGui::ColorEdit3("Cube Kd", glm::value_ptr(m_CubeKd));
                //ImGui::ColorEdit3("Sphere Kd", glm::value_ptr(m_SphereKd));
            }

            ImGui::End();
        }


		glmlv::imguiRenderFrame();

        /* Poll for and process events */
        glfwPollEvents();

        /* Swap front and back buffers*/
        m_GLFWHandle.swapBuffers();

        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {
            m_viewController.update(float(ellapsedTime));
        }
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


    const GLint positionAttrLocation = 0;
    const GLint normalAttrLocation = 1;
    const GLint texCoordsAttrLocation = 2;

    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeIBO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereIBO);

    cube = glmlv::makeCube();
    sphere = glmlv::makeSphere(32);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferStorage(GL_ARRAY_BUFFER, cube.vertexBuffer.size() * sizeof(glmlv::Vertex3f3f2f), cube.vertexBuffer.data(), 0);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferStorage(GL_ARRAY_BUFFER, sphere.vertexBuffer.size() * sizeof(glmlv::Vertex3f3f2f), sphere.vertexBuffer.data(), 0);

    glBindBuffer(GL_ARRAY_BUFFER, cubeIBO);
    glBufferStorage(GL_ARRAY_BUFFER, cube.indexBuffer.size() * sizeof(uint32_t), cube.indexBuffer.data(), 0);

    glBindBuffer(GL_ARRAY_BUFFER, sphereIBO);
    glBufferStorage(GL_ARRAY_BUFFER, sphere.indexBuffer.size() * sizeof(uint32_t), sphere.indexBuffer.data(), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const auto initVAO = [positionAttrLocation, normalAttrLocation, texCoordsAttrLocation](GLuint& vao, GLuint vbo, GLuint ibo)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(positionAttrLocation);
        glEnableVertexAttribArray(normalAttrLocation);
        glEnableVertexAttribArray(texCoordsAttrLocation);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(positionAttrLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*)offsetof(glmlv::Vertex3f3f2f, position));
        glVertexAttribPointer(normalAttrLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*)offsetof(glmlv::Vertex3f3f2f, normal));
        glVertexAttribPointer(texCoordsAttrLocation, 2, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*)offsetof(glmlv::Vertex3f3f2f, texCoords));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBindVertexArray(0);
    };

    initVAO(cubeVAO, cubeVBO, cubeIBO);
    initVAO(sphereVAO, sphereVBO, sphereIBO);

    glEnable(GL_DEPTH_TEST);

    m_program = glmlv::compileProgram({ m_ShadersRootPath / m_AppName / "forward.vs.glsl", m_ShadersRootPath / m_AppName / "forward.fs.glsl" });
    m_program.use();

    m_uModelViewProjMatrixLocation = glGetUniformLocation(m_program.glId(), "uModelViewProjMatrix");
    m_uModelViewMatrixLocation = glGetUniformLocation(m_program.glId(), "uModelViewMatrix");
    m_uNormalMatrixLocation = glGetUniformLocation(m_program.glId(), "uNormalMatrix");


    m_uDirectionalLightDirLocation = glGetUniformLocation(m_program.glId(), "uDirectionalLightDir");
    m_uDirectionalLightIntensityLocation = glGetUniformLocation(m_program.glId(), "uDirectionalLightIntensity");

    m_uPointLightPositionLocation = glGetUniformLocation(m_program.glId(), "uPointLightPosition");
    m_uPointLightIntensityLocation = glGetUniformLocation(m_program.glId(), "uPointLightIntensity");

    m_uKdLocation = glGetUniformLocation(m_program.glId(), "uKd");

    m_viewController.setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
}
