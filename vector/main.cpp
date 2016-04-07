#include <iostream>
#include "vector.hpp"

int main()
{
  Vector<int> a(0);
  Vector<int> b = a;
  Vector<int> ex1(12);
  Vector<int> ex2(12);
  for(int i = 0; i < 12; i++){
    ex1[i] = rand() % 10;
    std::cout << "ex1[" << i << "]= " << ex1[i] << std::endl;
    ex2[i] = rand() % 10;
    std::cout << "ex2[" << i << "]= " << ex2[i] << std::endl;
  }
  Vector<int> ex3(12);
  ex3 = ex1 + ex2;
  for(int i = 0; i < 12; i++)
    std::cout << "ex3[" << i << "]= " << ex3[i] << std::endl;
  ex3 -= ex3;
  for(int i = 0; i < 12; i++)
    std::cout << "ex3[" << i << "]= " << ex3[i] << std::endl;
  Vector<int> v4 = ex3 + 2 * ex1;
  for(int i = 0; i < 12; i++)
    std::cout << "ex4[" << i << "]= " << v4[i] << std::endl;
  /*for(int i = 0; i < 10; i++){
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
  a.push_back(2);*/
  return 0;
}
