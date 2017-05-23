#ifndef TIME_PROFILER_HH
#define TIME_PROFILER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include <string>
#include <ctime>
#include <utility>
#include <stack>
#include <list>
#include <iostream>

class TimeProfiler {
    private:
        typedef std::pair<std::string, std::time_t> timer_t;
        std::stack<timer_t> timers_launched_;
        std::list<timer_t> timers_finished_;

    public:
        void start_new_timer(const std::string& timer_name);
        void stop_last_timer();

        friend std::ostream& operator<<(std::ostream& os, const TimeProfiler& tp);
};

#endif // TIME_PROFILER_HH
