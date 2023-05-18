#include "index/fhqtreapindex.h"

#include <iostream>

FHQTreapIndex::FHQTreapIndex(const std::vector<std::unordered_map<std::string, std::any>>& records,
                             const std::vector<std::pair<std::string, std::string>>& fields,
                             const std::unordered_map<std::string, std::string>& field_map,
                             const std::vector<std::string>& compare_key):
    Index(records, fields, field_map, compare_key) {
    _records_ptr = nullptr;
    _field_map_ptr = nullptr;
    build(records, fields, field_map, compare_key);
}

FHQTreapIndex::~FHQTreapIndex() {
    // delete _records_ptr;
    _records_ptr = nullptr;
    // delete _field_map_ptr;
    _field_map_ptr = nullptr;
}

int FHQTreapIndex::build(const std::vector<std::unordered_map<std::string, std::any>>& records,
                         const std::vector<std::pair<std::string, std::string>>& fields,
                         const std::unordered_map<std::string, std::string>& field_map,
                         const std::vector<std::string>& compare_key) {

    // ===== Records =====
    // delete _records_ptr;
    _records_ptr = nullptr;
    _records_ptr = new std::vector<std::unordered_map<std::string, std::any>>(records);

    // delete _field_map_ptr;
    _field_map_ptr = nullptr;
    _field_map_ptr = new std::unordered_map<std::string, std::string>(field_map);

    // ===== Compare Key =====
    // n^2 optimization could be used here
    _compare_key = compare_key;
    for(const auto& field: fields) {
        int flag = 1;
        for(const auto& key: _compare_key) {
            if(field.first == key) {
                flag = 0;
                break;
            }
        }
        if(flag) {
            _compare_key.push_back(field.first);
        }
    }

    // ===== Build Tree =====
    reset();
    t = std::vector<Node>(records.size() + 2000);
    for(int i = 0; i < records.size(); i++) {
        insert(i);
    }

    // ===== End =====
    _state = 0;
    return _state;
}

int FHQTreapIndex::query(const std::vector<std::tuple<std::string, std::string, int>>& conditions, std::vector<int>& result_indexes) {
    result_indexes.clear();

    if(_state != 0) {
        return kFailedIndexNotBuild;
    }

    // 用作比较
    std::unordered_map<std::string, std::any> recordBasedOnConditions;

    // 构建 recordBasedOnConditions
    for(int i = 0, cnt = 0; i < _compare_key.size() && cnt < conditions.size(); i++) {
        int flag = false;
        for(const auto& [name, value, type]: conditions) {
            if(type != kEqualConditon) {
                return kFailedConditionRelationNotSupport;
            }
            if(name != _compare_key[i]) {
                continue;
            }
            if(recordBasedOnConditions.count(name) > 0) {
                return kFailedConditionDuplicate;
            }
            recordBasedOnConditions.insert({name, ColasqlTool::GetAnyByTypeAndValue(_field_map_ptr->at(name), value)});
            flag = true;
            break;
        }
        if(flag) { // this key is in condition
            cnt += 1;            
        } else { // this key is not in condition
            return kFailedIndexCouldnotSpeedup;
        }
    }

    // for(auto& [str, any]: recordBasedOnConditions) {
    //     std::cout << str << ", " << ColasqlTool::AnyToString(any) << std::endl;
    // }


    // query
    int a, b, c;
    splitvR(rt, recordBasedOnConditions, a, b);
    splitv(b, recordBasedOnConditions, b, c, false);
    
    output(b, result_indexes);

    rt = merge(a, merge(b, c));

    return 0; // success
}

// ===== FHQ Treap =====
#define LC (t[x].c[0])
#define RC (t[x].c[1])

void FHQTreapIndex::reset() {
    rt = 0;
    tot = 0;
}

void FHQTreapIndex::pushup(int x) {
    t[x].siz = t[LC].siz + t[RC].siz + 1;
}

int FHQTreapIndex::newNode(int va) {
    tot++;
    t[tot].c[0] = 0;
    t[tot].c[1] = 0;
    t[tot].siz = 1;
    t[tot].pos = rand() << 15 | rand();
    t[tot].va = va;
    return tot;
}

