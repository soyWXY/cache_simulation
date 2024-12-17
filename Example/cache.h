#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>

#include "indexing_policy.h"
#include "cacheline.h"
#include "replacement_policy.h"

using namespace std;

class Cache;

class CacheConfig {
public:
    friend class Cache;

    CacheConfig(istream &is) {
        string unused;
        is >> unused >> addr_size_;

        is >> unused >> block_size_;
        if (block_size_ <= 0) {
            throw invalid_argument("Block_size must be positive");
        }
        offset_size_ = log2(block_size_);

        is >> unused >> set_size_;
        if (set_size_ <= 0) {
            throw invalid_argument("Cache_sets must be positive");
        }
        idx_size_ = log2(set_size_);

        is >> unused >> way_size_;
        if (way_size_ <= 0) {
            throw invalid_argument("Associativity must be positive");
        }
    }

    array<int, 2> CandidateInterval() const {
        return {offset_size_, addr_size_};
    }

    int idx_size() const {
        return idx_size_;
    }

private:
    int addr_size_;
    int block_size_;
    int offset_size_;
    int set_size_;
    int idx_size_;
    int way_size_;
};

class CacheSet {
public:
    CacheSet(int way) : way_size_(way), all_cacheline_(way), policy_(way) {} 

    bool Access(const string &tag_and_idx) {
        assert(way_size_ == static_cast<int>(all_cacheline_.size()));
        auto it = map_.find(tag_and_idx);
        if (it != map_.end()) {
            all_cacheline_[it->second].ref_bit = 1;
            return true;
        }

        int insert_pos;
        if (size_ == way_size_) {
            insert_pos = policy_.Replace(all_cacheline_);
            map_.erase(all_cacheline_[insert_pos].tag_and_idx);
        } else {
            insert_pos = size_;
            size_ += 1;
        }
        map_[tag_and_idx] = insert_pos;
        all_cacheline_[insert_pos].Store(tag_and_idx);
        return false;
    }

private:
    int way_size_;
    int size_ {0};
    vector<Cacheline> all_cacheline_;
    unordered_map<string, int> map_;
    ClockReplacement policy_;
};

class Cache {
public:
    Cache(const CacheConfig &config, const vector<int> &all_idx_bit)
          : config_(config), all_idx_bit_(all_idx_bit) {
        for (int i = 0; i < config_.set_size_; ++i) {
            all_cache_.emplace_back(config_.way_size_);
        }
    }

    bool Access(const string &addr) {
        int idx = ExtractIndex(addr, all_idx_bit_);
        return all_cache_[idx].Access(
            addr.substr(0, config_.addr_size_ - config_.offset_size_));
    }

    friend ostream &operator<<(ostream &os, const Cache &cache) {
        os << "Address bits: " << cache.config_.addr_size_
           << "\nBlock size: " << cache.config_.block_size_
           << "\nCache sets: " << cache.config_.set_size_
           << "\nAssociativity: " << cache.config_.way_size_ << "\n\n"
           << "\nOffset bit count: " << cache.config_.offset_size_
           << "\nIndexing bit count: " << cache.config_.idx_size_
           << "\nIndexing bits:";
        for (int i: cache.all_idx_bit_) {
            os << " " << i;
        }
        return os << "\n";
    }

private:
    CacheConfig config_;
    vector<int> all_idx_bit_;
    vector<CacheSet> all_cache_;
};
