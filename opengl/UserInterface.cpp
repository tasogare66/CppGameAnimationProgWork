#include "UserInterface.h"
#include <string>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void UserInterface::init(const OGLRenderData& renderData)
{
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(renderData.rdWindow, true);

  const char* glslVersion = "#version 460 core";
  ImGui_ImplOpenGL3_Init(glslVersion);
}

void UserInterface::createFrame(const OGLRenderData& renderData)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiWindowFlags imguiWindowFlags = 0;
  //imguiWindowFlags |= ImGuiWindowFlags_NoCollapse;
  //imguiWindowFlags |= ImGuiWindowFlags_NoResize;
  //imguiWindowFlags |= ImGuiWindowFlags_NoMove;

  ImGui::SetNextWindowBgAlpha(0.8f);

  ImGui::Begin("Control", nullptr, imguiWindowFlags);

  static double newFps = 0.0;
  /* avoid inf values (division by zero) */
  if (renderData.rdFrameTime > 0.0) {
    newFps = 1.0f / renderData.rdFrameTime;
  }
  /* make an averge value to avoid jumps */
  mFramesPerSecond = (mAveragingAlpha * mFramesPerSecond) + (1.0f - mAveragingAlpha) * newFps;

  ImGui::Text("FPS:");
  ImGui::SameLine();
  ImGui::Text("%s", std::to_string(mFramesPerSecond).c_str());

  ImGui::Separator();

  ImGui::Text("Triangles:");
  ImGui::SameLine();
  ImGui::Text("%s", std::to_string(renderData.rdTriangleCount).c_str());

  std::string windowDims = std::to_string(renderData.rdWidth) + "x" + std::to_string(renderData.rdHeight);
  ImGui::Text("Window Dimensions:");
  ImGui::SameLine();
  ImGui::Text("%s", windowDims.c_str());

  std::string imgWindowPos = std::to_string(static_cast<int>(ImGui::GetWindowPos().x)) + "/" + std::to_string(static_cast<int>(ImGui::GetWindowPos().y));
  ImGui::Text("ImGui Window Position:");
  ImGui::SameLine();
  ImGui::Text("%s", imgWindowPos.c_str());

  ImGui::End();
}

void UserInterface::render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::cleanup()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
