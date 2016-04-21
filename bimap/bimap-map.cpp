#ifndef MY_BIMAP_H
#define MY_BIMAP_H

#include <iostream>
#include <algorithm>
#include <utility>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include <unordered_map>
#include <functional>

template <typename key, typename val>
class bimap{

typedef std::pair<key, val> value_type;


private:
  std::unordered_map<key, value_type> map;
  void _swap(bimap<key, val>& first, bimap<key, val>& second){
      std::swap(first.map, second.map);
  }

public:
class iter : public boost::iterator_facade<
  iter,
  value_type,
  boost::forward_traversal_tag
  >
  {
    friend class boost::iterator_core_access;
  public:
    iter(typename std::unordered_map<key, value_type>::iterator& base) :
      _base(base) {}
    iter(typename std::unordered_map<key, value_type>::iterator&& base){
      std::swap(_base, base);
    }
    ~iter(){}

  private:
    typename std::unordered_map<key, value_type>::iterator _base;
    void increment(){
      _base++;
    }

    value_type& dereference() const {
      return _base->second; }

    bool equal(typename bimap<key, val>::iter const& other) const{
      return (_base == other._base);
    }

  };


  friend class bimap<key, val>::iter;
public:
  bimap() {}
  bimap(const bimap<key, val> &copy);
  bimap(bimap<key, val> &&move);
  bimap<key, val>& operator= (bimap<key, val> copied);
  ~bimap(){}
  std::pair<bimap<key, val>::iter, bool> insert(const value_type& _pair);
  bimap<key, val>::iter begin();
  bimap<key, val>::iter end();
  bool empty();
  std::size_t size();
  bimap<key, val>::iter find_byVal(const val& Value);
  bimap<key, val>::iter find_byKey(const key& Key);
  val& operator[](const key K);

};

template <typename key, typename val>
bimap<key, val>::bimap(const bimap<key, val> &copy){
  //std::copy(copy.map.begin(), copy.map.end(), map.begin());
  map = copy.map;
}

template <typename key, typename val>
bimap<key, val>::bimap(bimap<key, val> &&move){
  _swap(*this, move);
}

template <typename key, typename val>
bimap<key, val>& bimap<key, val>::operator= (bimap<key, val> copied){
  _swap(*this, copied);
  return *this;
}

template <typename key, typename val>
std::pair<typename bimap<key, val>::iter, bool> bimap<key, val>::insert(const value_type& _pair){
  auto result = map.insert(std::pair<key, value_type>(_pair.first, _pair));
  return std::pair<typename bimap<key, val>::iter, bool> (iter(result.first), result.second);
}

template <typename key, typename val>
typename bimap<key, val>::iter bimap<key, val>::begin(){
  return iter(map.begin());
}

template <typename key, typename val>
typename bimap<key, val>::iter bimap<key, val>::end(){
  return iter(map.end());
}

template <typename key, typename val>
bool bimap<key, val>::empty(){
  return map.empty();
}

template <typename key, typename val>
std::size_t bimap<key, val>::size(){
  return map.size();
}

template <typename key, typename val>
typename bimap<key, val>::iter bimap<key, val>::find_byKey(const key& Key){
  return iter(map.find(Key));
}

template <typename key, typename val>
typename bimap<key, val>::iter bimap<key, val>::find_byVal(const val& Val){
  auto it = std::find_if(map.begin(), map.end(), [&](std::pair<const key, std::pair<int, int>>& pair){
    return (pair.second.second == Val);
  });
  return iter(it);
}

template <typename key, typename val>
val& bimap<key, val>::operator[](key Key){
  auto it = find_byKey(Key);
  if(it != end())
    return it->second;
  return insert(std::pair<key, val>()).first->second;
}

int main(){
  bimap<int, int> map;
  bimap<int, int> map2;
  auto res = map.insert(std::pair<int, int> (2, 16));
  auto it = *(res.first);
  std::cout << (it).first << " " << (it).second << std::endl;
  std::cout << (it).first << " " << (it).second << std::endl;
  std::cout << (it).first << " " << (it).second << std::endl;
  map.insert(std::pair<int, int> (3, 17));
  map.insert(std::pair<int, int> (4, 18));
  auto it_begin = map.begin();
  std::cout << (*it_begin).first << " " << (*it_begin).second << std::endl;
  std::cout << (*it_begin).first << " " << (*it_begin).second << std::endl;
  it_begin++;
  std::cout << (*it_begin).first << " " << (*it_begin).second << std::endl;

  map2 = map;
  std::for_each(map2.begin(), map2.end(), [](std::pair<int, int>& pair){
    std::cout << "map2: " << pair.first << " " << pair.second << std::endl;
  });
  std::for_each(map.begin(), map.end(), [](std::pair<int, int>& pair){
    std::cout << "map: " << pair.first << " " << pair.second << std::endl;
  });
  auto keyIt = map.find_byKey(3);
  std::cout << keyIt->second << std::endl;

  auto ValIt = map.find_byVal(17);
  std::cout << keyIt->first << std::endl;
  std::cout << map[4] << std::endl;
  map[5] = 6;
  std::cout << map[5] << std::endl;
  bimap<int, std::string> strmap;
  strmap[5] = "abc";
  std::cout << strmap[5] << std::endl;
}

#endif
