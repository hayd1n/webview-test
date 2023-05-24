#if defined __APPLE__
#include <mach-o/dyld.h>
#elif defined _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <filesystem>
#include <iostream>
#include <string>

#include "webview.h"

namespace fs = std::filesystem;
using namespace std;

std::filesystem::path getExeDirectory() {
#if defined __APPLE__
  // MacOS specific
  char szPath[PATH_MAX];
  uint32_t size = sizeof(szPath);
  _NSGetExecutablePath(szPath, &size);
#elif defined _WIN32
  // Windows specific
  wchar_t szPath[MAX_PATH];
  GetModuleFileNameW(NULL, szPath, MAX_PATH);
#else
  // Linux specific
  char szPath[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", szPath, PATH_MAX);
  if (count < 0 || count >= PATH_MAX)
    return {};  // some error
  szPath[count] = '\0';
#endif
  // to finish the folder path with (back)slash
  return std::filesystem::path{szPath}.parent_path() / "";
}

std::string myBoundCallback(string args) {
  return "\"Return this string to the JS function 'myBoundCallback'\"";
}

int main() {
  webview::webview w(true, nullptr);

  w.set_title("Basic Example");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);

  w.bind("myBoundCallback", myBoundCallback);

  string path = "file:///";
  path += getExeDirectory();
#if defined __APPLE__
  path += "../Resources/assets/app.html";
#else
  path += "assets/app.html";
#endif
  cout << path << endl;
  w.navigate(path);

  w.run();

  return 0;
}
