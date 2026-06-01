#pragma once
#include "splay_tree_api.h"

template<typename Key, typename Value>
SplayTree<Key, Value>::SplayTree() : _root(nullptr), _size(0) {}

template<typename Key, typename Value>
void SplayTree<Key, Value>::clear(Node* node){
	if (node == nullptr){
		return;
	}
	clear(node->left);
	clear(node->right);
	delete node;
}

template<typename Key, typename Value>
SplayTree<Key, Value>::~SplayTree(){
	clear(_root);
	_root = nullptr;
	_size = 0;
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::rotateLeft(Node* x){
	Node* y = x->right;
	x->right = y->left;
	if (y->left){
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (!x->parent){
		_root = y;
	}
	else if (x == x->parent->left){
		x->parent->left = y;
	}
	else{
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::rotateRight(Node* x){
	Node* y = x->left;
	x->left = y->right;
	if (y->right){
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (!x->parent){
		_root = y;
	}
	else if (x == x->parent->right){
		x->parent->right = y;
	}
	else{
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::splay(Node* x){
	if (!x){
		return;
	}
	while (x->parent){
		Node* p = x->parent;
		Node* g = p->parent;
		if (!g){
			if (x == p->left){
				rotateRight(p);
			}
			else{
				rotateLeft(p);
			}
		}
		else if (x == p->left && p == g->left){
			rotateRight(g);
			rotateRight(p);
		}
		else if (x == p->right && p == g->right){
			rotateLeft(g);
			rotateLeft(p);
		}
		else if (x == p->right && p == g->left){
			rotateLeft(p);
			rotateRight(g);
		}
		else{
			rotateRight(p);
			rotateLeft(g);
		}
	}
	_root = x;
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::insert(const Key& key, const Value& value){
	if (!_root){
		_root = new Node(key, value);
		++_size;
		return;
	}
	Node* current = _root;
	Node* parent = nullptr;
	while (current){
		parent = current;
		if (key < current->key){
			current = current->left;
		}
		else if (current->key < key){
			current = current->right;
		}
		else{
			current->value = value;
			splay(current);
			return;
		}
	}
	Node* node = new Node(key, value);
	node->parent = parent;
	if (key < parent->key){
		parent->left = node;
	}
	else{
		parent->right = node;
	}
	++_size;
	splay(node);
}

template<typename Key, typename Value>
Value* SplayTree<Key, Value>::search(const Key& key){
	Node* current = _root;
	Node* last = nullptr;
	while (current){
		last = current;
		if (key < current->key){
			current = current->left;
		}
		else if (current->key < key){
			current = current->right;
		}
		else{
			splay(current);
			return &_root->value;
		}
	}
	if (last){
		splay(last);
	}
	return nullptr;
}

template<typename Key, typename Value>
const Value* SplayTree<Key, Value>::search(const Key& key) const{
	const Node* current = _root;
	while (current){
		if (key < current->key){
			current = current->left;
		}
		else if (current->key < key){
			current = current->right;
		}
		else{
			return &current->value;
		}
	}
	return nullptr;
}

template<typename Key, typename Value>
bool SplayTree<Key, Value>::remove(const Key& key){
	Node* current = _root;
	while (current){
		if (key < current->key){
			current = current->left;
		}
		else if (current->key < key){
			current = current->right;
		}
		else{
			break;
		}
	}
	if (!current){
		return false;
	}
	splay(current);
	Node* leftSub = _root->left;
	Node* rightSub = _root->right;
	if (leftSub){
		leftSub->parent = nullptr;
	}
	if (rightSub){
		rightSub->parent = nullptr;
	}
	delete _root;
	--_size;

	if (!leftSub){
		_root = rightSub;
	}
	else{
		_root = leftSub;
		Node* maxLeft = leftSub;
		while (maxLeft->right){
			maxLeft = maxLeft->right;
		}
		splay(maxLeft);
		maxLeft->right = rightSub;
		if (rightSub){
			rightSub->parent = maxLeft;
		}
	}
	return true;
}

template<typename Key, typename Value>
bool SplayTree<Key, Value>::isValidBSTImpl(const Node* node,
                                           const Node* naim,
                                           const Node* naib) const{
	if (!node){
		return true;
	}
	if (naim && !(naim->key < node->key)){
		return false;
	}
	if (naib && !(node->key < naib->key)){
		return false;
	}
	return isValidBSTImpl(node->left, naim, node) &&
	       isValidBSTImpl(node->right, node, naib);
}

template<typename Key, typename Value>
bool SplayTree<Key, Value>::isValidBST() const{
	return isValidBSTImpl(_root, nullptr, nullptr);
}

template<typename Key, typename Value>
size_t SplayTree<Key, Value>::size() const{
	return _size;
}

template<typename Key, typename Value>
bool SplayTree<Key, Value>::empty() const{
	return _size == 0;
}
