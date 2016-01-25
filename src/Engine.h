#pragma once

#include <stdio.h>
#include <chrono>
#include <thread>

using namespace std;

namespace engine {
    class Engine{
      using UpdateFunction = std::function<bool(float)>;
    private:
        bool running_ = false;
        bool sleeping_ = false;
        int updates_per_second_when_sleeping_ = 24;
        thread update_thread_;
        UpdateFunction update_function_;
        
    public:
        explicit Engine(UpdateFunction update_function) : update_function_(update_function){}
        
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

