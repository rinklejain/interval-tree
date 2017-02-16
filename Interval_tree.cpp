#include <bits/stdc++.h>
using namespace std;

struct interval{
	int start, end;
};

struct Node{
	interval *i;
	int max;
	Node *left, *right, *parent;
};

interval *newinterval(int start, int end){
	interval *i = new interval();
	i->start = start;
	i->end = end;
	return i;
}

class intervalTree{
private :
	Node *root;

public:
	intervalTree(interval *i){
		root = new Node();
		root->i = i;
		root->max = max(i->start, i->end);
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;
	}
	
	Node* newNode(interval *i){
		Node *node = new Node();
		node->i = i;
		node->max = max(i->start, i->end);
		node->left = NULL;
		node->right = NULL;
		return node;
	}

	void insert(interval *i){
		Node *currentNode = newNode(i);
		insert(root, currentNode);
	}

	void insert(Node *root, Node *currentNode){
		// If tree is empty
		if (!this->root){
			this->root = currentNode;
			this->root->parent = NULL;
		}else{
			root->max = max(root->max, currentNode->max);     //update max since it is going to be changed after insertion of node
			// Case when we go to left for insertion
			if (currentNode->i->start < root->i->start){
				// if left node is NULL, no need to recurse just insert it
				if (!root->left){
					root->left = currentNode;
					currentNode->parent = root;
				}else{
					insert(root->left, currentNode);
				}
			}else{// Case when we go to right for insertion
				// if right node is NULL, no need to recurse just insert it
				if(!root->right){
					root->right = currentNode;
					currentNode->parent = root;
				}else{
					insert(root->right, currentNode);
				}
			}
		}
	}

	bool doMatch(interval *a, interval *b){
		if (a->start == b->start && a->end == b->end)
			return true;
		return false;
	}

	void deleteNode(interval *i){
		deleteNode(root, i);
	}

	bool isLeftChild(Node *t){
		if (t->parent->left == t)
			return true;
		return false;
	}

	void deleteNode(Node *root, interval *i){
		if(!root){
			cout<<"No such interval exists"<<endl;
			return;
		}

		if (doMatch(root->i, i)){
			//Node *currentParent = root->parent;
			if (root->right){//if match is FOUND and we can replace some value from right subtree
				Node *ReplaceBy = root;
				Node *temp = root->right;
				while(temp){
					ReplaceBy = temp;
					temp = temp->left;
				}
				//delete ReplaceBy
				if (isLeftChild(ReplaceBy)){
					ReplaceBy->parent->left = ReplaceBy->right;
					if (ReplaceBy->right) ReplaceBy->right->parent = ReplaceBy->parent;
				}else{
					ReplaceBy->parent->right = ReplaceBy->right;
					if (ReplaceBy->right) ReplaceBy->right->parent = ReplaceBy->parent;
				} 
				// updating max till root from ReplaceBy
				temp = ReplaceBy->parent;
				while(temp!=root){
					if (temp->left) temp->max = root->left->max;
					if (temp->right) temp->max = max(root->max, root->right->max);
					temp = temp->parent;
				}
				//replacing MATCh with ReplaceBy
				root->i = ReplaceBy->i;
				root->max = ReplaceBy->max;
				if (root->left) root->max = max(root->max, root->left->max);
				if (root->right) root->max = max(root->max, root->right->max);
			}else{//if right subtree is empty
				if (isLeftChild(root)){
					root->parent->left = root->left;
					if (root->left) root->left->parent = root->parent;
				}else{
					root->parent->right = root->left;
					if (root->left) root->left->parent = root->parent;
				} 
			}

			//Updating max till rootOfTree starting from the MATCH's parent 
			Node *temp = root->parent;
			while(temp != NULL){
				temp->max = temp->i->end;
				if (temp->left) temp->max = max(temp->left->max, temp->max);
				if (temp->right) temp->max = max(temp->right->max, temp->max);
				temp = temp->parent;
			}

		}else{
			if (i->start < root->i->start)
				deleteNode(root->left, i);
			else deleteNode(root->right, i);
		}

	}

	bool doOverlap(interval *a, interval *b){
		if (a->end < b->start || a->start > b->end)
			return false;
		return true;
	}

	interval* searchOverlap(interval *i){
		return searchOverlap(root, i);
	}

	interval* searchOverlap(Node *root, interval *i){
		if (!root)
			return NULL;

		if (doOverlap(root->i, i))
			return root->i;

		if (root->left && i->start <= root->left->max)
			return searchOverlap(root->left, i);
		else return searchOverlap(root->right, i);
	}

	void traverseTree(){
		traverseTree(root);
	}

	void traverseTree(Node *root){
		if (!root)
			return;
		traverseTree(root->left);
		cout<<"("<<root->i->start<<","<<root->i->end<<",["<<root->max<<"]"<<")"<<"  ";
		traverseTree(root->right);
	}
	
};

int main(){
	intervalTree IT(newinterval(15,20));
	interval ints[] = {{15,20}, {10, 30}, {17, 19},
        {5, 20}, {12, 15}, {16, 40}
    };
    int n = sizeof(ints)/sizeof(ints[0]);
    
    for (int i = 0; i < n; i++)
		IT.insert(newinterval(ints[i].start, ints[i].end));
	//test case 1
	interval *match = IT.searchOverlap(newinterval(14, 19));
	if(match)
		cout<<"Overlaps with ("<<match->start<<","<<match->end<<")"<<endl;
	else cout<<"No Overlap";
	//test case 2
	match = IT.searchOverlap(newinterval(21, 23));
	if(match)
		cout<<"Overlaps with ("<<match->start<<","<<match->end<<")"<<endl;
	else cout<<"No Overlap";	

	return 0;
}