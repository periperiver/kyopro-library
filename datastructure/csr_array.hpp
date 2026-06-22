#pragma once
#include<vector>
#include<type_traits>
template<typename T>
struct csr_array{
private:
  std::vector<int>ptr;
  std::vector<T>dat;
public:
  using iterator=typename std::vector<T>::iterator;
  using const_iterator=typename std::vector<T>::const_iterator;
  struct csr{
    iterator l,r;
    iterator begin(){return l;}
    iterator end(){return r;}
    int size()const{return r-l;}
    T &operator[](int i)const{return l[i];}
  };
  struct const_csr{
    const_iterator l,r;
    const_iterator begin(){return l;}
    const_iterator end(){return r;}
    int size()const{return r-l;}
    const T &operator[](int i)const{return l[i];}
  };
  csr_array():ptr{0}{}
  csr_array(int n,const std::vector<std::pair<int,T>>&a):ptr(n+1),dat(a.size()){
    for(const auto&[k,v]:a)ptr[k]++;
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    for(int i=std::ssize(a);i--;)dat[--ptr[a[i].first]]=a[i].second;
  }
  explicit csr_array(int n,const std::vector<int>&a):ptr(n+1),dat(a.size()){
    static_assert(std::is_same_v<T,int>);
    for(const int&x:a)ptr[x]++;
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    for(int i=std::ssize(a);i--;)dat[--ptr[a[i]]]=i;
  }
  csr operator[](int i){return csr{dat.begin()+ptr[i],dat.begin()+ptr[i+1]};}
  const_csr operator[](int i)const{return const_csr{dat.begin()+ptr[i],dat.begin()+ptr[i+1]};}
  iterator begin(){return dat.begin();}
  iterator end(){return dat.end();}
  const_iterator begin()const{return dat.begin();}
  const_iterator end()const{return dat.end();}
  int size()const{return std::ssize(ptr)-1;}
};