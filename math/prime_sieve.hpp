#pragma once
#include<vector>
#include<cmath>
#include<cstdint>
std::vector<int>prime_sieve(int n){
  if(n<=6){
    if(n<=1)return std::vector<int>{};
    else if(n==2)return std::vector<int>{2};
    else if(n<=4)return std::vector<int>{2,3};
    else return std::vector<int>{2,3,5};
  }
  static constexpr int mod30table[8]={1,7,11,13,17,19,23,29};
  static constexpr uint8_t k_mask[][8] = {
    {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f},
    {0xfd, 0xdf, 0xef, 0xfe, 0x7f, 0xf7, 0xfb, 0xbf},
    {0xfb, 0xef, 0xfe, 0xbf, 0xfd, 0x7f, 0xf7, 0xdf},
    {0xf7, 0xfe, 0xbf, 0xdf, 0xfb, 0xfd, 0x7f, 0xef},
    {0xef, 0x7f, 0xfd, 0xfb, 0xdf, 0xbf, 0xfe, 0xf7},
    {0xdf, 0xf7, 0x7f, 0xfd, 0xbf, 0xfe, 0xef, 0xfb},
    {0xbf, 0xfb, 0xf7, 0x7f, 0xfe, 0xef, 0xdf, 0xfd},
    {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe},
  };
  static constexpr int c0[][8] = {
    {0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 0, 1, 1, 1, 1},
    {2, 2, 0, 2, 0, 2, 2, 1}, {3, 1, 1, 2, 1, 1, 3, 1},
    {3, 3, 1, 2, 1, 3, 3, 1}, {4, 2, 2, 2, 2, 2, 4, 1},
    {5, 3, 1, 4, 1, 3, 5, 1}, {6, 4, 2, 4, 2, 4, 6, 1},
  };
  static constexpr int c1[8]={6,4,2,4,2,4,6,2};
  n++;
  int sz=(n+29)/30;
  std::vector<uint8_t>p(sz,0xff);
  {
    int r=n%30;
    if(r==0);
    else if(r==1)p.back()=0x00;
    else if(r<=7)p.back()=0x01;
    else if(r<=11)p.back()=0x03;
    else if(r<=13)p.back()=0x07;
    else if(r<=17)p.back()=0x0f;
    else if(r<=19)p.back()=0x1f;
    else if(r<=23)p.back()=0x3f;
    else if(r<=29)p.back()=0x7f;
  }
  p[0]=0xfe;
  int sq=std::min(sz-1,((int)std::sqrt(n)+29)/30);
  for(int i=0;i<=sq;i++){
    for(uint8_t f=p[i];f>0;f=f&(f-1)){
      uint8_t l=__builtin_ctz(f);
      int m=mod30table[l];
      int pm=i*30+m*2;
      for(int j=i*pm+m*m/30,k=l;j<(int)p.size();j+=i*c1[k]+c0[l][k],k=(k+1)&7){
        p[j]&=k_mask[l][k];
      }
    }
  }
  std::vector<int>res{2,3,5};
  for(int i=0;i<(int)p.size()-1;i++){
    for(int j=p[i];j>0;j=j&(j-1))res.push_back(i*30+mod30table[__builtin_ctz(j)]);
  }
  for(int j=p.back();j>0;j=j&(j-1)){
    int l=__builtin_ctz(j);
    int k=(p.size()-1)*30+mod30table[l];
    if(k<n)res.push_back(k);
    else break;
  }
  return res;
}