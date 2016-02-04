#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <thirdparty/tiny_obj_loader.h>

#include "Resource.h"
#include "gfx/Shader.h"
#include "gfx/Model.h"
#include "gfx/Bitmap.h"


namespace engine {

using namespace gfx;

static const unsigned char COLOR[] = {0xff, 0xff, 0xff};
static Bitmap NO_TEXTURE = Bitmap(1,1,Bitmap::Format::Format_RGB, COLOR);

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

std::string fix_slash(std::string filepath){
#ifdef WINDOWS
  char old_symbol = '/';
  char new_symbol = '\\';
#else
  char old_symbol = '\\';
  char new_symbol = '/';
#endif
  for(char& c : filepath){
    if(c == old_symbol) c = new_symbol;
  }
  return filepath;
}

template<>
std::string Resource::load<std::string>(std::string filepath) {
  std::ifstream f;
  filepath = find_filepath(fix_slash(filepath), 1);
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
  filepath = find_filepath(fix_slash(filepath), 1);
  unsigned char *pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
  if (!pixels) throw std::runtime_error(stbi_failure_reason());
  gfx::Bitmap bmp(width, height, (gfx::Bitmap::Format) channels, pixels);
  stbi_image_free(pixels);
  return bmp;
}

template<>
gfx::Shader Resource::load<gfx::Shader>(std::string filepath) {
  filepath = find_filepath(fix_slash(filepath), 1);
  std::string file_format = fileformat(filepath);
  if(file_format == "vert"){
    return gfx::Shader(load<std::string>(filepath),GL_VERTEX_SHADER);
  }else if(file_format == "frag"){
    return gfx::Shader(load<std::string>(filepath),GL_FRAGMENT_SHADER);
  }else throw std::runtime_error("unrecognised shader format");
}

template<>
gfx::Model Resource::load<gfx::Model>(std::string filepath) {
  filepath = find_filepath(fix_slash(filepath), 1);
  std::string file_format = fileformat(filepath);
  if(file_format == "obj"){
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    tinyobj::LoadObj(shapes, materials, err, filepath.c_str(), folder(filepath).c_str());
    if (!err.empty()) {
      throw std::runtime_error(err);
    }
    std::vector<gfx::Model::Material> materials_ref;
    for (auto && material : materials) {
      gfx::Model::Material mat;
      mat.ambient_ = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
      mat.diffuse_ = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
      mat.specular_ = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
      if(material.ambient_texname != "") {
        mat.ambient_tex = std::make_shared<Texture>(load<Bitmap>(folder(filepath) + material.ambient_texname));
      } else mat.ambient_tex = std::make_shared<Texture>(Bitmap(NO_TEXTURE));
      if(material.diffuse_texname != "") {
        mat.diffuse_tex = std::make_shared<Texture>(load<Bitmap>(folder(filepath) + material.diffuse_texname));
      } else mat.diffuse_tex = std::make_shared<Texture>(Bitmap(NO_TEXTURE));
      if(material.specular_texname != "") {
        mat.specular_tex = std::make_shared<Texture>(load<Bitmap>(folder(filepath) + material.specular_texname));
      } else mat.specular_tex = std::make_shared<Texture>(Bitmap(NO_TEXTURE));
      materials_ref.push_back(mat);
    }
    return gfx::Model(shapes, materials, materials_ref);
  }else throw std::runtime_error("unrecognised model format");
}

}