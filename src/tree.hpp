#pragma once

#include <vector>
#include <string>
#include <map>

template<typename INDEX_T, typename MAP_T, typename DATA_T = void*>
struct tree {
	struct node {
		tree* next;
		MAP_T data;
	};
	std::map<INDEX_T, node> map;
	DATA_T data;
};


// Adds a subtree at index "i" if one doesn't exist yet.
template<typename INDEX_T, typename MAP_T, typename DATA_T = void*>
inline tree<INDEX_T, MAP_T, DATA_T>* make_tree(tree<INDEX_T, MAP_T, DATA_T>* tree_obj, const INDEX_T& i) {
	auto m = &tree_obj->map[i];
	if (!m->next)
		m->next = new tree<INDEX_T, MAP_T, DATA_T>();
	return m->next;
}

// Retrieves subtree to the end of the path.  If an intermediate step is missing, add a new one.
template<typename INDEX_T, typename MAP_T, typename DATA_T = void*>
tree<INDEX_T, MAP_T, DATA_T>& trail_tree(tree<INDEX_T, MAP_T, DATA_T>* node, const std::vector<INDEX_T>& path) {
	auto beg = path.begin();
	auto end = path.end();
	for (auto i = beg; i != end; ++i)
		node = make_tree(node, *i);
	return node;
}

// Retrieves data node to the end of the path.  If an intermediate step is missing, add a new one.
template<typename INDEX_T, typename MAP_T, typename DATA_T = void*>
typename tree<INDEX_T, MAP_T, DATA_T>::node& trail_data(tree<INDEX_T, MAP_T, DATA_T>* node, const std::vector<INDEX_T>& path) {
	auto beg = path.begin();
	auto end = path.end() - 1;
	for (auto i = beg; i != end; ++i)
		node = make_tree(node, *i);
	return node->map[*end];
}