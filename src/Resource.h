#pragma once

#include <string>

namespace engine {
    class Resource{
    public:
        template<typename T>
        T load(std::string filepath);
    };
}