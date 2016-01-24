#pragma once

#include <cstddef>

/**
 *  Class is used internally to keep track if different classes.
 */
namespace engine {
    namespace util{
        class BaseClass {
        public:
            typedef std::size_t Family;
            virtual ~BaseClass();
            
        protected:
            static Family _family_counter;
        };
        
        template <typename Derived>
        class Class : public BaseClass {
        public:
            static Family family() {
                static Family family = _family_counter++;
                return family;
            }
        };
    }
}