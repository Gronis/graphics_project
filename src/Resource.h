#pragma once

#include <string>
#include <bitset>

namespace engine {
class Resource {
 public:
  template<typename T>
  T load(std::string filepath);
};
}