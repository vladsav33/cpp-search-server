#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) {
        begin_ = begin;
        end_ = end;
    }

    Iterator begin() {
        return begin_;
    }

    Iterator end() {
        return end_;
    }
private:
    Iterator begin_;
    Iterator end_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& o, IteratorRange<Iterator> page) {
    for(auto it = page.begin(); it < page.end(); it++) {
        o << *it;
    }
    return o;
}

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, int size) {
        while (begin + size < end) {
            paginator_.push_back(IteratorRange(begin, begin + size));
            begin += size;
        }
        if (begin < end) {
            paginator_.push_back(IteratorRange(begin, end));
        }
    }

    auto begin() const {
        return paginator_.begin();
    }

    auto end() const {
        return paginator_.end();
    }

private:
    std::vector<IteratorRange<Iterator>> paginator_;
};

template <typename Container>
auto Paginate(const Container& c, std::size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}
