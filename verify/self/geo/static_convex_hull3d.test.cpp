#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "geo/static_convex_hull3d.hpp"
#include "random/generator.hpp"
#include "datastructure/unionfind.hpp"
namespace gen{
struct Matrix{
  std::array<std::array<long long,3>,3>a;
  //det!=0
  bool check(){
    long long c=0;
    std::vector<long long>p{0,1,2};
    do{
      long long prod=1;
      for(int i=0;i<3;i++)prod*=a[i][p[i]];
      long long inv=0;
      for(int i=0;i<3;i++)for(int j=i+1;j<3;j++)inv+=p[i]>p[j];
      c+=(inv&1?-prod:prod);
    }while(std::next_permutation(p.begin(),p.end()));
    return c!=0;
  }
  void make(){
    for(int i=0;i<3;i++)for(int j=0;j<3;j++){
      a[i][j]=Random::range(-10,11);
    }
  }
  Matrix(){
    do make();while(!check());
  }
  friend Point3d<long long> operator*(const Matrix&lhs,const Point3d<long long>&rhs){
    Point3d<long long>res;
    res.x=lhs.a[0][0]*rhs.x+lhs.a[0][1]*rhs.y+lhs.a[0][2]*rhs.z;
    res.y=lhs.a[1][0]*rhs.x+lhs.a[1][1]*rhs.y+lhs.a[1][2]*rhs.z;
    res.z=lhs.a[2][0]*rhs.x+lhs.a[2][1]*rhs.y+lhs.a[2][2]*rhs.z;
    return res;
  }
};
constexpr long long max_xyz=500000;
Point3d<long long>single(int lim){
  Point3d<long long>res;
  res.x=Random::range(-lim,lim+1);
  res.y=Random::range(-lim,lim+1);
  res.z=Random::range(-lim,lim+1);
  return res;
}
bool isok(Point3d<long long>p){
  if(std::abs(p.x)>max_xyz)return false;
  if(std::abs(p.y)>max_xyz)return false;
  if(std::abs(p.z)>max_xyz)return false;
  return true;
}
void shift(std::vector<Point3d<long long>>&points){
  long long xmin=max_xyz,xmax=-max_xyz;
  long long ymin=max_xyz,ymax=-max_xyz;
  long long zmin=max_xyz,zmax=-max_xyz;
  for(Point3d<long long>&p:points){
    xmin=std::min(xmin,p.x);
    xmax=std::max(xmax,p.x);
    ymin=std::min(ymin,p.y);
    ymax=std::max(ymax,p.y);
    zmin=std::min(zmin,p.z);
    zmax=std::max(zmax,p.z);
  }
  Point3d<long long>d;
  d.x=Random::range(-xmin-max_xyz,max_xyz-xmax+1);
  d.y=Random::range(-ymin-max_xyz,max_xyz-ymax+1);
  d.z=Random::range(-zmin-max_xyz,max_xyz-zmax+1);
  for(Point3d<long long>&p:points)p+=d;
  assert(std::all_of(points.begin(),points.end(),[&](Point3d<long long>p){return isok(p);}));
}
std::vector<Point3d<long long>>random(int n){
  std::vector<Point3d<long long>>res(n);
  for(int i=0;i<n;i++){
    res[i]=single(max_xyz);
  }
  return res;
}
std::vector<Point3d<long long>>line(int n){
  Matrix coef;
  std::vector<Point3d<long long>>res;
  while((int)res.size()<n){
    Point3d<long long>now=single(max_xyz/10);
    now.y=now.z=0;
    now=coef*now;
    if(isok(now))res.push_back(now);
  }
  shift(res);
  return res;
}
std::vector<Point3d<long long>>plane(int n){
  Matrix coef;
  std::vector<Point3d<long long>>res;
  while((int)res.size()<n){
    Point3d<long long>now=single(max_xyz/10);
    now.z=0;
    now=coef*now;
    if(isok(now))res.push_back(now);
  }
  shift(res);
  return res;
}
std::vector<Point3d<long long>>many_line(int n,int t){
  std::vector<Point3d<long long>>res;
  for(int i=0;i<t;i++){
    std::vector<Point3d<long long>>a=line(n);
    res.insert(res.end(),a.begin(),a.end());
  }
  std::shuffle(res.begin(),res.end(),Random::mt);
  return res;
}
std::vector<Point3d<long long>>many_plane(int n,int t){
  std::vector<Point3d<long long>>res;
  for(int i=0;i<t;i++){
    std::vector<Point3d<long long>>a=plane(n);
    res.insert(res.end(),a.begin(),a.end());
  }
  std::shuffle(res.begin(),res.end(),Random::mt);
  return res;
}
std::vector<Point3d<long long>>parallel(int n){
  std::vector<Point3d<long long>>a;
  Point3d<long long>d;
  do{
    a=plane(n);
    d=single(10);
  }while(std::any_of(a.begin(),a.end(),[&](Point3d<long long>p){return !isok(p+d);}));
  a.insert(a.end(),a.begin(),a.end());
  for(int i=0;i<n;i++)a[i]+=d;
  return a;
}
}
void test(std::vector<Point3d<long long>>points){
  int n=points.size();
  auto cht=static_convex_hull3d<long long,long long>(points);
  if(cht.empty()){//colinear
    for(int i=1;i<(int)points.size();i++){
      for(int j=i+1;j<(int)points.size();j++){
        assert(cross(points[i]-points[0],points[j]-points[0])==Point3d<long long>());
      }
    }
    return;
  }
  UnionFind uf(cht.size());
  for(int i=0;i<(int)cht.size();i++){//adjacent connection
    for(int j=0;j<3;j++){
      int ni=cht[i].es[j];
      assert(0<=ni&&ni<(int)cht.size()&&ni!=i);
      long long v1=cht[i].vs[j],v2=cht[i].vs[(j+1)%3];
      bool ok=true;
      for(int k=0;k<3;k++){
        int u1=cht[ni].vs[k],u2=cht[ni].vs[(k+1)%3];
        if(v1==u2&&v2==u1){
          assert(cht[ni].es[k]==i);
          ok=true;
        }
      }
      uf.merge(i,ni);
    }
  }
  assert(uf.size()==1);
  for(Point3d<long long>p:points){
    for(auto f:cht){
      Point3d<long long>A=points[f.vs[0]],B=points[f.vs[1]],C=points[f.vs[2]];
      assert(dot(cross(B-A,C-A),p-A)<=0);//scalar triple product
    }
  }
}
constexpr int loop=500;
int main(){
  for(int n:{1,2,3,10,30,50,100,200,300,500}){
    for(int i=0;i<loop;i++){
      test(gen::random(n));
      test(gen::line(n));
      test(gen::plane(n));
      test(gen::many_line(n,3));
      test(gen::many_plane(n,3));
      test(gen::parallel(n));
    }
  }
  long long a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}