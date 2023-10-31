#include "request_queue.h"

using namespace std;

RequestQueue::RequestQueue(const SearchServer& search_server)
        :searchserver(search_server)
{
    time = 0;
    empty_requests_ = 0;
}

template <typename DocumentPredicate>
vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
    vector<Document> result = searchserver.FindTopDocuments(raw_query, document_predicate);
    QueryResult queryResult({result.empty(), ++time >= min_in_day_ ? 0 : time});
    requests_.push_back(queryResult);
    if (result.empty()) {
        empty_requests_++;
    }
    if (requests_.size() > min_in_day_) {
        if (requests_.front().empty) {
            empty_requests_--;
        }
        requests_.pop_front();
    }
    return result;
}

vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
    vector<Document> result = searchserver.FindTopDocuments(raw_query, status);
    QueryResult queryResult({result.empty(), ++time >= min_in_day_ ? 0 : time});
    requests_.push_back(queryResult);
    if (result.empty()) {
        empty_requests_++;
    }
    if (requests_.size() > min_in_day_) {
        if (requests_.front().empty) {
            empty_requests_--;
        }
        requests_.pop_front();
    }
    return result;
}

vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
    vector<Document> result = searchserver.FindTopDocuments(raw_query);
    QueryResult queryResult({result.empty(), ++time >= min_in_day_ ? 0 : time});
    requests_.push_back(queryResult);
    if (result.empty()) {
        empty_requests_++;
    }
    if (requests_.size() > min_in_day_) {
        if (requests_.front().empty) {
            empty_requests_--;
        }
        requests_.pop_front();
    }
    return result;
}

int RequestQueue::GetNoResultRequests() const {
    return empty_requests_;
}