#pragma once
namespace Random{
constexpr unsigned long long to_seed(const char*s){
  unsigned long long h=14695981039346656037ULL;
  while(*s){
    h^=static_cast<unsigned char>(*s++);
    h*=1099511628211ULL;
  }
  return h;
}
constexpr unsigned long long constexpr_random_seed=(to_seed(__TIME__)*0x9e3779b97f4a7c15ULL)^to_seed(__DATE__);
constexpr unsigned long long next_value(unsigned long long n){
  n^=n<<13;
  n^=n>>7;
  n^=n<<17;
  return n;
}
}