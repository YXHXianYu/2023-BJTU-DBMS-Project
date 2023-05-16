#ifndef __INDEX_H__
#define __INDEX_H__

#include <vector>
#include <unordered_map>
#include <any>
#include <string>
#include <memory>

#include "colasqltool.h"
#include "constants.h"

/**
* 索引基类
*/
class Index {
public:
    /**
    * Construct an Index
    * compare_key is the keys of Index
    */
    Index(const std::vector<std::unordered_map<std::string, std::any>>& records,
          const std::vector<std::pair<std::string, std::string>>& fields,
          const std::unordered_map<std::string, std::string>& field_map,
          const std::vector<std::string>& compare_key);

    /**
     * Destructor an Index
     */
    virtual ~Index();

    /**
    * build an index (rebuild)
    */
    virtual int build(const std::vector<std::unordered_map<std::string, std::any>>& records,
                      const std::vector<std::pair<std::string, std::string>>& fields,
                      const std::unordered_map<std::string, std::string>& field_map,
                      const std::vector<std::string>& compare_key) = 0;

    /**
    * query on index
    * @return if return value isn't zero, then result is invalid!
    */
    virtual int query(const std::vector<std::tuple<std::string, std::string, int>>& conditions,
                      std::vector<int>& result_indexes) = 0;

    /**
    * get index state
    * 0 if valid
    * != 0 if invalid
    */
    int getState() const {
        return _state;
    }
    
    /**
    * compare two records
    * -1 if r[i] < r[j]
    * 0  if r[i] == r[j]
    * 1  if r[i] > r[j]
    */
    int compare(int i, int j) const {
        if(i >= _records_ptr->size() || j >= _records_ptr->size()) return 0;
        for(const auto& fieldName: _compare_key) {
            bool iHave = _records_ptr->at(i).count(fieldName) > 0;
            bool jHave = _records_ptr->at(j).count(fieldName) > 0;

            int ret;
            if(iHave && jHave)
                ret = ColasqlTool::CompareAny(_records_ptr->at(i).at(fieldName), _records_ptr->at(j).at(fieldName));
            else if(iHave && !jHave)
                ret = 1;
            else if(!iHave && jHave)
                ret = -1;
            else
                ret = 0;

            if(ret == 0) continue;
            return ret;
        }
        return 0;
    }

    /**
    * compare two records
    * -1 if r[i] < r[j]
    * 0  if r[i] == r[j]
    * 1  if r[i] > r[j]
    */
    int compare(const std::unordered_map<std::string, std::any>& i, int j) const {
        if(j >= _records_ptr->size()) return 0;
        for(const auto& fieldName: _compare_key) {
            bool iHave = i.count(fieldName) > 0;
            bool jHave = _records_ptr->at(j).count(fieldName) > 0;

            int ret;
            if(iHave && jHave)
                ret = ColasqlTool::CompareAny(i.at(fieldName), _records_ptr->at(j).at(fieldName));
            else if(iHave && !jHave)
                ret = 1;
            else if(!iHave && jHave)
                ret = -1;
            else
                ret = 0;

            if(ret == 0) continue;
            return ret; }
        return 0;
    }

protected:
    int _state; // 记录index状态

    std::unique_ptr<const std::vector<std::unordered_map<std::string, std::any>>> _records_ptr;
    std::unique_ptr<const std::unordered_map<std::string, std::string>> _field_map_ptr;
    std::vector<std::string> _compare_key;
};

#endif // __INDEX_H__
