#pragma once
#include<iostream>
#include<vector>
#include<queue>
template<typename T,typename Seq=std::vector<T>,typename Comp=std::less<T>>
struct ErasablePriorityQueue{
private:
  std::priority_queue<T,Seq,Comp>que,rem;
public:
  ErasablePriorityQueue(){}
  explicit ErasablePriorityQueue(std::vector<T>init):que(init.begin(),init.end()){}
  int size()const{return que.size()-rem.size();}
  void push(T x){que.push(std::move(x));}
  template<typename...Args>
  void emplace(Args&&...args){
    que.emplace(std::forward<Args>(args)...);
  }
  void erase(T x){rem.push(std::move(x));}
  template<typename...Args>
  void emplace_erase(Args&&...args){
    rem.emplace(std::forward<Args>(args)...);
  }
  void normalize(){while(!rem.empty()&&que.top()==rem.top())que.pop(),rem.pop();}
  T top(){
    normalize();
    return que.top();
  }
  void pop(){
    normalize();
    que.pop();
  }
  bool empty(){
    normalize();
    return que.empty();
  }
  friend std::ostream &operator<<(std::ostream&os,ErasablePriorityQueue rhs){
    os<<'{';
    while(!rhs.empty()){
      os<<rhs.top();
      rhs.pop();
      if(!rhs.empty())os<<',';
    }
    os<<'}';
    return os;
  }
};