void FHQTreapIndex::splits(int x, int siz, int &l, int &r) {
    if(x == 0) {
        l = r = 0;
        return;
    }
    if(siz <= t[LC].siz) {
        r = x;
        splits(LC, siz, l, LC);
    } else {
        l = x;
        splits(RC, siz-t[LC].siz-1, RC, r);
    }
    pushup(x);
}

void FHQTreapIndex::splitv(int x, int va, int &l, int &r, bool nullIsLess) {
    if(x == 0) {
        l = r = 0;
        return;
    }
    if(compare(va, t[x].va, nullIsLess) < 0) {
        r = x;
        splitv(LC, va, l, LC, nullIsLess);
    } else {
        l = x;
        splitv(RC, va, RC, r, nullIsLess);
    }
    pushup(x);
}

void FHQTreapIndex::splitv(int x, const std::unordered_map<std::string, std::any>& va, int &l, int &r, bool nullIsLess) {
    if(x == 0) {
        l = r = 0;
        return;
    }
    if(compare(va, t[x].va, nullIsLess) < 0) {
        r = x;
        splitv(LC, va, l, LC, nullIsLess);
    } else {
        l = x;
        splitv(RC, va, RC, r, nullIsLess);
    }
    pushup(x);
}

void FHQTreapIndex::splitvR(int x, int va, int &l, int &r, bool nullIsLess) {
    if(x == 0) {
        l = r = 0;
        return;
    }
    if(compare(va, t[x].va, nullIsLess) <= 0) {
        r = x;
        splitvR(LC, va, l, LC, nullIsLess);
    } else {
        l = x;
        splitvR(RC, va, RC, r, nullIsLess);
    }
    pushup(x);
}

void FHQTreapIndex::splitvR(int x, const std::unordered_map<std::string, std::any>& va, int &l, int &r, bool nullIsLess) {
    if(x == 0) {
        l = r = 0;
        return;
    }
    if(compare(va, t[x].va, nullIsLess) <= 0) {
        r = x;
        splitvR(LC, va, l, LC, nullIsLess);
    } else {
        l = x;
        splitvR(RC, va, RC, r, nullIsLess);
    }
    pushup(x);
}

int FHQTreapIndex::merge(int l, int r) {
    if(l == 0 || r == 0) return l + r;
    if(t[l].pos < t[r].pos) {
        t[l].c[1] = merge(t[l].c[1], r);
        pushup(l);
        return l;
    } else {
        t[r].c[0] = merge(l, t[r].c[0]);
        pushup(r);
        return r;
    }
}

void FHQTreapIndex::output(int x, std::vector<int>& result) {
    if(x == 0) return;
    result.push_back(t[x].va);
    output(LC, result);
    output(RC, result);
}

#undef LC
#undef RC


void FHQTreapIndex::insert(int va) {
    int a, b;
    splitv(rt, va, a, b);
    rt = merge(merge(a, newNode(va)), b);
}

void FHQTreapIndex::erase(int va) {
    int a, b, c;
    splitv(rt, va, a, b);
    splitv(a, t[a].siz - 1, a, c);
    rt = merge(a, b);
}

int FHQTreapIndex::getRank(int va) {
    int a, b, ans;
    splitv(rt, va - 1, a, b);
    ans = t[a].siz + 1;
    rt = merge(a, b);
    return ans;
}

int FHQTreapIndex::getNumber(int siz) {
    int a, b, c, ans;
    splits(rt, siz, a, b);
    splits(a, siz - 1, a, c);
    ans = t[c].va;
    rt = merge(merge(a, c), b);
    return ans;
}

int FHQTreapIndex::getLower(int va) {
    int a, b, c, ans;
    splitv(rt, va - 1, a, b);
    splits(a, t[a].siz - 1, a, c);
    ans = t[c].va;
    rt = merge(merge(a, c), b);
    return ans;
}

int FHQTreapIndex::getUpper(int va) {
    int a, b, c, ans;
    splitv(rt, va, a, b);
    splits(b, 1, b, c);
    ans = t[b].va;
    rt = merge(a, merge(b, c));
    return ans;
}

