#include <cstdlib>
#include <iostream>
#include <fstream>

#ifndef NDEBUG
  #include "test.h"
#endif

#include "cache.h"

using namespace std;

vector<string> ParseInput(istream &is) {
    vector<string> ret;

    char title[30];
    is.getline(title, sizeof(title));
    ret.emplace_back(title);

    string tmp;
    while (is >> tmp)
        ret.emplace_back(tmp);
    return ret;
}

void run_and_display(
        Cache &cache, const vector<string> &input, ostream &output) {
    int miss_cnt = 0;

    output << cache << "\n";
    output << input[0] << "\n";
    const int sz = input.size();
    for (int i = 1; i < sz - 1; ++i) {
        output << input[i];
        if (cache.Access(input[i])) {
            output << " hit\n";
        } else {
            output << " miss\n";
            miss_cnt += 1;
        }
    }
    output << ".end\n";
    output << "\nTotal cache miss count: " << miss_cnt;
}

int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifndef NDEBUG
    return !run_test();
#endif

    if (argc != 4) {
        cerr << "expected 4 arguments, but got " << argc << "\n";
        exit(1);
    }

    ofstream output{argv[3]};
    if (!output) {
        cerr << "fail to open output file\n";
        exit(1);
    }

    ifstream cache_input{argv[1]};
    if (!cache_input) {
        cerr << "fail to open cache configure file\n";
        exit(1);
    }

    ifstream ref_input{argv[2]};
    if (!ref_input) {
        cerr << "fail to open memory reference file\n";
        exit(1);
    }

    vector<string> ref = ParseInput(ref_input);

    CacheConfig config{cache_input};
    LsbIndexing idx{config.CandidateInterval(), config.idx_size()};
    // PaperIndexing idx{ref, config.CandidateInterval(), config.idx_size()};
    Cache cache(config, idx.all_idx_bit());

    run_and_display(cache, ref, output);
    return 0;
}