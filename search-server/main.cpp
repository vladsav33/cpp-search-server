#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

struct Document {
    int id;
    double relevance;
};

struct Query {
    set<string> query_words;
    set<string> minus_words;
};

class SearchServer {
public:
    int document_count_ = 0;
    
    void SetStopWords(const string& text) {
        if (text.size() == 0) {
            return;
        }
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        double term_freq = 1.0 / words.size();
        for (const string& s : words) {
            documents_words_freq_[s][document_id] += term_freq;
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    map<string, map<int, double>> documents_words_freq_;
    set<string> stop_words_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (word[0] == '-') {
                query.minus_words.insert(word.substr(1));
            } else {
                query.query_words.insert(word);
            }
        }
        return query;
    }

    double CalculateIdf(const string& query_word) const {
        return log(static_cast<double> (document_count_) / documents_words_freq_.at(query_word).size());
    } 

    vector<Document> FindAllDocuments(const Query& query) const {        
        vector<Document> matched_documents;
        map<int, double> relevance;
        for (const string& query_word : query.query_words) {
            if (documents_words_freq_.find(query_word) == documents_words_freq_.end()) {
                continue;
            }
            double idf = CalculateIdf(query_word);
            for (auto [key, value] : documents_words_freq_.at(query_word)) {
                relevance[key] += idf * value;
            }
        }
        for (const string& minus_word : query.minus_words) {
            if (documents_words_freq_.find(minus_word) == documents_words_freq_.end()) {
                continue;
            }
            for (auto& [key, value] : documents_words_freq_.at(minus_word)) {
                relevance.erase(key);
            }
        }
        
        for (auto& [key, value] : relevance) {
            matched_documents.push_back({key, value});
        }

        return matched_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    search_server.document_count_ = ReadLineWithNumber();
    for (int document_id = 0; document_id < search_server.document_count_; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
