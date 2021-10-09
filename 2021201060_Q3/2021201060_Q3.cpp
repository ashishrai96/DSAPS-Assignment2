#include <iostream>

template <typename T>
class Deque
{

private:
    T *arr = nullptr;
    size_t length, max_size;
    long long f, r;
    // const size_t init_size {2305843009213693951};
    const size_t init_size {5};
public:
    Deque();
    Deque(size_t n, T x);
    ~Deque() {
        if(arr != nullptr)
            delete arr;
    }

    void push_back(T x);
    void pop_back();
    void push_front(T x);
    void pop_front();
    T front();
    T back();
    bool empty();
    size_t size();
    void resize(size_t x, T d);
    void clear();

    T &operator[](long long n);

    //  Helper Methods
    bool isFull();
    bool isOutOfBound(long long i);
    void increase_twice_max_size(size_t x);

    void display();
};

template <typename T>
Deque<T>::Deque() 
    : length{0}, f{-1}, r{-1}
    {
        arr = new T[init_size];
        max_size = init_size;
    }

template <typename T>
Deque<T>::Deque(size_t n, T x) 
    : length{n}, f{0}, r{static_cast<long long>(n-1)}
    {
        max_size = n+init_size;
        arr = new T[max_size];
        for(size_t i{0}; i<n; i++)
            arr[i] = x;
    }


//  Helper Methods

template <typename T>
bool Deque<T>::isFull() {
    return length == max_size;
}

template <typename T>
bool Deque<T>::isOutOfBound(long long i) {

}

template <typename T>
void Deque<T>::increase_twice_max_size(size_t x){
    T *p = arr;
    arr = new T[(x+x)];
    max_size = (x+x);

    for(size_t j{0}; j<length; j++){
        arr[j] = p[(f+j) % max_size];
    }

    f = 0;
    r = length-1;
    delete p;
}


//  Required Methods

template <typename T>
void Deque<T>::push_front(T x) {
    if(length < max_size){
        // Deque is not full
        if(f == -1){
            // Deque is Empty
            f = r = 0;
        }
        else if(f == 0){
            f = max_size-1;
        }
        else{
            f--;
        }

        this->arr[f] = x;
    }
    else{
        // Deque Overflow - resize
        increase_twice_max_size(max_size);
        if(f == 0){
            f = max_size-1;
        }
        else{
            f--;
        }

        this->arr[f] = x;

    }

    length++;
}

template <typename T>
void Deque<T>::pop_front() {
    if(length == 0){
        // Deque underflow
    }
    else{
        f = (f+1) % max_size;
        length--;

        if(length == 0){
            f = r = -1;
        }
    }
}

template <typename T>
void Deque<T>::push_back(T x) {
    if(length < max_size){
        // Deque is not full
        if(r == -1){
            // Deque is Empty
            f = r = 0;
        }
        else if(r == max_size-1){
            r = 0;
        }
        else{
            r++;
        }

        this->arr[r] = x;
    }
    else{
        // Deque Overflow - resize
        increase_twice_max_size(max_size);
        if(r == max_size-1){
            r = 0;
        }
        else{
            r++;
        }

        this->arr[r] = x;
    }

    length++;
}

template <typename T>
void Deque<T>::pop_back() {
    if(length == 0){
        // Deque underflow
    }
    else{
        if(r == 0){
            r = max_size;
        }
        else{
            r = (r-1) % max_size;
        }
        length--;

        if(length == 0){
            r = f = -1;
        }
    }
}

template <typename T>
T Deque<T>::front() {
    if(length > 0){
        return this->arr[f];
    }
    else{
        throw "Underflow";
    }
}

template <typename T>
T Deque<T>::back() {
    if(length > 0){
        return this->arr[r];
    }
    else{
        throw "Underflow";
    }
}

template <typename T>
bool Deque<T>:: empty() {
    return length == 0;
}

template <typename T>
size_t Deque<T>::size() {
    return this->length;
}

template <typename T>
void Deque<T>::resize(size_t x, T d) {
    if(max_size < x){
        // T *p {arr};
        // arr = new T[(x+x)];
        // max_size = (x+x);

        // for(size_t j{0}; j<length; j++){
        //     arr[j] = p[(f+j) % max_size];
        // }

        increase_twice_max_size(x);

        size_t j{length};
        while(j<x){
            arr[j] = d;
            j++;
        }

        r = x-1;
        length = x;
        // delete p;
    }
    else if(length < x){
        while(length != x){
            this->push_back(d);
        }
    }
    else{
        length = x;
        r = x-1;
    }
}

template <typename T>
void Deque<T>::clear() {
    f = r = -1;
    length = 0;
}

template <typename T>
T &Deque<T>::operator[](long long n){
    if(n < length){
        return arr[(f+n)%max_size];
    }
    else{
        throw "Index out of bound!";
    }
}

template <typename T>
void Deque<T>::display(){
    std::cout << "---------------------------------" << std::endl;
    std::cout << "length = " << length << " | max_size = " << max_size << std::endl;
    std::cout << "=================================" << std::endl;
    long long p {0};
    while(p < length){
        std::cout << arr[((p+f) % max_size)] << " ";
        p++;
    }
    // long long q {0};
    // while(q < deq.length){
    //     std::cout << deq[q] << " ";
    // }
    std::cout << std::endl << "---------------------------------" << std::endl;
}

void displayMenu() {
    std::cout << "\n" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Press 1 for PUSH BACK " << std::endl;
    std::cout << "Press 2 for PUSH FRONT " << std::endl;
    std::cout << "Press 3 for POP FRONT " << std::endl;
    std::cout << "Press 4 for POP BACK" << std::endl;
    std::cout << "Press 5 for FRONT" << std::endl;
    std::cout << "Press 6 for BACK" << std::endl;
    std::cout << "Press 7 for IS-EMPTY" << std::endl;
    std::cout << "Press 8 for SIZE" << std::endl;
    std::cout << "Press 9 for CLEAR" << std::endl;
    std::cout << "Press 10 for RESIZE" << std::endl;
    std::cout << "Press 11 for Nth - INDEX" << std::endl;
    std::cout << "Press 12 for DISPLAY" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "\n" << std::endl;
}

int main(){

    Deque<int> deq;

    int optn{};
    bool cont {true};
    int x;
    do{
        displayMenu();
        std::cin >> optn;
        switch (optn)
        {
        case 1:
            std::cout << "push back => ";
            std::cin >> x;
            deq.push_back(x);
            break;
        case 2:
            std::cout << "push front => ";
            std::cin >> x;
            deq.push_front(x);
            break;
        case 3:
            std::cout << "pop front => ";
            deq.pop_front();
            break;
        case 4:
            std::cout << "pop back => ";
            deq.pop_back();
            break;
        case 5:
            std::cout << "front elem => " << deq.front();
            break;
        case 6:
            std::cout << "Back back => " << deq.back();
            break;
        case 7:
            std::cout << "Is empty => " << deq.empty();
            break;
        case 8:
            std::cout << "Deque Size => " << deq.size();
            break;
        case 9:
            deq.clear();
            std::cout << "Deque Cleared" << std::endl;
            break;
        case 10:
            long long s;   
            std::cout << "Enter New size => ";
            std::cin >> s;
            deq.resize(s,0);
            break;            
        case 11:
            long long n;
            std::cout << "Enter index => ";
            std::cin >> n;
            std::cout << deq[n] << std::endl;
            break;
        case 12:
            std::cout << "\n" << std::endl;
            deq.display();
            std::cout << "\n" << std::endl;
            break;
        default:
            cont = false;
            break;
        }
    }while(cont);

    return 0;
}