#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>


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
            //return pow(real, 2) + pow(imag, 2);
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


/*int main() {
    numbers::complex a(3, 4);
    numbers::complex b(5, -6);
    
    std::cout << "a = " << a.to_string() << std::endl;
    std::cout << "b = " << b.to_string() << std::endl;
    
    numbers::complex c = a - b;
    numbers::complex d = -c;
    numbers::complex e = ~c;
    numbers::complex f = ~ ~c;
    
    std::cout << "c = a - b = " << c.to_string() << std::endl;
    std::cout << "   -c = " << d.to_string() << std::endl;
    std::cout << "   ~c = " << e.to_string() << std::endl;
    std::cout << " ~ ~c = " << f.to_string() << std::endl;
    
    c = a + b;
    d = a * b;
    e = a / b;
    
    std::cout << "a + b = " << c.to_string() << std::endl;
    std::cout << "a * b = " << d.to_string() << std::endl;
    std::cout << "a / b = " << e.to_string() << std::endl;
    
    return 0;
}*/
