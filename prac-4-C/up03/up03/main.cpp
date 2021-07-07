#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>

#include <ctime>
#include <cstdlib>



namespace numbers
{
    class complex
    {
        double real, imag;
    public:
        
        complex(double _re=0.0, double _im=0.0): real(_re), imag(_im) {}
        
        explicit complex(std::string s): real(0.0), imag(0.0) {
            sscanf(s.c_str(), "(%lf,%lf)", &real, &imag);
        }
        
        double re() const {
            return real;
        }
        
        double im() const {
            return imag;
        }
        
        double abs2() const {
            return real * real + imag * imag;
        }
        
        double abs() const {
            return sqrt(abs2());
        }
        
        /*std::string to_string() const {
            std::ostringstream streamObj;
            streamObj << std::setprecision(10);
            streamObj << "(" << real << "," << imag << ")";
            return streamObj.str();
        }*/
        
        std::string to_string() const {
            int maxlen = 100000;
            char stform[maxlen];
            snprintf(stform, maxlen, "(%.10g,%.10g)", real, imag);
            std::string s(stform);
            return s;
        }
        
        const complex& operator+=(const complex& z) {
            real += z.re();
            imag += z.im();
            return *this;
        }
        
        const complex& operator-=(const complex& z) {
            real -= z.re();
            imag -= z.im();
            return *this;
        }
        
        const complex& operator*=(const complex& z) {
            double tmp_re = re() * z.re() - im() * z.im();
            imag = re() * z.im() + im() * z.re();
            real = tmp_re;
            return *this;
        }
        
        const complex& operator/=(const complex& z) {
            double div = z.abs2();
            double tmp_re = (re() * z.re() + im() * z.im()) / div;
            imag = (im() * z.re() - re() * z.im()) / div;
            real = tmp_re;
            return *this;
        }
        
        const complex operator-() const {
            complex tmp = complex(-re(), -im());
            return tmp;
        }
        
        const complex operator~() const {
            complex tmp = complex(re(), -im());
            return tmp;
        }
    };

    class complex_stack
    {
        size_t size_;
        complex *arr;
    public:
        complex_stack(): size_(0) {}
        
        explicit complex_stack(complex_stack &CSC) {
            size_ = CSC.size();
            arr = new complex[size_];
            if (size_ > 0) {
                for (int i = 0; i < size_; i++){
                    arr[i] = CSC[i];
                }
            }
        }
        
        explicit complex_stack(complex_stack &CSC, complex &newelem) {
            size_ = CSC.size();
            arr = new complex[size_];
            if (size_ > 0) {
                for (int i = 0; i < size_; i++){
                    arr[i] = CSC[i];
                }
            }
        }

        ~complex_stack() {
            delete[] arr;
        }
        
        size_t size() const {
            return size_;
        }
        
        const complex_stack& operator<<(const complex& z) {
            complex_stack newstack = complex_stack(*this, z);
            return newstack;
        }
        
        const complex& operator[](int i) {
            return arr[i];
        }
        
        
        
        
    };

    complex operator+(const complex& z1, const complex& z2){
        complex tmp = complex(z1.re(), z1.im());
        tmp += z2;
        return tmp;
    }

    complex operator-(const complex& z1, const complex& z2){
        complex tmp = complex(z1.re(), z1.im());
        tmp -= z2;
        return tmp;
    }

    complex operator*(const complex& z1, const complex& z2){
        complex tmp = complex(z1.re(), z1.im());
        tmp *= z2;
        return tmp;
    }

    complex operator/(const complex& z1, const complex& z2){
        complex tmp = complex(z1.re(), z1.im());
        tmp /= z2;
        return tmp;
    }

};






numbers::complex randc(double min=-10, double max=10){
    double drr = min + (double)rand() * (max - min) / RAND_MAX;
    double dri = min + (double)rand() * (max - min) / RAND_MAX;
    return numbers::complex(drr, dri);
}

int main_() {
    srand((unsigned)time(NULL));
    
    /*numbers::complex a(3, 4);
    numbers::complex b(5, -6);
    
    std::cout << "a = " << a.to_string() << std::endl;
    std::cout << "b = " << b.to_string() << std::endl;*/
    
    for (int i = 0; i < 5; i ++){
        numbers::complex a = randc();
        std::cout << a.to_string() << std::endl;
    }
    
    return 0;
}



#include <iostream>
#include <cstdlib>
//using namespace std;
 
// Define the default capacity of the stack
#define SIZE 10
 
// A class to represent a stack
class stack
{
    int *arr;
    int top;
    int capacity;
 
public:
    stack(int size = SIZE);         // constructor
    ~stack();                       // destructor
 
    void push(int);
    int pop();
    int peek();
 
    int size();
    bool isEmpty();
    bool isFull();
};
 
// Constructor to initialize the stack
stack::stack(int size)
{
    arr = new int[size];
    capacity = size;
    top = -1;
}
 
// Destructor to free memory allocated to the stack
stack::~stack() {
    delete[] arr;
}
 
// Utility function to add an element `x` to the stack
void stack::push(int x)
{
    if (isFull())
    {
        std::cout << "Overflow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }
 
    std::cout << "Inserting " << x << std::endl;
    arr[++top] = x;
}
 
// Utility function to pop a top element from the stack
int stack::pop()
{
    // check for stack underflow
    if (isEmpty())
    {
        std::cout << "Underflow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }
 
    std::cout << "Removing " << peek() << std::endl;
 
    // decrease stack size by 1 and (optionally) return the popped element
    return arr[top--];
}
 
// Utility function to return the top element of the stack
int stack::peek()
{
    if (!isEmpty()) {
        return arr[top];
    }
    else {
        exit(EXIT_FAILURE);
    }
}
 
// Utility function to return the size of the stack
int stack::size() {
    return top + 1;
}
 
// Utility function to check if the stack is empty or not
bool stack::isEmpty() {
    return top == -1;               // or return size() == 0;
}
 
// Utility function to check if the stack is full or not
bool stack::isFull() {
    return top == capacity - 1;     // or return size() == capacity;
}
 
int main()
{
    stack pt(3);
 
    pt.push(1);
    pt.push(2);
 
    pt.pop();
    pt.pop();
 
    pt.push(3);
 
    std::cout << "The top element is " << pt.peek() << std::endl;
    std::cout << "The stack size is " << pt.size() << std::endl;
 
    pt.pop();
 
    if (pt.isEmpty()) {
        std::cout << "The stack is empty\n";
    }
    else {
        std::cout << "The stack is not empty\n";
    }
 
    return 0;
}
