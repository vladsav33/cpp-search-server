#pragma once
#include <deque>
#include "search_server.h"
#include <string>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;
private:
    struct QueryResult {
        bool empty;
        time_t tstamp;
    };
    const SearchServer& searchserver;
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    time_t time;
    int empty_requests_;
};
