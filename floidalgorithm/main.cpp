#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>
using namespace std;
using ValueType = int;

struct Node
{
public:
        ValueType data = 0;
        Node* next = nullptr;
        Node(ValueType value = 0): data(value), next(nullptr){}
};

pair<Node*, int> findcycle(Node* head){
	int len = 0;
	pair<Node*, int> p = {nullptr, 0};
	if (head == nullptr){
		return p;
	}
	Node* turtle = head;
	Node* rabbit = head;
	while ((rabbit!=nullptr)&&(rabbit->next!=nullptr)){
		turtle = turtle->next;
		rabbit = rabbit->next->next;
		if (turtle == rabbit){
			break;
		}
	}
	if (!rabbit || !rabbit->next) return p;
	turtle = head;
	while (rabbit != turtle){
		turtle = turtle->next;
                rabbit = rabbit->next;
	}
	rabbit = rabbit->next;
	len+=1;
	while (rabbit != turtle){
                rabbit = rabbit->next;
		len++;
	}
	p.first = turtle;
	p.second = len;
	return p;
}

class TheSpisok
{
private:
	Node* first = nullptr;
	Node* last = nullptr;
	int theSize = 0;
public:
	TheSpisok():first(nullptr), last(nullptr){}
	~TheSpisok(){
		Node* searchNode = first;      
            while(searchNode!=nullptr){
                Node* next = searchNode->next;
                delete searchNode;
                searchNode = next;
            }
	}
	void push_back(ValueType val){
		Node* newNode = new Node(val);
		if (first != nullptr){
			theSize+=1;
			last->next=newNode;
			last=newNode;
		}
		else{
			first=last=newNode;
			theSize+=1;
		}
	}
	void pop_back(){
		if (first==last){
			delete first;
			first = last = nullptr;
			theSize = 0;
		}
		else if (first != nullptr){
			theSize-=1;
			Node* searchNode = first;
			while(searchNode->next!=last){
				searchNode = searchNode->next;
			}
			delete last;
			last = searchNode;
			last->next = nullptr;
		}
		else{
			return;
		}
	}
	int size(){
		return theSize;
	}
	int find(ValueType value){
		int tempsize = 1;
		Node* searchNode = first;
		while(searchNode->data!=value||searchNode!=last){
			searchNode = searchNode->next;
			tempsize+=1;
		}
		if (searchNode==last && searchNode->data!=value){
			return -1;
		}
		return tempsize;
	}
};

int main() {
    std::cout << "=== BIG CYCLE TEST ===\n";
    Node* n1 = new Node(1);
    Node* n2 = new Node(2);
    Node* n3 = new Node(3);
    Node* n4 = new Node(4);
    Node* n5 = new Node(5);
    Node* n6 = new Node(6);
    Node* n7 = new Node(7);
    Node* n8 = new Node(8);
    Node* n9 = new Node(9);
    Node* n10 = new Node(10);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n6;
    n6->next = n7;
    n7->next = n8;
    n8->next = n9;
    n9->next = n10;
    n10->next = n7;
    Node* start = findcycle(n1).first;
    int length = findcycle(n1).second;
    if (start) {
        cout << "Cycle entry value: " << start->data << "\n"<<"Cycle length: "<< length<< "\n";
    } else {
        std::cout << "No cycle\n";
    }
    return 0;
}

