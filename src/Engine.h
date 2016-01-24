#pragma once

#include <stdio.h>
#include <chrono>
#include <thread>

#include "util/Manager.h"
#include "gfx/Window.h"
#include "gfx/Mesh.h"

using namespace std;

namespace engine {
    
    class Engine{
    private:
        bool running_ = false;
        bool sleeping_ = true;
        int updates_per_second_when_sleeping_ = 24;
        thread update_thread_;
        util::ManagerHandler managers_;
        
    public:
        explicit Engine(){}
        
        ~Engine(){
            stop();
        }
        
        /// Run the Engine on this thread
        void run();
        
        /// Start the Engine on a new thread
        void start();
        
        /// Stops this engine
        void stop();
        
        /// Wait for this Engine to stop
        void join();
        
        /// Get if this engine is running or not
        bool running(){ return running_; }
        
        /// Create and define what kind of manager should be used for a specific type
        template<typename T, typename ...Args>
        inline util::Manager<T>& create_manager(Args&&... args){
            return managers_.create_manager<T>(std::forward<Args>(args) ...);
        }
        
        /// Get managers of a specific type
        template<typename T>
        inline util::Manager<T>& manager(){ return managers_.get<T>(); }
        
        /// Create an object of a specific type and put it in the manager
        template<typename T, typename ...Args>
        inline util::Handle<T> create(Args&&... args){ return managers_.create<T>(std::forward<Args>(args) ...); }
        
        /// Destroy an object and release it from the manager
        template<typename T>
        inline void destroy(util::Handle<T> handle){ managers_.destroy(handle);}

        
    private:
        /* Wait for next update. Doesn't wait if not sleeping. If update time exceeded, this is returned in extended time.
         * dt is the time in milliseconds the last frame took
         */
        void waitForSleepyUpdate(long long timeBefore, long long timeAfter, long long *extendedTime, float* dt);
        
        inline long long timestamp(){
            return chrono::duration_cast<chrono::nanoseconds>(
                  chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
        
    };
    
}

