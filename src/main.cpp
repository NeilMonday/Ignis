
#include "Window.h"
#include "Renderer.h"
#include "ImGuiLayer.h"
#include "Scene.h"
#include "Input.h"
#include "Camera.h"

#include <imgui.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

static void DrawAxesOverlay(const Camera& cam) {
    ImDrawList* dl = ImGui::GetForegroundDrawList();
    ImGuiViewport* vp = ImGui::GetMainViewport();
    const float box = 96.0f;
    const float pad = 12.0f;
    ImVec2 topRight = ImVec2(vp->WorkPos.x + vp->WorkSize.x - box - pad, vp->WorkPos.y + pad);
    ImVec2 center = ImVec2(topRight.x + box * 0.5f, topRight.y + box * 0.5f);
    dl->AddRectFilled(topRight, ImVec2(topRight.x + box, topRight.y + box), IM_COL32(0,0,0,90), 8.0f);
    dl->AddRect(topRight, ImVec2(topRight.x + box, topRight.y + box), IM_COL32(255,255,255,60), 8.0f);
    auto drawAxis = [&](ImU32 col, const glm::vec3& axis, const char* label){
        glm::mat3 R = cam.rotationMatrix();
        glm::vec3 camSpace = glm::transpose(R) * axis;
        glm::vec2 dir = glm::normalize(glm::vec2(camSpace.x, -camSpace.y));
        float len = 32.0f * (camSpace.z > 0 ? 1.0f : 0.6f);
        ImVec2 end = ImVec2(center.x + dir.x * len, center.y + dir.y * len);
        dl->AddLine(center, end, col, 2.0f);
        dl->AddText(ImVec2(end.x + 4, end.y - 4), col, label);
    };
    drawAxis(IM_COL32(255,80,80,255),  glm::vec3(1,0,0), "X");
    drawAxis(IM_COL32(80,255,80,255),  glm::vec3(0,1,0), "Y");
    drawAxis(IM_COL32(80,160,255,255), glm::vec3(0,0,1), "Z");
}

int main() {
    try {
        Window window(1600, 900, "Ignis");
        Renderer renderer(window);
        ImGuiLayer imgui(window, renderer);

        Scene scene;
        scene.create("Camera");
        scene.create("Triangle");

        GLFWwindow* glfwWin = window.handle();
        Input input(glfwWin);
        Camera camera;
        double lastTime = glfwGetTime();

        while(!window.shouldClose()) {
            window.pollEvents();
            double now = glfwGetTime();
            float dt = static_cast<float>(now - lastTime);
            lastTime = now;

            input.beginFrame();
            bool rmb = input.mouseDown(GLFW_MOUSE_BUTTON_RIGHT);
            if (rmb) glfwSetInputMode(glfwWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else     glfwSetInputMode(glfwWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            if (glfwGetKey(glfwWin, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
            camera.update(dt, input, rmb);

            renderer.beginFrame();
            imgui.begin();

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            if (ImGui::Begin("Scene")) {
                static char nameBuf[128] = "NewObject";
                ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf));
                if (ImGui::Button("Add GameObject")) scene.create(nameBuf);
                ImGui::Separator();
                int idx = 0;
                for (auto& obj : scene.objects()) {
                    ImGui::PushID(idx++);
                    if (ImGui::TreeNode(obj.name.c_str())) {
                        ImGui::DragFloat3("Position", &obj.position.x, 0.1f);
                        ImGui::DragFloat3("Rotation", &obj.rotation.x, 0.5f);
                        ImGui::DragFloat3("Scale",    &obj.scale.x,    0.1f);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }
            }
            ImGui::End();

            if (ImGui::Begin("Camera")) {
                ImGui::SliderFloat3("Position", &camera.position[0], -50.0f, 50.0f);
                ImGui::SliderFloat("Yaw",   &camera.yaw,   -180.0f, 180.0f);
                ImGui::SliderFloat("Pitch", &camera.pitch,  -89.0f,  89.0f);
                ImGui::SliderFloat("FOV",   &camera.fov,     30.0f, 110.0f);
                ImGui::SliderFloat("Speed", &camera.moveSpeed, 0.5f, 50.0f);
                ImGui::SliderFloat("Look Sensitivity", &camera.lookSensitivity, 0.01f, 1.0f);
            }
            ImGui::End();

            DrawAxesOverlay(camera);
            imgui.end(renderer.currentCommandBuffer());
            renderer.endFrame();
        }
        renderer.waitIdle();
    } catch (const std::exception& e) {
        spdlog::error("Fatal: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
