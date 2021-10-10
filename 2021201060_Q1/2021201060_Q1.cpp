#include <iostream>

template <typename T>
struct Node
{
    T val;
    Node *left {nullptr}, *right {nullptr};
    long height {0};
    long count {1};
    long l_sub_count, r_sub_count;
};

template <typename T>
class AVLTree
{
private:
public:
    Node<T> *root {nullptr};

    AVLTree();
    ~AVLTree();

    Node<T> *insertNode(Node<T> *root, Node<T> *node);
    Node<T> *deleteNode(Node<T> *root, T val);
    Node<T> *searchNode(Node<T> *root, T val);
    Node<T> *lowerBound(Node<T> *root, T val);
    Node<T> *upperBound(Node<T> *root, T val);
    Node<T> *closestValue(Node<T> *root, T val);
    Node<T> *kthLargest(Node<T> *root, int k);
    int nodesInRange(Node<T> *root, T l, T u);

    Node<T> *leftRotation(Node<T> *node);
    Node<T> *rightRotation(Node<T> *node);

    // Helper Fn
    Node<T> *getMinimunNode(Node<T> *node);
    int getBalanceFactor(Node<T> *node);

};

template <typename T>
AVLTree<T>::AVLTree()
{
}

template <typename T>
AVLTree<T>::~AVLTree()
{
}

template <typename T>
int AVLTree<T>::getBalanceFactor(Node<T> *node) {
    if(node == nullptr)
        return -1;
    return ((node->left == nullptr)? -1 : node->left->height) - ((node->right == nullptr)? -1 : node->right->height);
}

template <typename T>
void updateHeight(Node<T> *root){
    if(root->left && root->right){
        root->height = ((root->left->height > root->right->height)? root->left->height : root->right->height) + 1;
    }
    else if(root->left){
        root->height = root->left->height + 1;
    }
    else if(root->right){
        root->height = root->right->height + 1;
    }
    else{
        root->height = 0;
    }
}

template <typename T>
Node<T>* AVLTree<T>::getMinimunNode(Node<T> *node) {
    // Finding node with Minimun Value
    if(node->left == nullptr){
        return node;
    }

    return getMinimunNode(node->left);
}

template <typename T>
Node<T>* AVLTree<T>::leftRotation(Node<T> *node) {
    Node<T>  *p = node->right;
    Node<T> *q = p->left;

    node->right = q;
    p->left = node;

    node->r_sub_count = (q!=nullptr)? q->l_sub_count + q->r_sub_count + q->count : 0;
    p->l_sub_count = node->l_sub_count + node->r_sub_count + node->count;
    updateHeight(p->left);

    return p;
}

template <typename T>
Node<T>* AVLTree<T>::rightRotation(Node<T> *node) {
    Node<T> *p = node->left;
    Node<T> *q = p->right;

    node->left = q;
    p->right = node;

    node->l_sub_count = (q!=nullptr)? q->l_sub_count + q->r_sub_count + q->count : 0;
    p->r_sub_count = node->l_sub_count + node->r_sub_count + node->count;
    updateHeight(p->right);

    return p;
}

template <typename T>
Node<T> *AVLTree<T>::insertNode(Node<T> *root, Node<T> *node) {
    if(root == nullptr){
        root = node;
        root->height = root->l_sub_count = root->r_sub_count = 0;
        return root;
    }
    else{
        if(node->val < root->val){
            root->left = insertNode(root->left, node);
            
            updateHeight(root);
            root->l_sub_count = root->left->l_sub_count + root->left->r_sub_count + root->left->count;

        }
        else if(node->val > root->val){
            root->right = insertNode(root->right, node);

            updateHeight(root);
            root->r_sub_count = root->right->l_sub_count + root->right->r_sub_count + root->right->count;
        }
        else{
            root->count++;
            return root;
        }
    }

    int bf = getBalanceFactor(root);
    if(bf > 1){
        if(root->left->val > node->val){        // L-L Imbalance
            return rightRotation(root);
        }
        else if(root->left->val < node->val){   // L-R Imbalance
            root->left = leftRotation(root->left);
            return rightRotation(root);
        }
    }
    else if(bf < -1){
        if(root->right->val < node->val) {      // R-R Imbalance
            return leftRotation(root);
        }
        else if(root->right->val > node->val){  // R-L Imbalance
            root->right = rightRotation(root->right);
            return leftRotation(root);
        }
    }

    return root;
}


