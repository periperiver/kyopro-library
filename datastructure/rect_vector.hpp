#pragma once
#include<iostream>
#include<vector>
#include<cassert>
#include<algorithm>
template<typename T>
struct rect_vector{
protected:
  int n,m;
  std::vector<T>a;
public:
  rect_vector():n(0),m(0){}
  rect_vector(int n,int m):n(n),m(m),a(n*m){}
  rect_vector(const std::vector<std::vector<T>>&init):n(init.size()),m(init.empty()?0:init[0].size()),a(n*m){
    if(n>0){
      assert(std::all_of(init.begin(),init.end(),[&](const std::vector<T>&x){return (int)x.size()==m;}));
      for(int i=0;i<n;i++)for(int j=0;j<m;j++)a[i*m+j]=init[i][j];
    }
  }
  rect_vector(int n,int m,const std::vector<T>&init):n(n),m(m),a(init){
    assert((long long)n*m==(long long)init.size());
  }
  void resize(int n_,int m_){
    std::vector<T>na(n_*m_);
    for(int i=0;i<std::min(n,n_);i++)for(int j=0;j<std::min(m,m_);j++){
      na[i*m_+j]=a[i*m+j];
    }
    a=std::move(na);
    n=n_,m=m_;
  }
  std::pair<int,int>size()const{return std::make_pair(n,m);}
  std::vector<T>::iterator operator[](int x){return a.begin()+x*m;}
  std::vector<T>::const_iterator operator[](int x)const{return a.begin()+x*m;}
  [[nodiscard]]rect_vector transpose()const{
    rect_vector res(m,n);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)res[j][i]=a[i*m+j];
    return res;
  }
  [[nodiscard]]rect_vector rotl()const{
    rect_vector res(m,n);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)res[m-1-j][i]=a[i*m+j];
    return res;
  }
  [[nodiscard]]rect_vector rotr()const{
    rect_vector res(m,n);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)res[j][n-1-i]=a[i*m+j];
    return res;
  }
  friend std::istream &operator>>(std::istream&is,rect_vector&rhs){
    for(int i=0;i<(int)rhs.a.size();i++)is>>rhs.a[i];
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const rect_vector&rhs){
    os<<'{';
    for(int i=0;i<rhs.n;i++){
      os<<'{';
      for(int j=0;j<rhs.m;j++){
        os<<rhs[i][j];
        if(j+1<rhs.m)os<<',';
      }
      os<<'}';
      if(i+1<rhs.n)os<<',';
    }
    os<<'}';
    return os;
  }
};