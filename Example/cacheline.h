#pragma once

#include <string>

using namespace std;

struct Cacheline {
    void Store(const string &val) {
        ref_bit = 1;
        tag_and_idx = val;
    }

    int ref_bit;
    string tag_and_idx;
};