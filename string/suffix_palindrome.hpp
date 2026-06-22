#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<limits>
template<typename T=char>
struct SuffixPalindrome{
private:
  struct ArithmeticProgression{
    int l,d,n;
    ArithmeticProgression():l(-1),d(-1),n(0){}
    ArithmeticProgression(int l_,int d_,int n_):l(l_),d(d_),n(n_){}
    friend std::ostream &operator<<(std::ostream&os,const ArithmeticProgression&rhs){
      os<<'['<<rhs.l<<','<<rhs.d<<','<<rhs.n<<']';
      return os;
    }
  };
  std::vector<T>s;
  std::vector<std::vector<ArithmeticProgression>>g;
  static constexpr int inf=std::numeric_limits<int>::max()/2;
public:
  SuffixPalindrome():g(1){}
  std::vector<ArithmeticProgression>add(T c){
    std::vector<ArithmeticProgression>g2;
    s.push_back(c);
    int r=-inf;
    for(auto[l,d,n]:g.back()){
      if(l>0&&s[l-1]==c){   
        l--;
        if(l-r!=d){
          g2.emplace_back(l,l-r,1);
          if(n>1){
            g2.emplace_back(l+d,d,n-1);
          }
        }
        else g2.emplace_back(l,d,n);
        r=l+(n-1)*d;
      }
    }
    if((int)s.size()>=2&&s[s.size()-2]==s[s.size()-1]){
      g2.emplace_back(s.size()-2,s.size()-2-r,1);
      r=s.size()-2;
    }
    g2.emplace_back(s.size()-1,s.size()-1-r,1);
    g.emplace_back();
    auto [i1,d1,n1]=g2.front();
    g2.erase(g2.begin());
    for(const auto&[l,d,n]:g2){
      if(d==d1)n1=n1+n;
      else{
        g.back().emplace_back(i1,d1,n1);
        i1=l,d1=d,n1=n;
      }
    }
    g.back().emplace_back(i1,d1,n1);
    return g.back();
  }
  inline void undo(){
    s.pop_back();
    g.pop_back();
    pl.pop_back();
    gpl.pop_back();
  }
};