template <typename T>
Node<T> *AVLTree<T>::deleteNode(Node<T> *root, T val) {
    if(root == nullptr) {
        return root;
    }
    else if(root->val > val) {      // Look in the Left sub tree
        root->left = deleteNode(root->left, val);
        updateHeight(root);
        root->l_sub_count = (root->left != nullptr)? (root->left->l_sub_count + root->left->r_sub_count + root->left->count) : 0;
    }
    else if(root->val < val) {      // Look in the Right sub tree
        root->right = deleteNode(root->right, val);
        updateHeight(root);
        root->r_sub_count = (root->right != nullptr)? (root->right->l_sub_count + root->right->r_sub_count + root->right->count) : 0;
    }
    else{
        // Value matches
        // if(root->count > 1){
        //     root->count--;
        //     return root;
        // }
        
        if(root->left == nullptr){  // Either No children or right child
            Node<T> *tmp = root->right;
            delete root;
            return tmp;
        }
        else if(root->right == nullptr){    // Only left child exists
            Node<T> *tmp = root->left;
            delete root;
            return tmp;
        }
        else{       // Both children exist
            Node<T> *tmp = getMinimunNode(root->right);
            root->val = tmp->val;
            root->count = tmp->count;
            root->right = deleteNode(root->right, tmp->val);
            updateHeight(root);
            root->r_sub_count = (root->right != nullptr)? (root->right->l_sub_count + root->right->r_sub_count + root->right->count) : 0;
        }
    }

    int bf = getBalanceFactor(root);
    if(bf == 2){                    // Left heavy
        int left_bf = getBalanceFactor(root->left);
        if(left_bf >= 0){           // L-L imbalance
            return rightRotation(root);
        }
        else if(left_bf == -1){     // L-R imbalance
            root->left = leftRotation(root->left);
            return rightRotation(root);
        }
    }
    else if(bf == -2){              // Right heavy
        int right_bf = getBalanceFactor(root->right);
        if(right_bf <= 0){          // R-R imbalance
            return leftRotation(root);
        }
        else if(right_bf == 1){    //
            root->right = rightRotation(root->right);
            return leftRotation(root);
        }
    }

    return root;
}

template <typename T>
Node<T> *AVLTree<T>::searchNode(Node<T> *root, T val) {
    if(root == nullptr){
        // Not found or Does not exist
        return root;
    }
    else if(root->val > val){
        // Value < Root - Must be in left sub tree
        return searchNode(root->left, val);
    }
    else if(root->val < val){
        // Value > Root - Must be in right sub tree
        return searchNode(root->right, val);
    }
    else{
        // Value found
        return root;
    }
}

template <typename T>
Node<T> *AVLTree<T>::lowerBound(Node<T> *root, T val) {
    Node<T> *p {root}, *q {nullptr};

    while(p != nullptr) {
        if(p->val >= val){
            q = p;
            p = p->left;
        }
        else{
            p = p->right;
        }
    }

    return q;
}

template <typename T>
Node<T> *AVLTree<T>::upperBound(Node<T> *root, T val) {
    Node<T> *p {root}, *q {nullptr};

    while(p != nullptr) {
        if(p->val > val){
            q = p;
            p = p->left;
        }
        else {
            p = p->right;
        }
    }

    return q;
}

