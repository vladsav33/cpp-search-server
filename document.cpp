#include "document.h"
#include <iostream>

using namespace std;

Document::Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
}

ostream& operator<<(ostream& o, Document document) {
    o << "{ "s
      << "document_id = "s << document.id << ", "s
      << "relevance = "s << document.relevance << ", "s
      << "rating = "s << document.rating << " }"s;
    return o;
}
