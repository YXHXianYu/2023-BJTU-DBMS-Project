#ifndef __INDEX_H__
#define __INDEX_H__

#include <vector>
#include <unordered_map>
#include <any>
#include <string>

/**
* 索引基类
*/
class Index {
public:
    /**
    * Construct an Index
    * compare_key is the keys of Index
    */
    Index(const std::vector<std::unordered_map<std::string, std::any>>& records, const std::vector<std::pair<std::string, std::string>>& field_map, const std::vector<std::string>& compare_key) {
        state = 1; // invalid
    }

    /**
    * build an index (rebuild)
    */
    virtual int build(const std::vector<std::unordered_map<std::string, std::any>>& records, const std::vector<std::pair<std::string, std::string>>& field_map, const std::vector<std::string>& compare_key);

    /**
    * query on index
    * @return if return value isn't zero, then result is invalid!
    */
    virtual int query(const std::vector<std::tuple<std::string, std::string, int>>& conditions, std::vector<int>& result_indexes);

    /**
    * get index state
    * 0 if valid
    * != 0 if invalid
    */
    int getState() const {
        return state;
    }

protected:
    int state;
};

#endif // __INDEX_H__