template <typename T>
Node<T> *AVLTree<T>::closestValue(Node<T> *root, T val) {
    if(root == nullptr){
        return root;
    }

    Node<T> *p {root}, *q {root};

    T diff {p->val - val};
    if(diff < 0)
        diff *= -1;
    else if(diff == 0)
        return q;

    if(p->val > val)
        p = p->left;
    else
        p = p->right;

    while(p!=nullptr){
        T tmp = p->val - val;
        if(tmp < 0)
            tmp *= -1;
        
        if(tmp < diff || (tmp == diff && p->val < q->val)){
            diff = tmp;
            q = p;
        }

        if(diff == 0)
            return q;

        if(p->val > val)
            p = p->left;
        else
            p = p->right;
    }

    return q;
}

template <typename T>
Node<T> *AVLTree<T>::kthLargest(Node<T> *root, int k) {
    if(root == nullptr){
        return root;
    }

    if(root->r_sub_count < k){
        int tmp = k - root->r_sub_count - root->count;
        if(tmp <= 0){
            return root;
        }
        else{
            return kthLargest(root->left, tmp);
        }
    }

    return kthLargest(root->right, k);
}

template <typename T>
int AVLTree<T>::nodesInRange(Node<T> *root, T l, T u) {
    if(root == nullptr){
        return 0;
    }

    if(root->val >= l && root->val <= u){
        return root->count + nodesInRange(root->left, l, u) + nodesInRange(root->right, l, u);
    }

    if(root->val < l){
        return nodesInRange(root->right, l, u);
    }
    else{
        return nodesInRange(root->left, l, u);
    }
}


template <typename T>
void print2D(Node<T> *root, int space) {
    if (root == nullptr)
      return;
    
    space += 20;
    print2D(root->right, space);
    std::cout << std::endl;
    for (int i = 20; i < space; i++)
      std::cout << " ";
    std::cout 
        << root->val << "(" 
        << root->height << ", "
        << root->l_sub_count << ", "
        << root->r_sub_count << ", "
        << root->count
        << ")" << std::endl;
    print2D(root->left, space);
}


/**
 * Dummy Class for class evaluation
 * operator to overload '<' & '-' (for closesnt range)
*/

/***************************    Dummy class - START    ************************/
class Person
{
    friend bool operator<(const Person &p1, const Person &p2);
    friend bool operator>(const Person &p1, const Person &p2);
    friend bool operator<=(const Person &p1, const Person &p2);
    friend bool operator>=(const Person &p1, const Person &p2);
    friend bool operator==(const Person &p1, const Person &p2);

    friend std::ostream &operator<<(std::ostream &os, const Person &p);
    friend std::istream &operator>>(std::istream &is, const Person &p);
private:
    float height {0};
public:
    Person();
    Person(int i) {
        height = i;
    }
    ~Person() = default;
    void setHeight(float ht) { height = ht; }
};

Person::Person() {}

bool operator<(const Person &p1, const Person &p2) {
    return p1.height < p2.height;
}

bool operator>(const Person &p1, const Person &p2) {
    return p1.height > p2.height;
}

bool operator<=(const Person &p1, const Person &p2) {
    return p1.height <= p2.height;
}

bool operator>=(const Person &p1, const Person &p2) {
    return p1.height >= p2.height;
}

bool operator==(const Person &p1, const Person &p2) {
    return p1.height == p2.height;
}

std::ostream &operator<<(std::ostream &os, const Person &p) {
    os << "The person's height is " << p.height <<std::endl;
    return os;
}

std::istream &operator>>(std::istream &is, Person &p) {
    float ht;
    is >> ht;
    p.setHeight(ht);
    return is;
}

/***************************    Dummy class - END    ************************/


