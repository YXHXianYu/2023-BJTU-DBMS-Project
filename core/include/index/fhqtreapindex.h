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
    FHQTreapIndex(const std::vector<std::unordered_map<std::string, std::any>>& records, const std::vector<std::pair<std::string, std::string>>& field_map, const std::vector<std::string>& compare_key);

    /**
    * build an index (rebuild)
    */
    virtual int build(const std::vector<std::unordered_map<std::string, std::any>>& records, const std::vector<std::pair<std::string, std::string>>& field_map, const std::vector<std::string>& compare_key);

    /**
    * query on index
    * @return if return value isn't zero, then result is invalid!
    */
    virtual int query(const std::vector<std::tuple<std::string, std::string, int>>& conditions, std::vector<int>& result_indexes);

private:
    struct Node {
        int c[2], siz, pos, va;
    };

    std::vector<Node> t;
    int rt, root;

    void pushup(int x);
    int newNode(int va);
    
    void splits(int x, int siz, int &l, int &r);
    void splitv(int x, int va, int &l, int &r);
    int merge(int l, int r);
};


	
	void splits(int x, int siz, int &l, int &r) {
		if(x == 0) {l = r = 0; return;}
		if(siz <= t[LC].siz) {r = x; splits(LC, siz, l, LC);}
		else {l = x; splits(RC, siz-t[LC].siz-1, RC, r);}
		pushup(x);
	}
	void splitv(int x, int va, int &l, int &r) {
		if(x == 0) {l = r = 0; return;}
		if(va < t[x].va) {r = x; splitv(LC, va, l, LC);}
		else {l = x; splitv(RC, va, RC, r);}
		pushup(x);
	}
	int merge(int l, int r) {
		if(l == 0 || r == 0) return l + r;
		if(t[l].pos < t[r].pos) {t[l].c[1] = merge(t[l].c[1], r); pushup(l); return l;}
		else {t[r].c[0] = merge(l, t[r].c[0]); pushup(r); return r;}
	}
	
	void insert(int va) {
		int a, b;
		splitv(rt, va, a, b);
		rt = merge(merge(a, new_node(va)), b);
	}
	void erase(int va) {
		int a, b, c;
		splitv(rt, va, a, b);
		splits(a, t[a].siz-1, a, c);
		rt = merge(a, b);
	}
	int getrank(int va) {
		int a, b, ans;
		splitv(rt, va-1, a, b);
		ans = t[a].siz + 1;
		rt = merge(a, b);
		return ans;
	}
	int getnumber(int siz) {
		int a, b, c, ans;
		splits(rt, siz, a, b);
		splits(a, siz-1, a, c);
		ans = t[c].va;
		rt = merge(merge(a, c), b);
		return ans;
	}
	int getlower(int va) {
		int a, b, c, ans;
		splitv(rt, va-1, a, b);
		splits(a, t[a].siz-1, a, c);
		ans = t[c].va;
		rt = merge(merge(a, c), b);
		return ans;
	}
	int getupper(int va) {
		int a, b, c, ans;
		splitv(rt, va, a, b);
		splits(b, 1, b, c);
		ans = t[b].va;
		rt = merge(a, merge(b, c));
		return ans;
	}

#endif // __FHQTREAPINDEX_H__
