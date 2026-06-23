#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"
#include<iostream>
#include "string/suffix_array.hpp"
#include "string/lcp_array.hpp"
int main(){
  std::string s;
  std::cin>>s;
  std::vector<int>sa=suffix_array(s);
  std::vector<int>lcp=lcp_array(s,sa);
  long long ans=(long long)s.size()*(s.size()+1)/2;
  for(int l:lcp)ans-=l;
  std::cout<<ans<<std::endl;
}