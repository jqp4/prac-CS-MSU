#include <iostream>
#include <string>

int main(void) {
    std::string s;
    while (std::cin >> s) {
        size_t i = 0;
        size_t len = s.length();
        while (i < len && (s[i]=='3'||s[i]=='4')) i++;
        while (i < len && (s[i]=='1'||s[i]=='2')) i++;
        int ans = i >= len;
        std::cout << ans << std::endl;
    }
}
