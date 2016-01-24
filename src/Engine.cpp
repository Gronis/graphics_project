#include "Engine.h"

namespace engine {
    
    void Engine::run(){
        running_ = true;
        //the time between updates in milliseconds
        float dt = 0;
        
        long long timeBefore = 0;
        long long timeAfter = 0;
        //The amount of time the system has extended during update.
        //Should be 0 unless the system can't update as fast as it should
        long long extendedTime = 0;
        
        while (running_) {
            timeBefore = timestamp();
            bool all_windows_closed = false;
            for (auto window : manager<gfx::Window>()) {
                window->update(dt);
                window->render(dt, manager<gfx::Mesh>(), manager<gfx::Renderer>());
                all_windows_closed |= window->closed();
            }
            running_ = !all_windows_closed;
            timeAfter = timestamp();
            waitForSleepyUpdate(timeBefore, timeAfter, &extendedTime, &dt);
        }
    }
    
    void Engine::start(){
        if(running_){ throw exception(); }
        running_ = true;
        update_thread_ = thread(&Engine::run, this);
    }
    
    void Engine::stop(){
        running_ = false;
        //wait for update thread if isn't already dead or if this is the update thread
        if(update_thread_.joinable() && update_thread_.get_id() != this_thread::get_id()){
            update_thread_.join();
        }
    }
    
    void Engine::join(){
        while(running_){
            if(update_thread_.joinable()){
                update_thread_.join();
            }else{
                this_thread::sleep_for (chrono::milliseconds(100));
            }
        }
    }
    
    void Engine::waitForSleepyUpdate(long long timeBefore, long long timeAfter, long long* extendedTime, float* dt){
        //how long time the last update took
        long long updateTimeNano;
        //how long time it should take
        long long disiredUpdateTimeNano = 1000000000 / updates_per_second_when_sleeping_;
        updateTimeNano = timeAfter - timeBefore;
        //wait if didn't take enough time
        if (sleeping_ && disiredUpdateTimeNano > updateTimeNano + *extendedTime) {
            this_thread::sleep_for(chrono::nanoseconds(disiredUpdateTimeNano - updateTimeNano));
            (*dt) = (float)(disiredUpdateTimeNano) / 1000000000;
            (*extendedTime) = 0;
        }else{
            (*dt) = (float)(updateTimeNano) / 1000000000;
            (*extendedTime) += updateTimeNano - disiredUpdateTimeNano;
        }
    }
    
    
    
}
