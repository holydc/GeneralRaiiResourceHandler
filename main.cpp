#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>

#include <iostream>
#include <utility>

#include "Handle.h"

int main() {
  raii::Handle a{fopen, fclose, "Handle.h", "r"};
  if (!a) {
    std::cout << "fopen failed\n";
    return 0;
  }

  decltype(a) b;
  b = std::move(a);
  if (a) {
    std::cout << "move a failed\n";
    return 0;
  }
  if (!b) {
    std::cout << "move b failed\n";
    return 0;
  }
  fseek(b, 0, SEEK_END);
  std::cout << "file size: " << ftell(b) << '\n';
  return 0;
}
