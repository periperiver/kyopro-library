#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group"
#include "fastio.hpp"
#include "datastructure/potential_unionfind.hpp"
#include "matrix/constant.hpp"
#include "math/modint.hpp"
using mint=mint998;
using Matrix=ConstantMatrix<mint,2>;
struct M{
  using S=Matrix;
  static S op(const S&lhs,const S&rhs){return lhs*rhs;}
  static S inverse(S mat){
    std::swap(mat[0][0],mat[1][1]);
    mat[0][1]=-mat[0][1];
    mat[1][0]=-mat[1][0];
    return mat;
  }
  static S e(){return Matrix::E();}
};
int main(){
  int n,q;
  rd(n),rd(q);
  PotentialUnionFind<M>uf(n);
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int u,v;
      rd(u),rd(v);
      int a,b,c,d;
      rd(a),rd(b),rd(c),rd(d);
      Matrix mat;
      mat[0][0]=mint::raw(a);
      mat[0][1]=mint::raw(b);
      mat[1][0]=mint::raw(c);
      mat[1][1]=mint::raw(d);
      bool valid=true;
      if(uf.same(u,v)){
        Matrix mat2=uf.get(v,u);
        valid=mat==mat2;
      }
      else uf.merge(v,u,mat);
      wt((int)valid),wt('\n');
    }
    else{
      int u,v;
      rd(u),rd(v);
      if(uf.same(u,v)){
        Matrix mat=uf.get(v,u);
        wt(mat[0][0].val()),wt(' '),wt(mat[0][1].val()),wt(' '),wt(mat[1][0].val()),wt(' '),wt(mat[1][1].val()),wt('\n');
      }
      else wt("-1\n");
    }
  }
}