#include <iostream>
#include <string>
#include <unordered_map>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::unordered_map<std::string, int> map;

    int n;
    std::cin >> n;
    int count = 0;

    std::string tmp;
    tmp.reserve(1000);

    for (int i = 0; i < n; i++) {
        std::cin >> tmp;

        if (map.find(tmp) == map.end()) {
            map[tmp] = 1;
            count++;
        }
    }
    std::cout << count << std::endl;
}