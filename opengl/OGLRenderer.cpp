#include "OGLRenderer.h"
#include "Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui_impl_glfw.h>

OGLRenderer::OGLRenderer(GLFWwindow* window)
  : mRenderData(window)
{}

bool OGLRenderer::init(uint32_t width, uint32_t height)
{
  /* required for perspective */
  mRenderData.rdWidth = width;
  mRenderData.rdHeight = height;

  /* initalize GLAD */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Logger::log(1, "%s error: failed to initialize GLAD\n", __FUNCTION__);
    return false;
  }

  if (!GLAD_GL_VERSION_4_6) {
    Logger::log(1, "%s error: failed to get at least OpenGL 4.6\n", __FUNCTION__);
    return false;
  }

  if (!mFramebuffer.init(width, height)) {
    Logger::log(1, "%s error: could not init Framebuffer\n", __FUNCTION__);
    return false;
  }
  Logger::log(1, "%s: framebuffer succesfully initialized\n", __FUNCTION__);

  if (!mTex.loadTexture("textures/crate.png")) {
    Logger::log(1, "%s: texture loading failed\n", __FUNCTION__);
    return false;
  }
  Logger::log(1, "%s: texture successfully loaded\n", __FUNCTION__);

  mVertexBuffer.init();
  Logger::log(1, "%s: vertex buffer successfully created\n", __FUNCTION__);

  mUniformBuffer.init();
  Logger::log(1, "%s: uniform buffer successfully created\n", __FUNCTION__);

  if (!mBasicShader.loadShaders("shader/basic.vert", "shader/basic.frag")) {
    Logger::log(1, "%s: shader loading failed\n", __FUNCTION__);
    return false;
  }
  if (!mChangedShader.loadShaders("shader/changed.vert", "shader/changed.frag")) {
    Logger::log(1, "%s: shader loading failed\n", __FUNCTION__);
    return false;
  }
  Logger::log(1, "%s: shaders succesfully loaded\n", __FUNCTION__);

  mUserInterface.init(mRenderData);
  Logger::log(1, "%s: user interface initialized\n", __FUNCTION__);

  /* add backface culling and depth test already here */
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  mFrameTimer.start();

  return true;
}

void OGLRenderer::setSize(uint32_t width, uint32_t height)
{
  /* handle minimize */
  if (width == 0 || height == 0) {
    return;
  }

  mRenderData.rdWidth = width;
  mRenderData.rdHeight = height;

  mFramebuffer.resize(width, height);
  glViewport(0, 0, width, height);

  Logger::log(1, "%s: resized window to %dx%d\n", __FUNCTION__, width, height);
}

void OGLRenderer::uploadData(const OGLMesh& vertexData)
{
  mRenderData.rdTriangleCount = static_cast<int32_t>(vertexData.vertices.size() / 3);
  mVertexBuffer.uploadData(vertexData);
}

void OGLRenderer::handleKeyEvents(int key, int scancode, int action, int mods)
{
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
    toggleShader();
  }
}

