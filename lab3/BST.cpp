#include "BST.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <limits>
using namespace std;

BinarySearchTree::Node::Node(Key key, Value value,
             Node *parent, Node *left,
             Node *right){
	keyValuePair.first=key;
	keyValuePair.second=value;
	this->parent = parent;
	this->left = left;
	this->right = right;
}
BinarySearchTree::Node::Node(const Node &other){
	keyValuePair=other.keyValuePair;
	parent = nullptr;
	if (other.left){
		left = new Node(*other.left);
		left->parent = this;
	}
	else{
		left = nullptr;
	}
	if (other.right){
		right = new Node(*other.right);
		right->parent = this;
	}
	else{
		right = nullptr;
	}
}
bool BinarySearchTree::Node::operator==(const Node &other) const{
	return keyValuePair == other.keyValuePair;
}
void BinarySearchTree::Node::output_node_tree() const{
	if (left){
		left->output_node_tree();
	}
	if (keyValuePair.first != std::numeric_limits<Key>::max()){
		std::cout << keyValuePair.first
			  << " : "
			  << keyValuePair.second
			  << std::endl;
	}
	if (right){
		right->output_node_tree();
	}
}
void BinarySearchTree::Node::insert(const Key &key, const Value &value){
	if (key < keyValuePair.first){
		if (left){
			left->insert(key, value);
		}
		else{
			left = new Node(key,value,this);
		}
	}
	else{
		if (right && right->keyValuePair.first != std::numeric_limits<Key>::max()){
			right->insert(key, value);
		}
		else{
			Node* oldRight = right;
			right = new Node(key, value, this);
			right->right = oldRight;
			if (oldRight){
				oldRight->parent = right;
			}
		}
	}
}
void BinarySearchTree::Node::erase(const Key &key){
	if (key < keyValuePair.first && (left)){
		left->erase(key);
	}
	else if (key > keyValuePair.first && (right) &&
	         right->keyValuePair.first != std::numeric_limits<Key>::max()){
		right->erase(key);
	}
	else{
		if (key != keyValuePair.first){
			return;
		}
		Node* sentinel = nullptr;
		if (right && right->keyValuePair.first == std::numeric_limits<Key>::max()){
			sentinel = right;
			right = nullptr;
		}
		if (!right && !left){
			if (parent){
				if (this == parent->left){
					parent->left = nullptr;
				}
				else if (this == parent->right){
					parent->right = nullptr;
				}
			}
			if (sentinel){
				Node* rightmost = parent;
				while (rightmost && rightmost->right){
					rightmost = rightmost->right;
				}
				if (rightmost){
					rightmost->right = sentinel;
					sentinel->parent = rightmost;
				}
			}
			delete this;
			return;
		}
		else if (right && !left){
			if (parent){
				if (this == parent->left){
					parent->left = right;
					right->parent = parent;
				}
				else if (this == parent->right){
					parent->right = right;
					right->parent = parent;
				}
			}
			delete this;
			return;
		}
		else if (left && !right){
			if (parent){
				if(this == parent->left){
					parent->left = left;
					left->parent = parent;
				}
				else if (this == parent->right){
					parent->right = left;
					left->parent = parent;
				}
			}
			if (sentinel){
				Node* rightmost = left;
				while (rightmost->right){
					rightmost = rightmost->right;
				}
				rightmost->right = sentinel;
				sentinel->parent = rightmost;
			}
			delete this;
			return;
		}
		else{
			Node* temp = right;
			while (temp->left){
				temp = temp->left;
			}
			if (temp->parent != this) {
				temp->parent->left = temp->right;
				if (temp->right){
					temp->right->parent = temp->parent;
				}
				temp->right = right;
				if (right){
					right->parent = temp;
				}
			}
			temp->left = left;
			temp->parent = parent;
			if (left){
				left->parent = temp;
			}
			if (parent) {
				if (this == parent->left) {
					parent->left = temp;
				}
				else if (this == parent->right) {
					parent->right = temp;
				}
			}
			delete this;
			return;
		}
	}
}
void BinarySearchTree::clear(Node* node){
	if (node == nullptr){
		return;
	}
	clear(node->left);
	clear(node->right);
	delete node;
}

