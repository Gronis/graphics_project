#pragma once

#include <vector>
#include <cassert>

namespace engine {
    namespace util{
        class BasePool{
        public:
            explicit BasePool(size_t element_size, size_t chunk_size = 8192) :
                size_(0),
                capacity_(0),
                element_size_(element_size),
                chunk_size_(chunk_size){
            }
            
            virtual ~BasePool() {
                for (char *ptr : chunks_) {
                    delete[] ptr;
                }
            }
            
            inline size_t size() const { return size_; }
            inline size_t capacity() const { return capacity_; }
            inline size_t chunks() const { return chunks_.size(); }
            
            inline void ensure_min_size(std::size_t size) {
                if (size >= size_) {
                    if (size >= capacity_) ensure_min_capacity(size);
                    size_ = size;
                }
            }
            
            inline void ensure_min_capacity(size_t min_capacity){
                while (min_capacity >= capacity_) {
                    char *chunk = new char[element_size_ * chunk_size_];
                    chunks_.push_back(chunk);
                    capacity_ += chunk_size_;
                }
            }
            
            inline void* get(size_t index){
                //assert(index < size_);
                return chunks_[index / chunk_size_] + (index % chunk_size_) * element_size_;
            }
            
            const inline void* get(size_t index) const{
                assert(index < size_);
                return chunks_[index / chunk_size_] + (index % chunk_size_) * element_size_;
            }
            
            inline virtual void destroy(size_t index) = 0;
            
        protected:
            size_t size_;
            size_t capacity_;
            size_t element_size_;
            size_t chunk_size_;
            std::vector<char *> chunks_;
            
        };
        
        template <typename T>
        class Pool : public BasePool {
        public:
            Pool(size_t chunk_size) : BasePool(sizeof(T), chunk_size) {}
            virtual ~Pool() {
                // Component destructors *must* be called by owner.
            }
            
            virtual void destroy(std::size_t index) override {
                assert(index < size_);
                T *ptr = static_cast<T*>(get(index));
                ptr->~T();
            }
        };
    }
}