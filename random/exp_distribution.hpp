#pragma once
#include<cmath>
#include "generator.hpp"
namespace Random{
double exp_distribution(double beta){
  return -std::log(Random::get<double>())/beta;
}
}