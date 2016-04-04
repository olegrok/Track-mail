#include <iostream>
#include "vector.hpp"

int main()
{
  Vector<int> a(0);
  Vector<int> b = a;
  for(int i = 0; i < 10; i++){
    b.push_back(rand() % 10);
    std::cout << b.back() << std::endl;
  }
  std::cout << std::endl;
  for(int i = 0; i < 10; i++){
    std::cout << b.back() << std::endl;
    b.pop_back();
  }
  std::cout << std::endl;
  Vector<int> c(a);
  Vector<int> d(Vector<int>(15));
  std::cout << a.size() << std::endl;
  a.push_back(15);
  a.push_back(2);
  return 0;
}
