/* Dear ImGui*/
#pragma once
#include "OGLRenderData.h"

class UserInterface {
public:
  void init(const OGLRenderData& renderData);
  void createFrame(OGLRenderData& renderData);
  void render();
  void cleanup();
private:
  double mFramesPerSecond = 0.0;
  /* averaging speed */
  double mAveragingAlpha = 0.96;
};
