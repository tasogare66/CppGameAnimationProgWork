#include "OGLRenderer.h"
#include "Logger.h"
#include <glm/gtc/matrix_transform.hpp>

OGLRenderer::OGLRenderer(GLFWwindow* window)
  : mWindow(window)
{}

bool OGLRenderer::init(uint32_t width, uint32_t height)
{
  /* required for perspective */
  mWidth = width;
  mHeight = height;

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

  return true;
}

void OGLRenderer::setSize(uint32_t width, uint32_t height)
{
  /* handle minimize */
  if (width == 0 || height == 0) {
    return;
  }

  mFramebuffer.resize(width, height);
  glViewport(0, 0, width, height);

  Logger::log(1, "%s: resized window to %dx%d\n", __FUNCTION__, width, height);
}

void OGLRenderer::uploadData(const OGLMesh& vertexData)
{
  mTriangleCount = static_cast<int32_t>(vertexData.vertices.size() / 3);
  mVertexBuffer.uploadData(vertexData);
}

void OGLRenderer::handleKeyEvents(int key, int scancode, int action, int mods)
{
  if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
    mUseChangedShader = !mUseChangedShader;
  }
}

void OGLRenderer::draw()
{
  /* handle minimize */
  while (mWidth == 0 || mHeight == 0) {
    glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
    glfwWaitEvents();
  }

  /* draw to framebuffer */
  mFramebuffer.bind();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glm::vec3 cameraPosition = glm::vec3(0.4f, 0.3f, 1.0f);
  glm::vec3 cameraLookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
  mProjectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 100.f);
  float t = static_cast<float>(glfwGetTime());
  glm::mat4 model = glm::mat4(1.0f);

  if (mUseChangedShader) {
    mChangedShader.use();
    model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 0.0f, 1.0f));
  }
  else {
    mBasicShader.use();
    model = glm::rotate(glm::mat4(1.0f), -t, glm::vec3(0.0f, 0.0f, 1.0f));
  }
  mViewMatrix = glm::lookAt(cameraPosition, cameraLookAtPosition, cameraUpVector) * model;
  mUniformBuffer.uploadUboData(mViewMatrix, mProjectionMatrix);
  mTex.bind();
  mVertexBuffer.bind();

  mVertexBuffer.draw(GL_TRIANGLES, 0, mTriangleCount * 3);
  mVertexBuffer.unbind();
  mTex.unbind();

  mFramebuffer.unbind();

  /* blit color buffer to screen */
  mFramebuffer.drawToScreen();
}

void OGLRenderer::cleanup()
{
  mBasicShader.cleanup();
  mChangedShader.cleanup();
  mTex.cleanup();
  mVertexBuffer.cleanup();
  mUniformBuffer.cleanup();
  mFramebuffer.cleanup();
}
