#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<array>
#include<map>
#include<unordered_map>
#include<set>
#include<unordered_set>
using namespace std;
template<typename T1,typename T2>istream &operator>>(istream&,pair<T1,T2>&);
template<typename...Args>istream &operator>>(istream&,tuple<Args...>&a);
template<typename T>istream &operator>>(istream&is,vector<T>&a);
template<typename T,size_t N>istream &operator>>(istream&is,array<T,N>&a);
template<typename T1,typename T2>
istream &operator>>(istream&is,pair<T1,T2>&a){
  is>>a.first>>a.second;
  return is;
}
template<size_t pos,typename...Args>
void read_tuple(istream&is,tuple<Args...>&a){
  if constexpr(pos<tuple_size<tuple<Args...>>::value){
    is>>get<pos>(a);
    read_tuple<pos+1>(is,a);
  }
}
template<typename...Args>
istream &operator>>(istream&is,tuple<Args...>&a){
  read_tuple<0>(is,a);
  return is;
}
template<typename T>
istream &operator>>(istream&is,vector<T>&a){
  for(T&x:a)is>>x;
  return is;
}
template<typename T,size_t N>
istream &operator>>(istream&is,array<T,N>&a){
  for(T&x:a)is>>x;
  return is;
}
template<typename T1,typename T2>ostream &operator<<(ostream&os,const pair<T1,T2>&);
template<typename...Args>ostream &operator<<(ostream&os,const tuple<Args...>&);
template<typename T>ostream &operator<<(ostream&os,const vector<T>&);
template<typename T,typename Seq,typename Comp>ostream &operator<<(ostream&os,priority_queue<T,Seq,Comp>);
template<typename T>ostream &operator<<(ostream&os,queue<T>);
template<typename T>ostream &operator<<(ostream&os,deque<T>);
template<typename T>ostream &operator<<(ostream&os,stack<T>);
template<typename T,size_t N>ostream &operator<<(ostream&os,const array<T,N>&);
template<typename Key,typename Val,typename Comp>ostream &operator<<(ostream&os,const map<Key,Val,Comp>&);
template<typename Key,typename Val,typename Hash>ostream &operator<<(ostream&os,const unordered_map<Key,Val,Hash>&);
template<typename T,typename Comp>ostream &operator<<(ostream&os,const set<T,Comp>&);
template<typename T,typename Comp>ostream &operator<<(ostream&os,const multiset<T,Comp>&);
template<typename T,typename Hash>ostream &operator<<(ostream&os,const unordered_set<T,Hash>&);
template<typename T1,typename T2>
ostream &operator<<(ostream&os,const pair<T1,T2>&a){
  os<<a.first<<' '<<a.second;
  return os;
}
template<size_t pos,typename...Args>
void write_tuple(ostream&os,const tuple<Args...>&a){
  if constexpr(pos<tuple_size<tuple<Args...>>::value){
    if constexpr(pos>0)os<<' ';
    os<<get<pos>(a);
    write_tuple<pos+1>(os,a);
  }
}
template<typename...Args>
ostream &operator<<(ostream&os,const tuple<Args...>&a){
  write_tuple<0>(os,a);
  return os;
}
template<typename T>
ostream &operator<<(ostream&os,const vector<T>&a){
  os<<'{';
  for(int i=0;i<(int)a.size();i++){
    os<<a[i];
    if(i+1!=a.size())os<<',';
  }
  os<<'}';
  return os;
}
template<typename T,typename Seq,typename Comp>
ostream &operator<<(ostream&os,priority_queue<T,Seq,Comp>a){
  os<<'{';
  if(!a.empty()){
    os<<a.top();a.pop();
    while(!a.empty()){
      os<<',';
      os<<a.top();
      a.pop();
    }
  }
  os<<'}';
  return os;
}
template<typename T>
ostream &operator<<(ostream&os,queue<T>a){
  os<<'{';
  if(!a.empty()){
    os<<a.front();a.pop();
    while(!a.empty()){
      os<<',';
      os<<a.front();
      a.pop();
    }
  }
  os<<'}';
  return os;
}
template<typename T>
ostream &operator<<(ostream&os,deque<T>a){
  os<<'{';
  if(!a.empty()){
    os<<a.front();a.pop_front();
    while(!a.empty()){
      os<<',';
      os<<a.front();
      a.pop_front();
    }
  }
  os<<'}';
  return os;
}
template<typename T>
ostream &operator<<(ostream&os,stack<T>a){
  os<<'{';
  if(!a.empty()){
    os<<a.top();a.pop();
    while(!a.empty()){
      os<<',';
      os<<a.top();
      a.pop();
    }
  }
  os<<'}';
  return os;
}
template<typename T,size_t N>
ostream &operator<<(ostream&os,const array<T,N>&a){
  os<<'{';
  for(int i=0;i<(int)a.size();i++){
    os<<a[i];
    if(i+1!=a.size())os<<',';
  }
  os<<'}';
  return os;
}
template<typename Key,typename Val,typename Comp>
ostream &operator<<(ostream&os,const map<Key,Val,Comp>&a){
  if(a.empty()){
    os<<"{}";
    return os;
  }
  auto itr=a.begin();
  os<<"{["<<itr->first<<","<<itr->second<<']';
  while(++itr!=a.end())os<<",["<<itr->first<<','<<itr->second<<']';
  os<<'}';
  return os;
}
template<typename Key,typename Val,typename Hash>
ostream &operator<<(ostream&os,const unordered_map<Key,Val,Hash>&a){
  if(a.empty()){
    os<<"{}";
    return os;
  }
  auto itr=a.begin();
  os<<"{["<<itr->first<<","<<itr->second<<']';
  while(++itr!=a.end())os<<",["<<itr->first<<','<<itr->second<<']';
  os<<'}';
  return os;
}
template<typename T,typename Comp>
ostream &operator<<(ostream&os,const set<T,Comp>&a){
  if(a.empty()){
    os<<"{}";
    return os;
  }
  auto itr=a.begin();
  os<<'{'<<*itr;
  while(++itr!=a.end())os<<','<<*itr;
  os<<'}';
  return os;
}
template<typename T,typename Comp>
ostream &operator<<(ostream&os,const multiset<T,Comp>&a){
  if(a.empty()){
    os<<"{}";
    return os;
  }
  auto itr=a.begin();
  os<<'{'<<*itr;
  while(++itr!=a.end())os<<','<<*itr;
  os<<'}';
  return os;
}
template<typename T,typename Hash>
ostream &operator<<(ostream&os,const unordered_set<T,Hash>&a){
  if(a.empty()){
    os<<"{}";
    return os;
  }
  auto itr=a.begin();
  os<<'{'<<*itr;
  while(++itr!=a.end())os<<','<<*itr;
  os<<'}';
  return os;
}