void OGLRenderer::handleMouseButtonEvents(int button, int action, int mods)
{
  /* forward to ImGui */
  ImGuiIO& io = ImGui::GetIO();
  if (button >= 0 && button < ImGuiMouseButton_COUNT) {
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);
  }

  /* hide from application */
  if (io.WantCaptureMouse) {
    return;
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    mMouseLock = !mMouseLock;

    if (mMouseLock) {
      glfwSetInputMode(mRenderData.rdWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      /* enable raw mode if possible */
      if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(mRenderData.rdWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
      }
    }
    else {
      glfwSetInputMode(mRenderData.rdWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void OGLRenderer::handleMousePositionEvents(double xPos, double yPos)
{
  /* forward to ImGui */
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent(static_cast<float>(xPos), static_cast<float>(yPos));

  /* hide from application */
  if (io.WantCaptureMouse) {
    return;
  }

  /* calculate relative movement from last position */
  int mouseMoveRelX = static_cast<int>(xPos) - mMouseXPos;
  int mouseMoveRelY = static_cast<int>(yPos) - mMouseYPos;

  if (mMouseLock) {
    mRenderData.rdViewAzimuth += static_cast<float>(mouseMoveRelX / 10.0);
    /* keep between 0 and 360 degree */
    if (mRenderData.rdViewAzimuth < 0.0f) {
      mRenderData.rdViewAzimuth += 360.0f;
    }
    if (mRenderData.rdViewAzimuth >= 360.0f) {
      mRenderData.rdViewAzimuth -= 360.0f;
    }

    mRenderData.rdViewElevation -= static_cast<float>(mouseMoveRelY / 10.0);
    /* keep between -89 and +89 degree */
    if (mRenderData.rdViewElevation > 89.0f) {
      mRenderData.rdViewElevation = 89.0f;
    }
    if (mRenderData.rdViewElevation < -89.0f) {
      mRenderData.rdViewElevation = -89.0f;
    }
  }

  /* save old values*/
  mMouseXPos = static_cast<int>(xPos);
  mMouseYPos = static_cast<int>(yPos);
}

void OGLRenderer::toggleShader()
{
  mRenderData.rdUseChangedShader = !mRenderData.rdUseChangedShader;
}

void OGLRenderer::draw()
{
  /* handle minimize */
  while (mRenderData.rdWidth == 0 || mRenderData.rdHeight == 0) {
    glfwGetFramebufferSize(mRenderData.rdWindow, &mRenderData.rdWidth, &mRenderData.rdHeight);
    glfwWaitEvents();
  }

  /* get time difference for movement */
  double tickTime = glfwGetTime();
  mRenderData.rdTickDiff = static_cast<float>(tickTime - mLastTickTime);

  mRenderData.rdFrameTime = mFrameTimer.stop();
  mFrameTimer.start();

  handleMovementKeys();

  /* draw to framebuffer */
  mFramebuffer.bind();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  mMatrixGenerateTimer.start();
  mProjectionMatrix = glm::perspective(glm::radians(static_cast<float>(mRenderData.rdFieldOfView)), static_cast<float>(mRenderData.rdWidth) / static_cast<float>(mRenderData.rdHeight), 0.1f, 10.0f);

  float t = static_cast<float>(glfwGetTime());
  glm::mat4 model = glm::mat4(1.0f);

  if (!mRenderData.rdUseChangedShader) {
    mBasicShader.use();
    model = glm::rotate(glm::mat4(1.0f), -t, glm::vec3(0.0f, 0.0f, 1.0f));
  }
  else {
    mChangedShader.use();
    model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 0.0f, 1.0f));
  }
  mViewMatrix = mCamera.getViewMatrix(mRenderData) * model;

  mRenderData.rdMatrixGenerateTime = mMatrixGenerateTimer.stop();

  mUploadToUBOTimer.start();
  mUniformBuffer.uploadUboData(mViewMatrix, mProjectionMatrix);
  mRenderData.rdUploadToUBOTime = mUploadToUBOTimer.stop();

  mTex.bind();
  mVertexBuffer.bind();

  mVertexBuffer.draw(GL_TRIANGLES, 0, mRenderData.rdTriangleCount * 3);
  mVertexBuffer.unbind();
  mTex.unbind();

  mFramebuffer.unbind();

  /* blit color buffer to screen */
  mFramebuffer.drawToScreen();

  mUIGenerateTimer.start();
  mUserInterface.createFrame(mRenderData);
  mRenderData.rdUIGenerateTime = mUIGenerateTimer.stop();

  mUIDrawTimer.start();
  mUserInterface.render();
  mRenderData.rdUIDrawTime = mUIDrawTimer.stop();

  mLastTickTime = tickTime;
}

void OGLRenderer::cleanup()
{
  mUserInterface.cleanup();
  mBasicShader.cleanup();
  mChangedShader.cleanup();
  mTex.cleanup();
  mVertexBuffer.cleanup();
  mUniformBuffer.cleanup();
  mFramebuffer.cleanup();
}

void OGLRenderer::handleMovementKeys()
{
  mRenderData.rdMoveForward = 0;
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_W) == GLFW_PRESS) {
    mRenderData.rdMoveForward += 1;
  }
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_S) == GLFW_PRESS) {
    mRenderData.rdMoveForward -= 1;
  }

  mRenderData.rdMoveRight = 0;
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_A) == GLFW_PRESS) {
    mRenderData.rdMoveRight -= 1;
  }
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_D) == GLFW_PRESS) {
    mRenderData.rdMoveRight += 1;
  }

  mRenderData.rdMoveUp = 0;
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_E) == GLFW_PRESS) {
    mRenderData.rdMoveUp += 1;
  }
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_Q) == GLFW_PRESS) {
    mRenderData.rdMoveUp -= 1;
  }
}
