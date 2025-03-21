
#pragma once
#include "OGLRenderData.h"

class Model {
public:
  void init();

  const OGLMesh& getVertexData() { return mVertexData; }

private:
  OGLMesh mVertexData;
};
