#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "geo/union_rectangles_range.hpp"
#include "random/generator.hpp"
long long naive(std::vector<std::tuple<long long,long long,long long,long long>>a,int l,int r){
  std::vector<long long>zx,zy;
  for(const auto&[lx,rx,ly,ry]:a){
    zx.push_back(lx);
    zx.push_back(rx);
    zy.push_back(ly);
    zy.push_back(ry);
  }
  std::sort(zx.begin(),zx.end()),zx.erase(std::unique(zx.begin(),zx.end()),zx.end());
  std::sort(zy.begin(),zy.end()),zy.erase(std::unique(zy.begin(),zy.end()),zy.end());
  std::vector<std::vector<int>>imos(zx.size(),std::vector<int>(zy.size()));
  for(auto [lx,rx,ly,ry]:a){
    lx=std::lower_bound(zx.begin(),zx.end(),lx)-zx.begin();
    rx=std::lower_bound(zx.begin(),zx.end(),rx)-zx.begin();
    ly=std::lower_bound(zy.begin(),zy.end(),ly)-zy.begin();
    ry=std::lower_bound(zy.begin(),zy.end(),ry)-zy.begin();
    imos[lx][ly]++;
    imos[lx][ry]--;
    imos[rx][ly]--;
    imos[rx][ry]++;
  }
  for(int i=0;i<(int)zx.size();i++)for(int j=0;j<(int)zy.size()-1;j++)imos[i][j+1]+=imos[i][j];
  for(int i=0;i<(int)zx.size()-1;i++)for(int j=0;j<(int)zy.size();j++)imos[i+1][j]+=imos[i][j];
  long long res=0;
  for(int i=0;i<(int)zx.size()-1;i++)for(int j=0;j<(int)zy.size()-1;j++)if(l<=imos[i][j]&&imos[i][j]<r){
    res+=(zx[i+1]-zx[i])*(zy[j+1]-zy[j]);
  }
  return res;
}
void test(std::vector<std::tuple<long long,long long,long long,long long>>a,int l,int r){
  assert(union_rectangles_range(a,l,r)==naive(a,l,r));
}
std::vector<std::tuple<long long,long long,long long,long long>>gen(int n,long long lim){
  std::vector<std::tuple<long long,long long,long long,long long>>res(n);
  for(auto&[lx,rx,ly,ry]:res){
    lx=Random::range(-lim,lim+1);
    rx=Random::range(-lim,lim+1);
    ly=Random::range(-lim,lim+1);
    ry=Random::range(-lim,lim+1);
    if(lx>rx)std::swap(lx,rx);
    if(ly>ry)std::swap(ly,ry);
  }
  return res;
}
int main(){
  for(int n=1;n<=100;n++){
    for(int i=0;i<10;i++){
      auto a=gen(n,20);
      auto [l,r]=Random::distinct(n+1);
      l++,r++;
      test(a,l,r);
    }
    for(int i=0;i<10;i++){
      auto a=gen(n,1000000000);
      auto [l,r]=Random::distinct(n+1);
      l++,r++;
      test(a,l,r);
    }
  }
  for(int i=0;i<10;i++){
    const int n=3000;
    auto a=gen(n,100);
    auto b=gen(n,1000000000);
    auto [l,r]=Random::distinct(n+1);
    l++,r++;
    test(a,l,r);
    test(b,l,r);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}