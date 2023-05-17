#include "index/index.h"

Index::Index(const std::vector<std::unordered_map<std::string, std::any>>& records,
             const std::vector<std::pair<std::string, std::string>>& fields,
             const std::unordered_map<std::string, std::string>& field_map,
             const std::vector<std::string>& compare_key) {
    _state = 1;
}

Index::~Index() {
}

const std::vector<std::string>& Index::getCompareKey() const {
    return _compare_key;
}

