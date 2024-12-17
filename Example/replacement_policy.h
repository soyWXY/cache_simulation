#pragma once

#include <cassert>
#include <vector>

#include "cacheline.h"

using namespace std;

class ClockReplacement {
public:
    ClockReplacement(int way) : way_size_(way) {}

    int Replace(vector<Cacheline> &all_cacheline) {
        assert(way_size_ == static_cast<int>(all_cacheline.size()));
        while (all_cacheline[clock_ptr_].ref_bit) {
            all_cacheline[clock_ptr_].ref_bit = 0;
            clock_ptr_ = (clock_ptr_ + 1) % way_size_;
        }
        int ret = clock_ptr_;
        clock_ptr_ = (clock_ptr_ + 1) % way_size_;
        return ret;
    }

private:
    int way_size_;
    int clock_ptr_{0};
};