#pragma once
#include<vector>
#include<concepts>
#include<limits>
#include "../math/util.hpp"
template<typename T>
struct StaticRangeMin{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  std::vector<std::vector<T>>dat;
public:
  StaticRangeMin(){}
  explicit StaticRangeMin(std::vector<T>a):dat(msb(a.size())+1){
    if(a.empty())return;
    int n=a.size();
    dat[0]=std::move(a);
    for(int i=1;i<(int)dat.size();i++){
      dat[i]=std::vector<T>(n-(1<<i)+1);
      for(int j=0;j<(int)dat[i].size();j++)dat[i][j]=std::min(dat[i-1][j],dat[i-1][j+(1<<(i-1))]);
    }
  }
  T min(int l,int r)const{
    int m=msb(r-l);
    return std::min(dat[m][l],dat[m][r-(1<<m)]);
  }
};