#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <ratio>
#include <iostream>

using namespace std::chrono;


class timer_c{
    public :
    timer_c(size_t update_time_interval):update_time(update_time_interval){
        last_update = clock_v.now();
    }

    bool is_ready(){
        //double to count in seconds
        duration<float> time_since_update = clock_v.now() - last_update;
        auto temp = time_since_update.count();
        (void) temp;
        return time_since_update.count() > update_time;
    }

    void update_timeframe(size_t update_time_interval){  
        update_time = update_time_interval;
    }

    void print_diff(){
        duration<float> time_since_update = clock_v.now() - last_update;
        auto temp = time_since_update.count();
        std::cout << temp << "\n";
    }
    void reset_update(){
        last_update = clock_v.now();
    }

    private:
    size_t update_time;
    steady_clock clock_v;
    time_point<steady_clock> last_update;
};