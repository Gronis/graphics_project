#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

#include "Resource.h"
#include "gfx/Bitmap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

namespace engine{
    
    std::string find_filepath(std::string filepath, int tries){
        std::ifstream f;
        f.open(filepath.c_str(), std::ios::in | std::ios::binary);
        if(!f.is_open()){
            //sometimes the binary is not in root, check above level
            //TODO: ugly solution, fix later
            if(tries > 0){
                return find_filepath("../" + filepath, ++tries);
            } else{
                throw std::runtime_error(std::string("Failed to open file: ") + filepath);
            }
        }
        f.close();
        return filepath;
    }
    
    template<> std::string Resource::load<std::string>(std::string filepath){
        std::ifstream f;
        filepath = find_filepath(filepath, 1);
        f.open(filepath.c_str(), std::ios::in | std::ios::binary);
        if(!f.is_open()){
            throw std::runtime_error(std::string("Failed to open file: ") + filepath);
        }
        //read whole file into stringstream buffer
        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }
    
    template<> gfx::Bitmap Resource::load<gfx::Bitmap>(std::string filepath) {
        int width, height, channels;
        filepath = find_filepath(filepath, 1);
        unsigned char* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        if(!pixels) throw std::runtime_error(stbi_failure_reason());
        gfx::Bitmap bmp(width, height, (gfx::Bitmap::Format)channels, pixels);
        stbi_image_free(pixels);
        return bmp;
    }
    
}