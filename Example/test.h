#pragma once

#include <fstream>
#include <sstream>
#include <iomanip>

#include "cache.h"

vector<string> ParseInput(istream &is);

void run_and_display(
        Cache &cache, const vector<string> &input, ostream &output);

bool lsb_test_2_2(bool display) {
    cout << "running " << __func__ << "\n";
    ostringstream actual;

    string cache_cfg_file = "../testcases-release1/config/cache2.org";
    ifstream cache_input{cache_cfg_file};
    if (!cache_input) {
        cerr << "fail to open cache configure file\n";
        exit(1);
    }

    string ref_file = "../testcases-release1/bench/reference2.lst";
    ifstream ref_input{ref_file};
    if (!ref_input) {
        cerr << "fail to open memory reference file\n";
        exit(1);
    }
    vector<string> ref = ParseInput(ref_input);

    CacheConfig config{cache_input};
    LsbIndexing idx{config.CandidateInterval(), config.idx_size()};
    Cache cache(config, idx.all_idx_bit());

    run_and_display(cache, ref, actual);

    string expect = R"(Address bits: 8
Block size: 4
Cache sets: 4
Associativity: 2


Offset bit count: 2
Indexing bit count: 2
Indexing bits: 3 2

.benchmark testcase1
00000000 miss
00010000 miss
00100000 miss
00000000 miss
00101100 miss
00000000 hit
00101100 hit
.end

Total cache miss count: 5)";

    bool result = actual.str() == expect;

    if (!result && display) {
        cout << __func__ << " fail\n"
             << "-------------- expect begin --------------\n"
             << quoted(expect) << "\n"
             << "-------------- expect end --------------\n"
             << "-------------- actual begin --------------\n"
             << quoted(actual.str()) << "\n"
             << "-------------- actual end --------------\n";
    }

    return result;
}

bool lsb_test_1_1(bool display) {
    cout << "running " << __func__ << "\n";
    ostringstream actual;

    string cache_cfg_file = "../testcases-release1/config/cache1.org";
    ifstream cache_input{cache_cfg_file};
    if (!cache_input) {
        cerr << "fail to open cache configure file\n";
        exit(1);
    }

    string ref_file = "../testcases-release1/bench/reference1.lst";
    ifstream ref_input{ref_file};
    if (!ref_input) {
        cerr << "fail to open memory reference file\n";
        exit(1);
    }
    vector<string> ref = ParseInput(ref_input);

    CacheConfig config{cache_input};
    LsbIndexing idx{config.CandidateInterval(), config.idx_size()};
    Cache cache(config, idx.all_idx_bit());

    run_and_display(cache, ref, actual);

    string expect = R"(Address bits: 8
Block size: 4
Cache sets: 8
Associativity: 1


Offset bit count: 2
Indexing bit count: 3
Indexing bits: 4 3 2

.benchmark testcase1
00101100 miss
00100000 miss
00101100 hit
00100000 hit
.end

Total cache miss count: 2)";

    bool result = actual.str() == expect;

    if (!result && display) {
        cout << __func__ << " fail\n"
             << "-------------- expect begin --------------\n"
             << quoted(expect) << "\n"
             << "-------------- expect end --------------\n"
             << "-------------- actual begin --------------\n"
             << quoted(actual.str()) << "\n"
             << "-------------- actual end --------------\n";
    }

    return result;
}

bool paper_test_2_1(bool display) {
    cout << "running " << __func__ << "\n";
    ostringstream actual;

    string cache_cfg_file = "../testcases-release1/config/cache2.org";
    ifstream cache_input{cache_cfg_file};
    if (!cache_input) {
        cerr << "fail to open cache configure file\n";
        exit(1);
    }

    string ref_file = "../testcases-release1/bench/reference1.lst";
    ifstream ref_input{ref_file};
    if (!ref_input) {
        cerr << "fail to open memory reference file\n";
        exit(1);
    }
    vector<string> ref = ParseInput(ref_input);

    CacheConfig config{cache_input};
    PaperIndexing idx{ref, config.CandidateInterval(), config.idx_size()};
    Cache cache(config, idx.all_idx_bit());

    run_and_display(cache, ref, actual);

    string expect = R"(Address bits: 8
Block size: 4
Cache sets: 4
Associativity: 2


Offset bit count: 2
Indexing bit count: 2
Indexing bits: 3 2

.benchmark testcase1
00101100 miss
00100000 miss
00101100 hit
00100000 hit
.end

Total cache miss count: 2)";

    bool result = actual.str() == expect;

    if (!result && display) {
        cout << __func__ << " fail\n"
             << "-------------- expect begin --------------\n"
             << quoted(expect) << "\n"
             << "-------------- expect end --------------\n"
             << "-------------- actual begin --------------\n"
             << quoted(actual.str()) << "\n"
             << "-------------- actual end --------------\n";
    }

    return result;
}

bool paper_test_1_2(bool display) {
    cout << "running " << __func__ << "\n";
    ostringstream actual;

    string cache_cfg_file = "../testcases-release1/config/cache1.org";
    ifstream cache_input{cache_cfg_file};
    if (!cache_input) {
        cerr << "fail to open cache configure file\n";
        exit(1);
    }

    string ref_file = "../testcases-release1/bench/reference2.lst";
    ifstream ref_input{ref_file};
    if (!ref_input) {
        cerr << "fail to open memory reference file\n";
        exit(1);
    }
    vector<string> ref = ParseInput(ref_input);

    CacheConfig config{cache_input};
    PaperIndexing idx{ref, config.CandidateInterval(), config.idx_size()};
    Cache cache(config, idx.all_idx_bit());

    run_and_display(cache, ref, actual);

    string expect = R"(Address bits: 8
Block size: 4
Cache sets: 8
Associativity: 1


Offset bit count: 2
Indexing bit count: 3
Indexing bits: 5 4 2

.benchmark testcase1
00000000 miss
00010000 miss
00100000 miss
00000000 hit
00101100 miss
00000000 hit
00101100 hit
.end

Total cache miss count: 4)";

    bool result = actual.str() == expect;

    if (!result && display) {
        cout << __func__ << " fail\n"
             << "-------------- expect begin --------------\n"
             << quoted(expect) << "\n"
             << "-------------- expect end --------------\n"
             << "-------------- actual begin --------------\n"
             << quoted(actual.str()) << "\n"
             << "-------------- actual end --------------\n";
    }

    return result;
}

bool run_test(bool display = true) {
    bool ret = lsb_test_1_1(display);
    ret &= lsb_test_2_2(display);
    ret &= paper_test_1_2(display);
    ret &= paper_test_2_1(display);
    return ret;
}