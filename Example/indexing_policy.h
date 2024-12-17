#pragma once

#include <cassert>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>

using namespace std;

static inline int ExtractIndex(
        const string &addr, const vector<int> &all_idx_bit) {
    int ret = 0;
    for (int i: all_idx_bit) {
        ret = ret * 2 + (addr[addr.size() - i - 1] - '0');
    }
    return ret;
}

class LsbIndexing {
public:
    LsbIndexing(const array<int, 2> &interval, int idx_size)
          : all_idx_bit_(idx_size) {
        iota(all_idx_bit_.rbegin(), all_idx_bit_.rend(), interval[0]);
    }

    const vector<int> &all_idx_bit() const {
        return all_idx_bit_;
    }

private:
    vector<int> all_idx_bit_;
};

class PaperIndexing {
public:
    PaperIndexing(const vector<string> &input,
                  const array<int, 2> &interval,
                  int idx_size) {
        assert(input.size() > 2UL);
        vector<double> quality = GetQuality(input);
        vector<vector<double>> correlation = GetCorrelation(input);
        for (int i = 0; i < idx_size; ++i) {
            auto it = max_element(quality.begin() + interval[0], quality.end());
            int idx = it - quality.begin();
            all_idx_bit_.push_back(idx);
            for (int k = interval[0]; k < interval[1]; ++k) {
                quality[k] = min(quality[k], quality[k] * correlation[idx][k]);
            }
            quality[idx] = -1;
        }
        sort(all_idx_bit_.rbegin(), all_idx_bit_.rend());
    }

    const vector<int> &all_idx_bit() const {
        return all_idx_bit_;
    }

private:
    vector<double> GetQuality(const vector<string> &input) const {
        int addr_size = input[1].size();
        vector<double> one_cnt(addr_size, 0);
        const int sz = input.size();
        for (int i = 1; i < sz - 1; ++i) {
            for (int j = 0; j < addr_size; ++j) {
                one_cnt[j] += input[i][addr_size - 1 - j] - '0';
            }
        }
        for (double &one: one_cnt) {
            double zero = sz - 2 - one;
            one = min(one, zero) / max(one, zero);
        }
        return one_cnt;
    }

    vector<vector<double>> GetCorrelation(const vector<string> &input) const {
        int addr_size = input[1].size();
        const int sz = input.size();
        vector<vector<double>> ret(addr_size);
        for (auto &row: ret)
            row.resize(addr_size);

        for (int i = 0; i < addr_size; ++i) {
            for (int j = 0; j < addr_size; ++j) {
                if (i == j) {
                    ret[i][i] = 0;
                    continue;
                }
                double eq = 0;
                for (int k = 1; k < sz - 1; ++k) {
                    eq += (input[k][addr_size - 1 - i]
                            == input[k][addr_size - 1 - j]);
                }
                double neq = sz - 2 - eq;
                ret[i][j] = min(eq, neq) / max(eq, neq);
            }
        }
        return ret;
    }

    vector<int> all_idx_bit_;
};