#include <cstdlib>
#include <iostream>
#include <fstream>
#include <climits>
#include <memory>

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

int run(Cache &cache, const vector<string> &input, vector<bool> &output) {
    int miss_cnt = 0;
    const int sz = input.size();
    for (int i = 1; i < sz - 1; ++i) {
        if (cache.Access(input[i])) {
            output.push_back(true);
        } else {
            output.push_back(false);
            miss_cnt += 1;
        }
    }
    return miss_cnt;
}

void display(
        const Cache &cache,
        const vector<string> &input,
        int miss_cnt,
        const vector<bool> &result,
        ostream &output) {
    const int sz = input.size();

    output << cache << "\n";
    output << input[0] << "\n";
    for (int i = 0; i < sz - 2; ++i) {
        output << input[i + 1];
        if (result[i]) {
            output << " hit\n";
        } else {
            output << " miss\n";
        }
    }
    output << ".end\n";
    output << "\nTotal cache miss count: " << miss_cnt;
}

void combination_dfs(
        const vector<int> &all, int idx, int sz,
        vector<int> &curr, vector<vector<int>> &ret) {
    if (!sz) {
        ret.push_back(curr);
        return;
    }

    for (;(int)all.size() - idx > sz; ++idx) {
        curr.push_back(all[idx]);
        combination_dfs(all, idx + 1, sz - 1, curr, ret);
        curr.pop_back();
    }

    for (;idx < (int)all.size(); ++idx) {
        curr.push_back(all[idx]);
    }
    ret.push_back(curr);
    curr.resize(curr.size() - sz);
}

vector<vector<int>> combination(const vector<int> &all, int sz) {
    vector<vector<int>> ret;
    vector<int> tmp;
    combination_dfs(all, 0, sz, tmp, ret);
    return ret;
}

int main(int argc, const char *argv[]){
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

    array<int, 2> itvl = config.CandidateInterval();
    int extra = config.idx_size() > 15 ? 1 :
                config.idx_size() > 10 ? 3 : 5;
    int bit_len = min(config.idx_size() + extra, itvl[1] - itvl[0]);
    PaperIndexing idx{ref, itvl, bit_len};

    const vector<int> &bit_with_extra = idx.all_idx_bit();
    vector<vector<int>> all_choice = combination(bit_with_extra, config.idx_size());

    LsbIndexing lsb{itvl, config.idx_size()};
    all_choice.push_back(lsb.all_idx_bit());

    int min_miss = INT_MAX;
    unique_ptr<Cache> min_cache;
    vector<bool> min_result;

    for (const vector<int> &choice: all_choice) {
        unique_ptr<Cache> cache = make_unique<Cache>(config, choice);
        vector<bool> res;
        int miss_cnt = run(*cache, ref, res);
        if (miss_cnt < min_miss) {
            min_miss = miss_cnt;
            min_result = res;
            min_cache = move(cache);
        }
    }

    assert(min_cache.get());
    display(*min_cache, ref, min_miss, min_result, output);
    return 0;
}