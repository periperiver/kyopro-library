#pragma once
#include "poly_matrix.hpp"
#include "linear_equations.hpp"
#include "../poly/polynomial_root_finding.hpp"
template<typename T>
std::vector<std::pair<T,std::vector<std::vector<T>>>>eigen_values_vectors(std::vector<std::vector<T>>mat){
  if(mat.empty())return {};
  assert(std::all_of(mat.begin(),mat.end(),[&](const std::vector<T>&v){return v.size()==mat.size();}));
  std::vector<std::pair<T,std::vector<std::vector<T>>>>res;
  for(T lamda:polynomial_root_finding(matrix_charpoly(mat))){
    std::vector<std::vector<T>>lhs(mat);
    for(int i=0;i<(int)mat.size();i++)lhs[i][i]-=lamda;
    std::vector<T>rhs(mat.size());
    res.emplace_back(lamda,matrix_linear_equation(lhs,rhs).second);
  }
  return res;
}