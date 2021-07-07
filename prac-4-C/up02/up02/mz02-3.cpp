class C{
public:
    C(int ci1 = 0, int ci2 = 0){}
    C(const C &cct){}
    C(const C *pct){}
    C(double cdub){}

    C& operator++(){
        return *this;
    }
    
    friend int operator~(const C &c){
        return 0;
    }
    friend int operator+(int x, const C &c){
        return 0;
    }
    friend int operator*(const C &c, const C *p){
        return 0;
    }
};
