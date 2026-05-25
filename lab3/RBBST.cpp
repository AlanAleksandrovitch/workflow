#include "RBBST.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <limits>
using namespace std;

static const bool RED   = false;
static const bool BLACK = true;

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
	color = other.color;
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
void BinarySearchTree::Node::insert(const Key &, const Value &){}
void BinarySearchTree::Node::erase(const Key &){}

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
	Node* match = nullptr;
	while (current){
		if (key < current->keyValuePair.first){
			current = current->left;
		}
		else if (key > current->keyValuePair.first){
			current = current->right;
		}
		else{
			match = current;
			current = current->left;
		}
	}
	if (match){
		return Iterator(match);
	}
	return end();
}
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const{
	Node* current = _root;
	Node* match = nullptr;
	while (current){
		if (key < current->keyValuePair.first){
			current = current->left;
		}
		else if (key > current->keyValuePair.first){
			current = current->right;
		}
		else{
			match = current;
			current = current->left;
		}
	}
	if (match){
		return ConstIterator(match);
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

BinarySearchTree::Node* BinarySearchTree::detachSentinel(){
	if (!_root){
		return nullptr;
	}
	Node* cur = _root;
	while (cur->right){
		cur = cur->right;
	}
	if (cur->keyValuePair.first != std::numeric_limits<Key>::max()){
		return nullptr;
	}
	Node* sent = cur;
	if (sent->parent){
		sent->parent->right = nullptr;
		sent->parent = nullptr;
	}
	else{
		_root = nullptr;
	}
	return sent;
}

void BinarySearchTree::attachSentinel(Node* sent){
	if (!sent){
		return;
	}
	sent->color = BLACK;
	sent->left = nullptr;
	sent->right = nullptr;
	if (!_root){
		_root = sent;
		sent->parent = nullptr;
		return;
	}
	Node* cur = _root;
	while (cur->right){
		cur = cur->right;
	}
	cur->right = sent;
	sent->parent = cur;
}

void BinarySearchTree::rotateLeft(Node* x){
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

void BinarySearchTree::rotateRight(Node* x){
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

void BinarySearchTree::insertFixup(Node* z){
	while (z->parent && z->parent->color == RED){
		Node* gp = z->parent->parent;
		if (!gp){
			break;
		}
		if (z->parent == gp->left){
			Node* y = gp->right;
			if (y && y->color == RED){
				z->parent->color = BLACK;
				y->color = BLACK;
				gp->color = RED;
				z = gp;
			}
			else{
				if (z == z->parent->right){
					z = z->parent;
					rotateLeft(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rotateRight(z->parent->parent);
			}
		}
		else{
			Node* y = gp->left;
			if (y && y->color == RED){
				z->parent->color = BLACK;
				y->color = BLACK;
				gp->color = RED;
				z = gp;
			}
			else{
				if (z == z->parent->left){
					z = z->parent;
					rotateRight(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rotateLeft(z->parent->parent);
			}
		}
	}
	if (_root){
		_root->color = BLACK;
	}
}

void BinarySearchTree::insert(const Key &key, const Value &value){
	if (key == std::numeric_limits<Key>::max()){
		throw std::invalid_argument("ключ UINT32_MAX зарезервирован под sentinel");
	}
	Node* sent = detachSentinel();

	Node* z = new Node(key, value);
	z->color = RED;

	Node* y = nullptr;
	Node* x = _root;
	while (x){
		y = x;
		if (key < x->keyValuePair.first){
			x = x->left;
		}
		else{
			x = x->right;
		}
	}
	z->parent = y;
	if (!y){
		_root = z;
	}
	else if (key < y->keyValuePair.first){
		y->left = z;
	}
	else{
		y->right = z;
	}

	insertFixup(z);

	if (!sent){
		sent = new Node(std::numeric_limits<Key>::max(), Value());
	}
	attachSentinel(sent);

	++_size;
}

void BinarySearchTree::transplant(Node* u, Node* v){
	if (!u->parent){
		_root = v;
	}
	else if (u == u->parent->left){
		u->parent->left = v;
	}
	else{
		u->parent->right = v;
	}
	if (v){
		v->parent = u->parent;
	}
}

BinarySearchTree::Node* BinarySearchTree::minNode(Node* node) const{
	while (node && node->left){
		node = node->left;
	}
	return node;
}

void BinarySearchTree::eraseFixup(Node* x, Node* xParent){
	while (x != _root && (x == nullptr || x->color == BLACK)){
		if (xParent == nullptr){
			break;
		}
		if (x == xParent->left){
			Node* w = xParent->right;
			if (w && w->color == RED){
				w->color = BLACK;
				xParent->color = RED;
				rotateLeft(xParent);
				w = xParent->right;
			}
			bool wLB = (!w || !w->left  || w->left->color  == BLACK);
			bool wRB = (!w || !w->right || w->right->color == BLACK);
			if (wLB && wRB){
				if (w){
					w->color = RED;
				}
				x = xParent;
				xParent = x ? x->parent : nullptr;
			}
			else{
				if (wRB){
					if (w && w->left){
						w->left->color = BLACK;
					}
					if (w){
						w->color = RED;
						rotateRight(w);
					}
					w = xParent->right;
				}
				if (w){
					w->color = xParent->color;
				}
				xParent->color = BLACK;
				if (w && w->right){
					w->right->color = BLACK;
				}
				rotateLeft(xParent);
				x = _root;
				break;
			}
		}
		else{
			Node* w = xParent->left;
			if (w && w->color == RED){
				w->color = BLACK;
				xParent->color = RED;
				rotateRight(xParent);
				w = xParent->left;
			}
			bool wRB = (!w || !w->right || w->right->color == BLACK);
			bool wLB = (!w || !w->left  || w->left->color  == BLACK);
			if (wRB && wLB){
				if (w){
					w->color = RED;
				}
				x = xParent;
				xParent = x ? x->parent : nullptr;
			}
			else{
				if (wLB){
					if (w && w->right){
						w->right->color = BLACK;
					}
					if (w){
						w->color = RED;
						rotateLeft(w);
					}
					w = xParent->left;
				}
				if (w){
					w->color = xParent->color;
				}
				xParent->color = BLACK;
				if (w && w->left){
					w->left->color = BLACK;
				}
				rotateRight(xParent);
				x = _root;
				break;
			}
		}
	}
	if (x){
		x->color = BLACK;
	}
}

void BinarySearchTree::erase(const Key &key){
	if (!_root){
		return;
	}
	if (key == std::numeric_limits<Key>::max()){
		return;
	}
	Node* sent = detachSentinel();

	while (_root){
		Node* z = _root;
		while (z){
			if (key < z->keyValuePair.first){
				z = z->left;
			}
			else if (key > z->keyValuePair.first){
				z = z->right;
			}
			else{
				break;
			}
		}
		if (!z){
			break;
		}

		Node* y = z;
		bool yOriginalColor = y->color;
		Node* x = nullptr;
		Node* xParent = nullptr;

		if (!z->left){
			x = z->right;
			xParent = z->parent;
			transplant(z, z->right);
		}
		else if (!z->right){
			x = z->left;
			xParent = z->parent;
			transplant(z, z->left);
		}
		else{
			y = minNode(z->right);
			yOriginalColor = y->color;
			x = y->right;
			if (y->parent == z){
				xParent = y;
			}
			else{
				xParent = y->parent;
				transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}

		delete z;
		--_size;

		if (yOriginalColor == BLACK && _root){
			eraseFixup(x, xParent);
		}
	}

	if (_root){
		attachSentinel(sent);
	}
	else{
		delete sent;
	}
}

size_t BinarySearchTree::size() const{
	return _size;
}

void BinarySearchTree::output_tree() {
	if (!_root){
		return;
	}
	_root->output_node_tree();
}

size_t BinarySearchTree::heightImpl(Node* node) const{
	if (!node){
		return 0;
	}
	if (node->keyValuePair.first == std::numeric_limits<Key>::max()){
		return 0;
	}
	size_t lh = heightImpl(node->left);
	size_t rh = heightImpl(node->right);
	return 1 + std::max(lh, rh);
}

size_t BinarySearchTree::max_height() const{
	return heightImpl(_root);
}
