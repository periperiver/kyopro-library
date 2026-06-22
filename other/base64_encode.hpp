#pragma once
#include<vector>
namespace base64_encode_impl{
char itoc(int x){
  if(x<26)return 'A'+x;
  else if(x<52)return 'a'+(x-26);
  else if(x<62)return '0'+(x-52);
  else if(x==62)return '+';
  else if(x==63)return '/';
  else return '\0';
}
template<typename T,int bit_width,typename container_t>
std::vector<bool>bin_encode(const container_t&c){
  std::vector<bool>res;
  res.reserve(c.size()*bit_width);
  int n=c.size();
  for(int i=0;i<n;i++){
    for(int j=0;j<bit_width;j++)res.push_back(c[i]>>j&1);
  }
  return res;
}
template<typename T,int bit_width,typename container_t>
std::string base64_encode(const container_t&c){
  int n=c.size();
  std::string res;
  res.reserve((c.size()*bit_width+5)/6);
  std::vector<bool>bin=bin_encode<T,bit_width,container_t>(c);
  for(int i=0;i<(int)bin.size();i+=6){
    int now=0;
    for(int j=0;j<6;j++)if(i+j<(int)bin.size()&&bin[i+j])now|=1<<j;
    res+=itoc(now);
  }
  return res;
}
}
using base64_encode_impl::base64_encode;