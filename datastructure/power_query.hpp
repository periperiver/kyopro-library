#pragma once
#include<vector>
#include<array>
#include<concepts>
template<typename M,int B=1024>
struct PowerQuery{
private:
  using S=typename M::S;
  std::vector<std::array<S,B>>dat;
  void extend(){
    S a=M::op(dat.back()[1],dat.back()[B-1]);
    std::array<S,B>&v=dat.emplace_back();
    v[0]=M::e();
    for(int i=1;i<B;i++)v[i]=M::op(v[i-1],a);
  }
public:
  PowerQuery(){}
  PowerQuery(const S&x):dat(1){
    dat[0][0]=M::e();
    for(int i=1;i<B;i++)dat[0][i]=M::op(dat[0][i-1],x);
  }
  template<typename T>
  S operator()(T k){
    S res=M::e();
    int p=0;
    while(k){
      if(p==(int)dat.size())extend();
      res=M::op(res,dat[p][k%B]);
      k/=B;
      p++;
    }
    return res;
  }
};