#ifndef MY_BIMAP_H
#define MY_BIMAP_H

#include <iostream>
#include <algorithm>
#include <utility>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>

//template <typename key, typename val>
template <template <typename, typename> class Type, typename key, typename val>
class iter;

template <typename key, typename val>
class bimap{
  friend class iter<bimap, key, val>;
public:
  bimap(int size = 0);
  bimap(const bimap<key, val> &copy);
  bimap(bimap<key, val> &&move);
  bimap<key, val>& operator= (bimap<key, val> copied);
  ~bimap();
  int size() const;
  bool empty() const;
  val& operator[](const key K);
  bool insert(std::pair<key, val> input);
  iter<bimap, key, val> begin();
  iter<bimap, key, val> end();

private:
  std::pair<key, val> * _array;
  void _swap(bimap<key, val>& first, bimap<key, val>& second);
  int _size;
  int _max_size;
  bool _resize(int size);
  std::pair<key, val>* _find_key(key K);
  std::pair<key, val>* _find_value(val V);
};

template <typename key, typename val>
bimap<key, val>::bimap(int size) :
  _array(size > 0 ? new std::pair<key, val> [size] : 0),
  _size(0),
  _max_size(size)
  {}

template <typename key, typename val>
bimap<key, val>::bimap(const bimap<key, val> &copy) :
  _size(copy._size),
  _max_size(copy._max_size),
  _array(copy._max_size ? new std::pair<key, val> [copy._max_size]: 0)
  {
    std::copy(copy._array, copy._array + copy._max_size, _array);
  }

template <typename key, typename val>
bimap<key, val>::bimap(bimap<key, val> &&move){
  swap(*this, move);
}

template <typename key, typename val>
bimap<key, val>& bimap<key, val>::operator= (bimap<key, val> copied){
  _swap(*this, copied);
  return *this;
}

template <typename key, typename val>
void bimap<key, val>::_swap(bimap<key, val>& first, bimap<key, val>& second){
  std::swap(first._size, second._size);
  std::swap(first._max_size, second._max_size);
  std::swap(first._array, second._array);
}

template <typename key, typename val>
bimap<key, val>::~bimap(){
  delete [] _array;
}

template <typename key, typename val>
int bimap<key, val>::size() const{
  return _size;
}

template <typename key, typename val>
bool bimap<key, val>::empty() const{
  return !_size;
}

template <typename key, typename val>
std::pair<key, val>* bimap<key, val>::_find_key(key K){
  std::pair<key, val> * elem = 0;
  for(int i = 0; i < _size; i++)
    if(_array[i].first == K){
      elem = _array + i;
    }
  return elem;
}

template <typename key, typename val>
std::pair<key, val>* bimap<key, val>::_find_value(val V){
  std::pair<key, val>* elem = 0;
  for(int i = 0; i < _size; i++)
    if(_array[i].second == V){
      elem = &_array[i];
    }
  return elem;
}

template <typename key, typename val>
val& bimap<key, val>::operator[](key K){
  auto elem = _find_key(K);
  if(elem)
    return elem->second;
  return _array[_size - 1]->second;
}

template <typename key, typename val>
bool bimap<key, val>::_resize(int new_size){
  if(new_size <= _size)
    return false;
  auto new_array = new std::pair<key, val> [new_size];
  std::copy(_array, _array + _max_size, new_array);
  _max_size = new_size;
  delete [] _array;
  _array = new_array;
  return true;
}

template <typename key, typename val>
bool bimap<key, val>::insert(const std::pair<key, val> newElem){

  if(_find_key(newElem.first))
    return false;
  if(_size == _max_size)
    _resize(2 * (_max_size + 1));
  _array[_size++] = newElem;
  return true;
}

template <typename key, typename val>
iter<bimap, key, val> bimap<key, val>::begin(){
  iter<bimap, key, val> it(this, 0);
  return it;
}

template <typename key, typename val>
iter<bimap, key, val> bimap<key, val>::end(){
  iter<bimap, key, val> it(this, _size);
  return it;
}


template < template <typename, typename> class Type, typename key, typename val>
class iter : public boost::iterator_facade<
  iter<Type, key, val>,
  Type<key, val>,
  boost::bidirectional_traversal_tag,
  std::pair<key, val>&
  >
{
  friend class boost::iterator_core_access;
  //template <template <typename, typename> class, typename, typename>
  friend class iter<bimap, key, val>;

public:
  iter(Type<key, val>* base = 0, int index = 0) :
  _base(base),
  _index(index)
  {};
  ~iter() {};

private:
  struct enabler {};
  Type<key, val>* _base;
  int _index;
  void increment(){
    if(_base->_size > _index)
      _index++;
  }
  void decrement(){
    if(_index > 0)
      _index--;
  }
  template <template<typename, typename> class OtherValue>
  bool equal(iter<OtherValue, key, val> const & other) const{
    return (_index == other._index);
  }
  std::pair<key, val>& dereference() const{
    auto& pair = _base->_array[_index];
    return pair;
  }
};

//template <typename key, typename val>
//typedef iter<bimap<key, val>> bimap_iter;
//typedef iter<bimap<key, val> const> const_bimap_inter;

int main(){

  using std::cout;
  using std::endl;

  bimap<int, int> mymap;

  mymap.insert(std::pair<int, int>(1,2));
  mymap.insert(std::pair<int, int>(1,2));
  mymap.insert(std::pair<int, int>(1,2));
  mymap.insert(std::pair<int, int>(1,2));
  mymap.insert(std::pair<int, int>(3,4));
  cout << "size:" << endl;
  cout << mymap.size() << endl;
  auto it_begin = mymap.begin();
  auto it_end = mymap.end();
  it_end--;
  //(*it);
  cout << (*it_begin).first << " " << (*it_begin).second << endl;
  cout << (*it_end).first << " " << (*it_end).second << endl;
  std::for_each(mymap.begin(), mymap.end(), [](std::pair<int, int> pair){
    cout << pair.first << " " << pair.second << endl;
  });

  return 0;
}


#endif
