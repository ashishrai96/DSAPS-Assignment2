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
    const long prime = 7321;  //  Prime Number
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

    Node<K,V> *p = nullptr;
    p = arr[index];
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
        else{
            // Key doesn't exist in the chain
            insert(key, {});
            return operator[](key);
        }
    }
    else{
        // Key doesn't exist
        insert(key, {});   
        return operator[](key);
    }
}

template <typename K>
std::vector<int> distinceElemInSubarray(size_t k, size_t n, K *array) {
    if(k > n){
        std::cout << "\nERROR: size of subarray is more that array size." << std::endl;
        return {};
    }

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

void displayMenu() {
    std::cout << "\n" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Press 0 for EXIT " << std::endl;
    std::cout << "Press 1 for INSERT " << std::endl;
    std::cout << "Press 2 for ERASE " << std::endl;
    std::cout << "Press 3 for FIND " << std::endl;
    std::cout << "Press 4 for KEY's VALUE" << std::endl;
    std::cout << "Press 5 for DISTINCT ELEMENT COUNT" << std::endl;
    std::cout << "Press 6 for DISPLAY" << std::endl;
    std::cout << "Press 7 for CLEAR" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "\n" << std::endl;
}


int main() {

    UnorderedMap<std::string, double> um;
    std::string k;
    double v;

    int optn;
    do{
        displayMenu();
        std::cout << "\nEnter menu option: ";
        std::cin >> optn;

        switch (optn)
        {
            case 1:
            {
                std::cout << "\nEnter key-value pair: ";
                std::cin >> k >> v;
                um.insert(k,v);
            }
            break;
            case 2:
            {
                std::cout << "\nEnter key to erase: ";
                std::cin >> k;
                um.erase(k);
            }
            break;
            case 3:
            {
                std::cout << "\nEnter key to find: ";
                std::cin >> k;
                std::cout << "Does element exists? - " << (um.find(k)?"YES":"NO") << std::endl;
            }
            break;
            case 4:
                {
                    std::cout << "\nEnter key to get the value: ";
                    std::cin >> k;
                    std::cout << "\n" << um[k] << std::endl;
                }
            break;
            case 5:
                {
                    size_t n;
                    std::cout << "\nEnter the size of an Array: ";
                    std::cin >> n;
                    
                    std::cout << "Enter the Array elements: ";
                    // Depends on Map Datatype.
                    std::string array[n];
                    for(size_t i{0}; i<n; i++){
                        std::cin >> array[i];
                    }

                    size_t k;
                    std::cout << "Enter the size of subarray (k): ";
                    std::cin >> k;

                    std::vector<int> vec {};
                    vec = distinceElemInSubarray(k, n,array);
                    std::cout << "\nResult Set:" << std::endl;
                    for(auto &a : vec){
                        std::cout << a << " ";
                    }std::cout << std::endl;
                }
            break;
            case 6:
                {
                    um.display();
                }
            break;
            case 7:
                {
                    um.clear();
                    std::cout << "\nCleared!!" << std::endl;
                }
            break;
        }
    }while(optn != 0);

    return 0;
}