BinarySearchTree::~BinarySearchTree()
{
	clear(_root);
	_root = nullptr;
	_size = 0;
}
BinarySearchTree::BinarySearchTree(const BinarySearchTree &other){
	_size = other._size;
	if (other._root){
		_root = new Node(*other._root);
	}
	else{
		_root = nullptr;
	}
}
BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other){
	if (this != &other){
		BinarySearchTree temp(other);
		swap(_root, temp._root);
		swap(_size, temp._size);
	}
	return *this;
}
BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept{
	_root = other._root;
	_size = other._size;
	other._root = nullptr;
	other._size = 0;
}
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept{
	if (this != &other){
		clear(_root);
		_root = other._root;
		_size = other._size;
		other._root = nullptr;
		other._size = 0;
	}
	return *this;
}
BinarySearchTree::Iterator::Iterator(Node *node){
	_node=node;
}
pair<Key, Value>& BinarySearchTree::Iterator::operator*(){
	if (!_node){
		throw runtime_error("Dereferencing end iterator");
	}
	return _node->keyValuePair;
}
const pair<Key, Value>& BinarySearchTree::Iterator::operator*() const{
	if (!_node){
		throw runtime_error("Dereferencing end iterator");
	}
	return _node->keyValuePair;
}
pair<Key, Value>* BinarySearchTree::Iterator::operator->(){
	if (!_node){
		throw runtime_error("Dereferencing end iterator");
	}
	return &_node->keyValuePair;
}
const pair<Key, Value>* BinarySearchTree::Iterator::operator->() const{
	if (!_node){
		throw runtime_error("Dereferencing end iterator");
	}
	return &_node->keyValuePair;
}
bool BinarySearchTree::Iterator::operator==(const Iterator &other) const{
	return _node==other._node;
}
bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const{
	return _node!=other._node;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(){
	if (!_node){
		throw runtime_error("Incrementing end iterator");
	}
	if (_node->keyValuePair.first == std::numeric_limits<Key>::max() && !_node->right){
		throw runtime_error("Incrementing end iterator");
	}
	if (_node->right){
		_node=_node->right;
		while(_node->left){
			_node = _node->left;
		}
	}
	else{
		Node* p = _node->parent;
		while(p && _node==p->right){
			_node = p;
			p=p->parent;
		}
		_node=p;
	}
	return *this;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int){
	Iterator temp = *this;
	++(*this);
	return temp;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(){
	if (!_node){
		throw runtime_error("Decrementing end iterator");
	}
	if (_node->left){
		_node=_node->left;
		while(_node->right){
			_node = _node->right;
		}
	}
	else{
		Node* p = _node->parent;
		while(p && _node==p->left){
			_node = p;
			p=p->parent;
		}
		_node=p;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int){
	Iterator temp = *this;
	--(*this);
	return temp;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node){
	_node=node;
}
const pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const{
	if (!_node){
		throw runtime_error("Dereferencing end iterator");
	}
	return _node->keyValuePair;
}
const pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const{
	if (!_node){
		throw runtime_error("Dereferencing end iterator");
	}
	return &_node->keyValuePair;
}
bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const{
	return _node==other._node;
}
bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const{
	return _node!=other._node;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(){
	if (!_node){
		throw runtime_error("Incrementing end iterator");
	}
	if (_node->keyValuePair.first == std::numeric_limits<Key>::max() && !_node->right){
		throw runtime_error("Incrementing end iterator");
	}
	if (_node->right){
		_node=_node->right;
		while(_node->left){
			_node = _node->left;
		}
	}
	else{
		const Node* p = _node->parent;
		while(p && _node==p->right){
			_node = p;
			p=p->parent;
		}
		_node=p;
	}
	return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int){
	ConstIterator temp = *this;
	++(*this);
	return temp;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(){
	if (!_node){
		throw runtime_error("Decrementing end iterator");
	}
	if (_node->left){
		_node=_node->left;
		while(_node->right){
			_node = _node->right;
		}
	}
	else{
		const Node* p = _node->parent;
		while(p && _node==p->left){
			_node = p;
			p=p->parent;
		}
		_node=p;
	}
	return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int){
	ConstIterator temp = *this;
	--(*this);
	return temp;
}
pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key){
	Iterator first = find(key);
	if (first == end()){
		return {end(), end()};
	}
	Iterator last = first;
	while (last != end() && last->first == key){
		++last;
	}
	return {first, last};
}

pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const{
	ConstIterator first = find(key);
	if (first == cend()){
		return {cend(), cend()};
	}
	ConstIterator last = first;
	while (last != cend() && last->first == key){
		++last;
	}
	return {first, last};
}
BinarySearchTree::Iterator BinarySearchTree::end(){
	if (!_root){
		return Iterator(nullptr);
	}
	Node* current = _root;
	while (current->right){
		current = current->right;
	}
	return Iterator(current);
}
BinarySearchTree::ConstIterator BinarySearchTree::cend() const{
	if (!_root){
		return ConstIterator(nullptr);
	}
	Node* current = _root;
	while (current->right){
		current = current->right;
	}
	return ConstIterator(current);
}
BinarySearchTree::Iterator BinarySearchTree::begin(){
	if (_root == nullptr){
		return end();
	}
	Node* current = _root;
	while (current->left)
	{
		current = current->left;
	}
	return Iterator(current);

}
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const{
	if (_root == nullptr){
		return cend();
	}
	Node* current = _root;
	while (current->left)
	{
		current = current->left;
	}
	return ConstIterator(current);
}
BinarySearchTree::Iterator BinarySearchTree::find(const Key &key){
	Node* current = _root;
	while (current){
		if (key < current->keyValuePair.first)
		{
			current = current->left;
		}
		else if (key > current->keyValuePair.first)
		{
			current = current->right;
		}
		else
		{
			return Iterator(current);
		}
	}
	return end();
}
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const{
	Node* current = _root;
	while (current){
		if (key < current->keyValuePair.first)
		{
			current = current->left;
		}
		else if (key > current->keyValuePair.first)
		{
			current = current->right;
		}
		else
		{
			return ConstIterator(current);
		}
	}
	return cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::min() const{
	return cbegin();
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const{
	ConstIterator temp1 = find(key);
	if (temp1 == cend()){
		return cend();
	}
	ConstIterator temp2 = temp1;
	++temp1;
	while (temp1 != cend() && temp1->first == key){
		if (temp1->second < temp2->second){
			temp2 = temp1;
		}
		++temp1;
	}
	return temp2;
}
BinarySearchTree::ConstIterator BinarySearchTree::max() const{
	if (!_root){
		return cend();
	}
	return --cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const{
	ConstIterator temp1 = find(key);
	if (temp1 == cend()){
		return cend();
	}
	ConstIterator temp2 = temp1;
	++temp1;
	while (temp1 != cend() && temp1->first == key){
		if (temp1->second > temp2->second){
			temp2 = temp1;
		}
		++temp1;
	}
	return temp2;
}
void BinarySearchTree::insert(const Key &key, const Value &value){
	if (!_root){
		_root = new Node(key, value);
		_root->right = new Node(std::numeric_limits<Key>::max(),
		                        Value(),
		                        _root);
		_size++;
		return;
	}
	Node* current = _root;
	while (current->right &&
	       current->right->keyValuePair.first != std::numeric_limits<Key>::max()){
		current = current->right;
	}
	Node* endNode = current->right;
	_root->insert(key, value);
	current = _root;
	while (current->right &&
	       current->right->keyValuePair.first != std::numeric_limits<Key>::max()){
		current = current->right;
	}
	if (!current->right){
		current->right = endNode;
		endNode->parent = current;
	}
	_size++;
}

void BinarySearchTree::erase(const Key &key){
	if (!_root) return;

	while (true){
		Node* node = _root;
		while (node){
            if(key < node->keyValuePair.first){
                node = node->left;
            }
            else if (key > node->keyValuePair.first){
                node = node->right;
            }
			else break;
		}
		if (!node) break;
		if (node->keyValuePair.first == std::numeric_limits<Key>::max()) break;
		if (node == _root){
			if (!_root->left && !_root->right){
				delete _root;
				_root = nullptr;
			}
			else if (_root->right && !_root->left){
				Node* newRoot = _root->right;
				if (newRoot->keyValuePair.first == std::numeric_limits<Key>::max()){
					delete newRoot;
					delete _root;
					_root = nullptr;
				}
				else{
					newRoot->parent = nullptr;
					delete _root;
					_root = newRoot;
				}
			}
			else if (_root->left && !_root->right){
				Node* newRoot = _root->left;
				newRoot->parent = nullptr;
				delete _root;
				_root = newRoot;
			}
			else{
				if (_root->right->keyValuePair.first == std::numeric_limits<Key>::max()){
					Node* sentinel = _root->right;
					Node* newRoot = _root->left;
					newRoot->parent = nullptr;
					Node* rightmost = newRoot;
					while (rightmost->right){
						rightmost = rightmost->right;
					}
					rightmost->right = sentinel;
					sentinel->parent = rightmost;
					delete _root;
					_root = newRoot;
				}
				else{
					Node* temp = _root->right;
					while (temp->left){
						temp = temp->left;
					}
					if (temp->parent != _root){
						temp->parent->left = temp->right;
						if (temp->right){
							temp->right->parent = temp->parent;
						}
						temp->right = _root->right;
						_root->right->parent = temp;
					}
					temp->left = _root->left;
					if (_root->left){
						_root->left->parent = temp;
					}
					temp->parent = nullptr;
					delete _root;
					_root = temp;
				}
			}
		}
		else{
			node->erase(key);
		}
		--_size;
	}
}
size_t BinarySearchTree::size() const{
	return _size;
}

void BinarySearchTree::output_tree() {
	if (!_root) return;
	_root->output_node_tree();
}
