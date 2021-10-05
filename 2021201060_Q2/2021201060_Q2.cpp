#include <iostream>
#include <sstream>
#include <vector>

template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node *next;
};

template <typename K, typename V>
class UnorderedMap
{

friend std::ostream &operator<<(std::ostream &os, const UnorderedMap<K,V> &umap);

private:
    // const long prime {7321};
    const long prime {10};
    const long starting_char {32};  // For SPACE
    const long csize {90};  //  32-SPACE to 122-z (ASCII values)
    
    Node<K,V> **arr = nullptr;
    long size;
public:
    UnorderedMap();
    ~UnorderedMap();

    void insert(K key, V value);
    void erase(K key);
    bool find(K key);
    V &operator[](K key);

    void clear();
    long hasher(std::string key);
    void display();
};

template <typename K, typename V>
UnorderedMap<K, V>::UnorderedMap()
    : size {0}
    {
        arr = new Node<K,V>*[prime];
    }

template <typename K, typename V>
UnorderedMap<K, V>::~UnorderedMap()
{
    delete arr;
}

template <typename K, typename V>
long UnorderedMap<K,V>::hasher(std::string key){
    // Karp-Rabin approach + Chaining
    long sum {0};
    size_t len {key.size()};
    size_t i{0};
    do{
        char ch = key.at(i);
        sum *= csize % prime;
        // sum += (ch - starting_char) % prime;
        sum += (ch - '0') % prime;
        i++;
    }while(i < len);

    return sum % prime;
}

template <typename K, typename V>
void UnorderedMap<K,V>::insert(K key, V value) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = arr[index];
    if(p != nullptr){
        while(p->key != key && p->next != nullptr){
            p = p->next;
        }
        if(p->key == key){
            p->value = value;
        }
        else{
            Node<K,V> *node = new Node<K,V>;
            node->key = key;
            node->value = value;
            node->next = nullptr;
            p->next = node;
            size++;
        }
    }
    else{
        Node<K,V> *node = new Node<K,V>;
        node->key = key;
        node->value = value;
        node->next = nullptr;
        arr[index] =  node;
        size++;
    }
}

template <typename K, typename V>
void UnorderedMap<K,V>::erase(K key) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = arr[index], *q = nullptr, *temp;
    if(p != nullptr){
        while(p && p->key != key){
            q = p;
            p = p->next;
        }

        if(p && p->key == key){
            temp = p;
            if(q == nullptr){
                //  First element to be deleted!
                arr[index] = p->next;
            }
            else{
                q->next = p->next;
            }

            delete temp;
            size--;
        }
    }
}

template <typename K, typename V>
void UnorderedMap<K,V>::clear() {
    for(long i{0}; i<prime; i++){
        if(arr[i] != nullptr){
            Node<K,V> *p = arr[i], *tmp;
            arr[i] = nullptr;

            while(p!=nullptr){
                tmp = p;
                p = p->next;
                delete tmp;
            }
        }
    }

    size = 0;
}

template <typename K, typename V>
bool UnorderedMap<K,V>::find(K key) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = arr[index];
    if(p != nullptr){
        while(p && p->key != key){
            p = p->next;
        }

        if(p && p->key == key){
            return true;
        }
    }

    return false;
}

template <typename K, typename V>
V &UnorderedMap<K,V>::operator[](K key) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = arr[index];
    if(p != nullptr){
        while(p && p->key != key){
            p = p->next;
        }

        if(p && p->key == key){
            return p->value;
        }
    }
    throw "Invalid Key!";
}

template <typename K>
std::vector<int> distinceElemInSubarray(size_t k, size_t n, K *array) {
    std::vector<int> res{};
    UnorderedMap<K,int> um;

    size_t distinct{0};
    size_t i{0};
    while(i<k && i<n){
        if(um.find(array[i])){
            um.insert(array[i], um[array[i]]+1);
        }
        else{
            um.insert(array[i], 1);
            distinct++;
        }
        i++;
    }

    res.push_back(distinct);
    while(i<n){
        K tmpKey = array[i-k];
        if(um[tmpKey] > 1){
            um.insert(tmpKey, um[tmpKey]-1);
        }
        else{
            um.erase(tmpKey);
            distinct--;
        }

        if(um.find(array[i])){
            um.insert(array[i], um[array[i]]+1);
        }
        else{
            um.insert(array[i], 1);
            distinct++;
        }
        res.push_back(distinct);
        i++;
    }

    return res;
}

template <typename K, typename V>
void UnorderedMap<K,V>::display() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Size = " << size << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - -" << std::endl;
    for(long i{0}; i<prime; i++){
        if(arr[i] != nullptr){
            Node<K,V> *p = arr[i];
            while(p!=nullptr){
                std::cout << "Key: " << p->key << " | Value: " << p->value << std::endl;
                p = p->next;
            }
        }
    }
    std::cout << "-----------------------------------" << std::endl;
}

int main() {

    UnorderedMap<int,int> um;
    size_t n;
    std::cout << "Enter the size of an Array: ";
    std::cin >> n;
    
    std::cout << "Enter the Array elements: ";
    int array[] = { 1,2,1,3,4,2,3 };
    for(size_t i{0}; i<n; i++){
        std::cin >> array[i];
    }

    size_t k;
    std::cout << "Enter the size of subarray (k): ";
    std::cin >> k;

    std::vector<int> vec {};
    vec = distinceElemInSubarray(k, n,array);
    for(auto &a : vec){
        std::cout << a << " ";
    }std::cout << std::endl;


    // um.insert(1,10);
    // um.insert(33,30);
    // um.insert(22,20);
    // um.insert(222,200);
    // um.insert(11,100);
    // um.insert(111,1000);

    // um.display();

    // um.insert(1,10000);

    // um.display();

    // std::cout << um.find(222) << std::endl;
    // um.erase(222);
    // std::cout << um.find(222) << std::endl;

    // std::cout << um[33] << std::endl;

    // um.display();

    // um.display();
    return 0;
}