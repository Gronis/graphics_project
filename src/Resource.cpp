#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdparty/tiny_obj_loader.h"

#include "Resource.h"
#include "gfx/Shader.h"
#include "gfx/Model.h"
#include "gfx/Bitmap.h"


namespace engine {

std::string find_filepath(std::string filepath, int tries) {
  std::ifstream f;
  f.open(filepath.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    //sometimes the binary is not in root, check above level
    //TODO: ugly solution, fix later
    if (tries > 0) {
      return find_filepath("../" + filepath, ++tries);
    } else {
      throw std::runtime_error(std::string("Failed to open file: ") + filepath);
    }
  }
  f.close();
  return filepath;
}

std::string fileformat(std::string filepath) {
  auto pos = filepath.find_last_of('.');
  return filepath.substr(pos + 1, filepath.length());
}

std::string folder(std::string filepath){
#ifdef WINDOWS
  char symbol = '\\';
#else
  char symbol = '/';
#endif
  auto pos = filepath.find_last_of(symbol);
  return filepath.substr(0, pos + 1);
}

template<>
std::string Resource::load<std::string>(std::string filepath) {
  std::ifstream f;
  filepath = find_filepath(filepath, 1);
  f.open(filepath.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    throw std::runtime_error(std::string("Failed to open file: ") + filepath);
  }
  //read whole file into stringstream buffer
  std::stringstream buffer;
  buffer << f.rdbuf();
  return buffer.str();
}

template<>
gfx::Bitmap Resource::load<gfx::Bitmap>(std::string filepath) {
  int width, height, channels;
  filepath = find_filepath(filepath, 1);
  unsigned char *pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
  if (!pixels) throw std::runtime_error(stbi_failure_reason());
  gfx::Bitmap bmp(width, height, (gfx::Bitmap::Format) channels, pixels);
  stbi_image_free(pixels);
  return bmp;
}

template<>
gfx::Shader Resource::load<gfx::Shader>(std::string filepath) {
  filepath = find_filepath(filepath, 1);
  std::string file_format = fileformat(filepath);
  if(file_format == "vert"){
    return gfx::Shader(load<std::string>(filepath),GL_VERTEX_SHADER);
  }else if(file_format == "frag"){
    return gfx::Shader(load<std::string>(filepath),GL_FRAGMENT_SHADER);
  }else throw std::runtime_error("unrecognised shader format");
}

template<>
gfx::Model Resource::load<gfx::Model>(std::string filepath) {
  filepath = find_filepath(filepath, 1);
  std::string file_format = fileformat(filepath);
  if(file_format == "obj"){
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    tinyobj::LoadObj(shapes, materials, err, filepath.c_str(), folder(filepath).c_str());
    if (!err.empty()) {
      throw std::runtime_error(err);
    }
    return gfx::Model(shapes, materials);
  }else throw std::runtime_error("unrecognised model format");
}

}