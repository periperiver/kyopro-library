#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "datastructure/dynamic/slide_sequence.hpp"
#include "random/generator.hpp"
struct Naive{
  std::vector<int>a;
  void add_left(int len,int val){
    std::vector<int>b(len,val);
    a.insert(a.begin(),b.begin(),b.end());
  }
  void add_right(int len,int val){
    std::vector<int>b(len,val);
    a.insert(a.end(),b.begin(),b.end());
  }
  int get(int key){
    return a[key];
  }
  void set(int key,int val){
    a[key]=val;
  }
  void replace_range_min(int len){
    for(int i=0;i<(int)a.size()-len;i++){
      for(int j=1;j<=len;j++)if(a[i]>a[i+j])a[i]=a[i+j];
    }
    a.resize(a.size()-len);
  }
  void replace_range_max(int len){
    for(int i=0;i<(int)a.size()-len;i++){
      for(int j=1;j<=len;j++)if(a[i]<a[i+j])a[i]=a[i+j];
    }
    a.resize(a.size()-len);
  }
  int size(){return a.size();}
};
void test(int q,int lenmax){
  SlideSequence<int,int>s;
  Naive na;
  while(q--){
    int sz=na.size();
    // std::cerr<<s.size()<<' '<<na.size()<<'\n';
    // for(int i=0;i<sz;i++)std::cerr<<na.a[i]<<" \n"[i+1==sz];
    // for(int i=0;i<sz;i++)std::cerr<<s.get(i)<<" \n"[i+1==sz];
    assert(s.size()==na.size());
    int t=Random::range(6);
    if(t<=1){
      int len=std::min(lenmax,na.size());
      if(len<=1)t=Random::range(2,6);
      else{
        len=Random::range(1,len);
        if(t==0){
          s.replace_range_min(len);
          na.replace_range_min(len);
        }
        else{
          s.replace_range_max(len);
          na.replace_range_max(len);
        }
        continue;
      }
    }
    if(t==2){
      if(na.size()==0)t=Random::range(3,6);
      else{
        int idx=Random::range(na.size());
        int val=Random::range(1000);
        na.set(idx,val);
        s.set(idx,val);
        continue;
      }
    }
    if(t==3){
      if(na.size()==0)t=Random::range(4,6);
      else{
        int idx=Random::range(na.size());
        assert(na.get(idx)==s.get(idx));
        continue;
      }
    }
    int len=Random::range(1,lenmax+1);
    int val=Random::range(1000);
    if(t==4)na.add_left(len,val),s.add_left(len,val);
    else na.add_right(len,val),s.add_right(len,val);
  }
}
int main(){
  for(int t=0;t<100;t++){
    test(100,Random::range(1,5));
  }
  for(int t=0;t<100;t++){
    test(100,Random::range(1,100));
  }
  for(int t=0;t<10;t++){
    test(10000,100);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}