#pragma once
#include<vector>
std::vector<int>square_free_number_sieve(int n){
  std::vector<bool>p(n+1,true);
  for(int i=2;i*i<=n;i++){
    int ii=i*i;
    for(int j=1;j*ii<=n;j++)p[j*ii]=false;
  }
  std::vector<int>res;
  for(int i=1;i<=n;i++)if(p[i])res.push_back(i);
  return res;
}