#include <bits/stdc++.h>
using namespace std;


struct Interval
{
    int low, high;
};

struct Node
{
    Interval i; 
    int max;
    Node *left, *right;
};

Node * newNode(Interval i)
{
    Node *temp = new Node;
    temp->i = i;
    temp->max = i.high;
    temp->left = temp->right = NULL;
};

Node *insert(Node *root, Interval i)
{
    if (root == NULL)
        return newNode(i);
    int l = root->i.low;
    if (i.low < l)
        root->left = insert(root->left, i);
    else
        root->right = insert(root->right, i);
    if (root->max < i.high)
        root->max = i.high;
 
    return root;
}

Node * minValueNode(Node* node)
{
    Node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

Node *deleteNode(Node *root, Interval k)
{
    
    if (root == NULL) return root;
    if (k.low < root->i.low)
        root->left = deleteNode(root->left, k);
    else if (k.low > root->i.low)
        root->right = deleteNode(root->right, k);
    else
    {
        if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = minValueNode(root->right);
        root->i = temp->i;
 
        root->right = deleteNode(root->right, temp->i);
    }
    return root;
}

bool doOVerlap(Interval i1, Interval i2)
{
    if (i1.low <= i2.high && i2.low <= i1.high)
        return true;
    return false;
}
 
// The main function that searches a given interval i in a given
// Interval Tree.
Interval overlapSearch(Node *root, Interval i)
{
    Interval x;
    x.low=-1;
    if (root == NULL) return x;
 
    // If given interval overlaps with root
    if (doOVerlap((root->i), i))
        return root->i;
 
    // If left child of root is present and max of left child is
    // greater than or equal to given interval, then i may
    // overlap with an interval is left subtree
    if (root->left != NULL && root->left->max >= i.low)
        return overlapSearch(root->left, i);
 
    // Else interval can only overlap with right subtree
    return overlapSearch(root->right, i);
}
int main()
{
    
    int n;
    cout<<"enter number of intervals in the tree"<<endl;
    cin>>n;
    cout<<"enter the intervals"<<endl;;
    Interval in;
    Node *root = NULL;
    for (int i = 0; i < n; i++)
    {
    	cin>>in.low>>in.high;
        root = insert(root, in);
    }
    Interval x;
    cout<<"enter the interval to search"<<endl;
    cin>>x.low>>x.high;
       Interval res = overlapSearch(root, x);
    if (res.low == -1)
        cout << "\nNo Overlapping Interval"<<endl;
    else
        cout << "\nOverlaps with [" << res.low << ", " << res.high << "]"<<endl;
     cout<<"enter the interval to delete"<<endl;
	 cin>>in.low>>in.high;   
	 root=deleteNode(root,in);
	 cout<<"enter the interval to search"<<endl;
    cin>>x.low>>x.high;
       res = overlapSearch(root, x);
    if (res.low == -1)
        cout << "\nNo Overlapping Interval"<<endl;
    else
        cout << "\nOverlaps with [" << res.low << ", " << res.high << "]"<<endl;
    return 0;
}
