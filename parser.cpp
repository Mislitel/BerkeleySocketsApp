#include "parser.h"

void parse(const char* s) {
    std::vector<short> digits;
    for(auto i = 0; i < strlen(s); i++)
        if(s[i] >= '0' && s[i] <= '9')
            digits.push_back(s[i] - '0');

    // If no digits found - return
    if(digits.empty())
        return;

    std::sort(digits.rbegin(), digits.rend());
    std::cout << "Sorted (desc): ";
    for(auto digit: digits) {
        std::cout << digit << " ";
    }
    std::cout << std::endl;
    std::cout << "Max: " << digits.front() << std::endl;
    std::cout << "Min: " << digits.back() << std::endl;
    std::cout << "Sum: " << std::accumulate(digits.begin(), digits.end(), 0) << std::endl;
}