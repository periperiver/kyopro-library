#pragma once
#include "suffix_tree.hpp"
template<int sigma=26>
struct BasicSubstringStructure{
  struct Block{

  };
  BasicSubstringStructure(){}
  explicit BasicSubstringStructure(std::vector<int>s){
    SuffixTree<sigma> pre_st(s);
    std::reverse(s.begin(),s.end());
    SuffixTree<sigma> suf_st(s);
  }
  explicit BasicSubstringStructure(std::string s):BasicSubstringStructure([](std::string s){
    std::vector<int>res(s.size());
    for(int i=0;i<(int)s.size();i++)res[i]=s[i];
    return res;
  }(std::move(s))){}
};