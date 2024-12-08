// CppGameAnimationProgWork.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "Main.h"
#include "Logger.h"

using namespace std;

int main()
{
  Logger::log(1, "%s error: Window init error\n", __FUNCTION__);
  cout << "Hello CMake." << endl;
  return 0;
}
