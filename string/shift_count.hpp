#pragma once
#include "../convolution/ntt.hpp"
#include "../datastructure/csr_array.hpp"
#include "../math/modint.hpp"
#include<algorithm>
#include<ranges>
template<typename T>
std::vector<int>shift_count(std::vector<T>f,std::vector<T>g){
  assert(f.size()>=g.size());
  int s=ceil_pow2(f.size());
  int n=f.size()-g.size()+1;
  int m=g.size();
  std::vector<int>res(n);
  std::vector<T>z(f);
  z.insert(z.end(),g.begin(),g.end());
  std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
  auto gen=[&z](std::vector<T>f)->std::vector<std::pair<int,int>> {
    std::vector<std::pair<int,int>>res(f.size());
    res.clear();
    for(auto [i,v]:f|std::views::enumerate)res.emplace_back(std::lower_bound(z.begin(),z.end(),v)-z.begin(),i);
    return res;
  };
  csr_array<int>csrf(z.size(),gen(std::move(f))),csrg(z.size(),gen(std::move(g)));
  std::vector<mint998>buf1(s),buf2(s);
  mint998 inv=mint998(s).inv();
  const int cost=s*msb(s)*2;
  for(int i=0;i<(int)z.size();i++)if(csrf[i].size()&&csrg[i].size()){
    if(csrf[i].size()<cost/csrg[i].size()){
      for(int j:csrf[i])for(int k:csrg[i])if(j-k<n){
        if(j>=k)res[j-k]++;
        else break;
      }
    }
    else{
      std::fill(buf1.begin(),buf1.end(),mint998());
      std::fill(buf2.begin(),buf2.end(),mint998());
      for(int j:csrf[i])buf1[j]++;
      for(int j:csrg[i])buf2[m-1-j]++;
      dft(buf1),dft(buf2);
      for(int j=0;j<s;j++)buf1[j]*=buf2[j];
      idft(buf1);
      for(int j=0;j<n;j++)res[j]+=(buf1[m-1+j]*inv).val();
    }
  }
  return res;
}
std::vector<int>shift_count(const std::string&s,const std::string&t){
  std::vector<char>f(s.size()),g(t.size());
  for(int i=0;i<(int)s.size();i++)f[i]=s[i];
  for(int i=0;i<(int)t.size();i++)g[i]=t[i];
  return shift_count(std::move(f),std::move(g));
}