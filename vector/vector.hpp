#ifndef MY_PERSONAL_LITTLE_VECTOR_H
#define MY_PERSONAL_LITTLE_VECTOR_H

#include  <algorithm>
#include <stdexcept>
#include <iostream>

template <class Type>
class Vector{
public:
  Vector(int size = 0);
  Vector(const Vector<Type> &copy);
  Vector(Vector &&move);
  Vector<Type>& operator=(Vector<Type> copied);
  ~Vector();
  int size() const;
  int max_size() const;
  bool resize(const int newSize);
  bool empty() const;
  Type& operator[](const int);
  Type& at(const int);
  void push_back(const Type data);
  void pop_back();
  Type& back();
  Vector<Type>& operator+=(const Vector<Type> v);
  Vector<Type>& operator-=(const Vector<Type> v);

  friend Vector<Type>& operator*(Vector<Type> v, const Type num){
    for(int i = 0; i < v._size; i++)
      v._array[i] *= num;
    return v;
  }

  friend Vector<Type>& operator*(const Type num, Vector<Type>& v){
    for(int i = 0; i < v._size; i++)
      v._array[i] *= num;
    return v;
  }

  friend Type operator*(const Vector<Type> &left, const Vector<Type>& right){
    if(left._size != right._size)
      throw std::length_error("Different length of vectors");
    Type result;
    Vector<Type> sum(left._size);
    for(int i = 0; i < sum._size; i++){
      result += right._array[i] * left._array[i];
    }
    return result;
  }

  friend const Vector<Type> operator+(const Vector<Type> &left, const Vector<Type>& right){
    if(left._size != right._size)
      throw std::length_error("Different length of vectors");
    Vector<Type> sum(left._size);
    for(int i = 0; i < sum._size; i++){
      sum._array[i] = right._array[i] + left._array[i];
    }
    return sum;
  }


private:
  Type* _array;
  int _size;
  int _max_size;
  void _swap(Vector& first, Vector& second);
};

template <class Type>
Vector<Type>::Vector(int size) :
  _size(size),
  _max_size(size >= 0 ? size : 1)
{
  _array = (_max_size ? new Type[_max_size] : 0);
}

template <class Type>
Vector<Type>::Vector(const Vector& copy) :
  _size(copy._size),
  _max_size(copy._max_size)
{
  _array = (_max_size ? new Type[_max_size] : 0);
  std::copy(copy._array, copy._array + _max_size, _array);
}

template <class Type>
Vector<Type>::Vector(Vector &&move){
  _swap(*this, move);
}

template <class Type>
Vector<Type>& Vector<Type>::operator=(Vector<Type> copied)
{
    _swap(*this, copied);
    return *this;
}

template <class Type>
Vector<Type>::~Vector(){
  delete [] _array;
}

template <class Type>
void Vector<Type>::_swap(Vector& first, Vector& second){
  std::swap(first._max_size, second._max_size);
  std::swap(first._size, second._size);
  std::swap(first._array, second._array);
}

template<class Type>
int Vector<Type>::size() const{
  return _size;
}

template<class Type>
int Vector<Type>::max_size() const{
  return _max_size;
}

template<class Type>
bool Vector<Type>::resize(int newSize){
  if(newSize <= 0)
    return false;
  auto newArray = new Type[newSize];
  std::copy(_array, _array + (newSize > _size ? _size : newSize), newArray);
  _max_size = newSize;
  if(_size > newSize)
    _size = newSize;
  delete [] _array;
  _array = newArray;
  return _array;
}

template<class Type>
bool Vector<Type>::empty() const{
  return _size;
}

template<class Type>
Type& Vector<Type>::operator[] (int i){
  if(i > _max_size)
    resize(i);
  return _array[i];
}

template<class Type>
Type& Vector<Type>::at(int i){
  if(i > _max_size)
    throw std::out_of_range("Out of range");
  return _array[i];
}

template<class Type>
void Vector<Type>::push_back(Type data){
  if(_max_size == 0)
    resize(1);
  if(_size == _max_size)
    resize(2 * _max_size);
  _array[_size++] = data;
}

template<class Type>
void Vector<Type>::pop_back(){
  if(_size > 0)
    _size--;
}

template <class Type>
Type& Vector<Type>::back(){
  if(!_size)
    throw std::out_of_range("Out of range");
  return _array[_size - 1];
}

template <class Type>
Vector<Type>& Vector<Type>::operator+=(const Vector<Type> v){
  if(_size != v._size)
    throw std::length_error("Different length of vectors");
  for(int i = 0; i < _size; i++)
    _array[i] += v[i]._array;
  return *this;
}

template <class Type>
Vector<Type>& Vector<Type>::operator-=(const Vector<Type> v){
  if(_size != v._size)
    throw std::length_error("Different length of vectors");
  for(int i = 0; i < _size; i++)
    _array[i] -= v._array[i];
  return *this;
}

#endif
