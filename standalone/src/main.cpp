#include <array>
#include <iostream>
#include <scl/prelude.h>
#include <string>

using namespace scl::prelude;

int main(int argc, char** argv) {
    std::array<int, 5> arr{1, 2, 3, 4, 5};

    streamFrom(arr)
        | map([](int x){
              return x * 2;
          })
        | forEach([](int x){
              std::cout << x << '\n';
          });

    return 0;
}