int main(){
    AVLTree<double> avl;
    double i, j;
    Node<double> *p;

    int optn {0};
    do{
        std::cout << "\n\nEnter 0 -> Exit" << std::endl;
        std::cout << "Enter 1 -> Display" << std::endl;
        std::cout << "Enter 2 -> Insertion" << std::endl;
        std::cout << "Enter 3 -> Deletion" << std::endl;
        std::cout << "Enter 4 -> Searching" << std::endl;
        std::cout << "Enter 5 -> Find an Element Occurrences" << std::endl;
        std::cout << "Enter 6 -> Find the Lower Bound" << std::endl;
        std::cout << "Enter 7 -> Find the Upper Bound" << std::endl;
        std::cout << "Enter 8 -> Find the Closest Value" << std::endl;
        std::cout << "Enter 9 -> Find the Kth Largest" << std::endl;
        std::cout << "Enter 10 -> Find the Nodes in range\n" << std::endl;
        std::cout << std::endl << "Enter option: ";
        std::cin >> optn;
        
        switch (optn)
        {
            case 1:
            {
                std::cout << std::endl;
                print2D(avl.root, 4);
                std::cout << std::endl;
            }
            break;

            case 2:
            {
                std::cout << "Enter node to insert: ";
                std::cin >> i;
                    p = new Node<double>;
                    p->val = i;
                    avl.root = avl.insertNode(avl.root, p);
            }
            break;

            case 3:
            {
                std::cout << "Enter node to delete: ";
                std::cin >> i;
                avl.root = avl.deleteNode(avl.root, i);
            }
            break;
            
            case 4:
            {
                std::cout << "Enter node to search: ";
                std::cin >> i;
                p = avl.searchNode(avl.root, i);
                if(p==nullptr){
                    std::cout << "\nDoes Not exist!" << std::endl;
                }
                else{
                    std::cout << "\n-------------Element details--------------" << std::endl;
                    std::cout << "Details of Node - " << p->val << std::endl;
                    std::cout << "Node's height = " << p->height << "\n"
                            << "Node's Left-children = " << p->l_sub_count << "\n"
                            << "Node's Right-children = " << p->r_sub_count << "\n"
                            << "Node's Occurrences = " << p->count << std::endl;
                    std::cout << "------------------------------------------" << std::endl;
                }
            }
            break;
            
            case 5:
            {
                std::cout << "Enter node to look for its occurrences: ";
                std::cin >> i;
                p = avl.searchNode(avl.root, i);
                if(p==nullptr){
                    std::cout << "\nDoes Not exist!" << std::endl;
                }
                else{
                    std::cout << "\nNode - " << p->val << " occurred " << p->count 
                            << ((p->count > 1)?" times":" time") << std::endl;
                }
            }
            break;

            case 6:
            {
                std::cout << "Enter node to look for its lower bound: ";
                std::cin >> i;
                p = avl.lowerBound(avl.root, i);
                if(p == nullptr)
                    std::cout << "\nLower Bound Does Not Exist" << std::endl;
                else
                    std::cout << "\nLower bound - Node - " << p->val << std::endl;
            }
            break;

            case 7:
            {
                std::cout << "Enter node to look for its Upper bound: ";
                std::cin >> i;
                p = avl.upperBound(avl.root, i);
                if(p == nullptr)
                    std::cout << "\nUpper Bound Does Not Exist" << std::endl;
                else
                    std::cout << "\nUpper bound - Node - " << p->val << std::endl;
            }
            break;
            
            case 8:
            {
                std::cout << "Enter node to look for its closest value: ";
                std::cin >> i;
                p = avl.closestValue(avl.root, i);
                std::cout << "\nClosest value - Node - " << p->val << std::endl;
            }
            break;
            
            case 9:
            {
                int i;
                std::cout << "Enter node to look for its kth Largest: ";
                std::cin >> i;
                p = avl.kthLargest(avl.root, i);
                if(p == nullptr)
                    std::cout << "\nDoesnot exist!!" << std::endl;
                else
                    std::cout << "\nKth largest - Node - " << p->val << std::endl;
            }
            break;
            
            case 10:
            {
                std::cout << "Enter range to look for #elements within: ";
                std::cin >> i >> j;
                int count = avl.nodesInRange(avl.root, i,j);
                std::cout << "\nNumber of nodes - " << count << std::endl;
            }
            break;
        }
    }while(optn != 0);

    return 0;
}