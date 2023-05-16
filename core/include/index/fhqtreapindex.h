#ifndef __FHQTREAPINDEX_H__
#define __FHQTREAPINDEX_H__

#include <index/index.h>

/**
* 索引基类
*/
class FHQTreapIndex: public Index {
public:
    /**
    * Construct an Index
    * compare_key is the keys of Index
    */
    FHQTreapIndex(const std::vector<std::unordered_map<std::string, std::any>>& records,
                  const std::vector<std::pair<std::string, std::string>>& fields,
                  const std::unordered_map<std::string, std::string>& field_map,
                  const std::vector<std::string>& compare_key);
    
    /**
     * Destructor an Index
     */
    virtual ~FHQTreapIndex();

    /**
    * build an index (rebuild)
    */
    virtual int build(const std::vector<std::unordered_map<std::string, std::any>>& records,
                      const std::vector<std::pair<std::string, std::string>>& fields,
                      const std::unordered_map<std::string, std::string>& field_map,
                      const std::vector<std::string>& compare_key);

    /**
    * query on index
    * @return if return value isn't zero, then result is invalid!
    */
    virtual int query(const std::vector<std::tuple<std::string, std::string, int>>& conditions,
                      std::vector<int>& result_indexes);

private:
    // ===== FHQ Treap Core =====

    struct Node {
        int c[2], siz, pos, va;
    };

    std::vector<Node> t;
    int rt, tot;

    // basic operations
    void reset();

    void pushup(int x);
    int newNode(int va);
    
    void splits(int x, int siz, int &l, int &r);
    void splitv(int x, int va, int &l, int &r, bool nullIsLess = true);                                              // 将与va相等的值归类至L
    void splitv(int x, const std::unordered_map<std::string, std::any>& va, int &l, int &r, bool nullIsLess = true); // 将与va相等的值归类至L
    void splitvR(int x, int va, int &l, int &r, bool nullIsLess = true);                                             // 将与va相等的值归类至R
    void splitvR(int x, const std::unordered_map<std::string, std::any>& va, int &l, int &r, bool nullIsLess = true);// 将与va相等的值归类至R
    int merge(int l, int r);

    void output(int x, std::vector<int>& result); // 将以x为根的子树的所有va存入result

    // operations
    void insert(int va);
    void erase(int va);
    int getRank(int va);
    int getNumber(int siz);
    int getLower(int va);
    int getUpper(int va);
};

#endif // __FHQTREAPINDEX_H__
