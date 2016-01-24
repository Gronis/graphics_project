#pragma once

#include <unordered_map>
#include <new>

#include "util/Pool.h"
#include "Class.h"

namespace engine {
    namespace util{
        template<typename T> class Manager;
        
        template<typename T>
        class Handle{
        public:
            Handle(Manager<T> &pool, size_t index) : pool_(pool), index_(index) {}
            
            Handle(const Handle& other) : pool_(other.pool_), index_(other.index_) {}
            
            Handle& operator = (const Handle& other){
                pool_ = other.pool_;
                index_ = other.index_;
                return *this;
            }
            
            T* operator-> (){
                return static_cast<T*>(pool_.get(index_));
            }
            
            T& get() {
                return *static_cast<T*>(pool_.get(index_));
            }
            
            void destroy(){
                pool_.destroy(*this);
            }
            
            friend Manager<T>;
        private:
            Manager<T> &pool_;
            size_t index_;
        };
        
        class BaseManager{
            
        };
        
        template<typename T>
        class Manager : public BaseManager{
        public:
            
            class Iterator : std::iterator<std::input_iterator_tag, T>{
            public:
                Iterator(Manager<T>* manager, size_t cursor = 0) : manager_(manager), cursor_(cursor){
                    std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
                    find_next();
                }
                
                Iterator(const Iterator& it) : manager_(it.manager_), free_cursor_(it.free_cursor_), cursor_(it.cursor_) {}
                
                Iterator& operator ++(){
                    cursor_++;
                    find_next();
                    return *this;
                }
                
                Iterator operator++(int) {
                    Iterator it(*this);
                    operator++();
                    return it;
                }
                
                bool operator==(const Iterator& rhs) {
                    return cursor_ == rhs.cursor_ && manager_ == rhs.manager_;
                }
                
                bool operator!=(const Iterator& rhs) {
                    return !(*this == rhs);
                }
                
                Handle<T> operator*() {
                    return (*manager_)[cursor_];
                }
                
            private:
                
                inline void find_next(){
                    //find next, non empty slot
                    while (manager_->free_list_.size() > free_cursor_ && manager_->free_list_[free_cursor_] == cursor_){
                        free_cursor_++;
                        cursor_++;
                    }
                }
                
                Manager<T> *manager_;
                size_t free_cursor_;
                size_t cursor_;
            };
            
            Manager(size_t chunk_size = 8192) : pool_(chunk_size){}
            
            ///Allocate memory for 1 element. Returns index in Pool
            template<typename ...Args>
            Handle<T> create(Args && ... args){
                pool_.ensure_min_capacity(size() + 1);
                size_t index;
                //see if there are holes in memory, allocate new element at first hole
                if (free_list_.empty()) {
                    index = size();
                    pool_.ensure_min_size(index + 1);
                } else{
                    index = free_list_.back();
                    free_list_.pop_back();
                }
                new(get(index)) T(args ...);
                return this->operator[](index);
            }
            
            inline void destroy(Handle<T> handle){
                std::sort(free_list_.begin(), free_list_.end());
                //cannot destroy already destroyed object
                assert(!binary_search(free_list_.begin(), free_list_.end(), handle.index_));
                pool_.destroy(handle.index_);
                free_list_.push_back(handle.index_);
            }
            
            inline Iterator begin(){
                return Iterator(this);
            }
            
            inline Iterator end(){
                return Iterator(this, pool_.size());
            }
            
            inline Handle<T> operator[] (size_t index){
                return Handle<T>(*this,index);
            }
            
            inline size_t size() const { return pool_.size() - free_list_.size(); }
            inline size_t capacity() const { return pool_.capacity(); }
            inline size_t chunks() const { return pool_.chunks(); }
            
            friend class Handle<T>;
            friend class Iterator;
        private:
            inline T* get (size_t index){
                return static_cast<T*>(pool_.get(index));
            }
            
            util::Pool<T> pool_;
            std::vector<size_t> free_list_;
        };
        
        class ManagerHandler{
        public:
            template<typename T, typename ...Args>
            Manager<T>& create_manager(Args && ... args){
                std::shared_ptr<Manager<T>> ptr(new Manager<T>(std::forward<Args>(args) ...));
                create_manager<T>(ptr);
                return *ptr;
            }
            
            template<typename T>
            void create_manager(std::shared_ptr<Manager<T>> ptr){
                auto it = managers_.find(Class<T>::family());
                //cannot create if already created. Have you already inserted an element of this type?
                assert(it == managers_.end());
                managers_.insert(std::make_pair(Class<T>::family(), std::static_pointer_cast<BaseManager>(ptr)));
            }
            
            template<typename T>
            std::shared_ptr<Manager<T>> get_ptr(){
                auto it = managers_.find(Class<T>::family());
                if(it == managers_.end()){
                    create_manager<T>();
                }
                return std::static_pointer_cast<Manager<T>>(managers_[Class<T>::family()]);
            }
            
            template<typename T>
            Manager<T>& get(){
                std::shared_ptr<Manager<T>> ptr = get_ptr<T>();
                return *ptr;
            }
            
            template<typename T, typename ...Args>
            Handle<T> create(Args && ... args){
                return get<T>().create(std::forward<Args>(args) ...);
            }
            
            template<typename T>
            inline void destroy(Handle<T> handle){
                Manager<T>& manager = get<T>();
                manager.template destroy(handle);
            }
            
        private:
            std::unordered_map<BaseClass::Family, std::shared_ptr<BaseManager>> managers_;
        };
    }
}