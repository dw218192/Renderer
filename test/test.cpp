#include <iostream>
#include <scene.h>

int main() {
    Scene s;
    auto res = s.from_obj("Sofa.obj");
    if(res.valid()) {
        std::cout << "Hello World!\n";
        return 0;
    } else {
        std::cout << res.error() << "\n";
        return 1;
    }
}