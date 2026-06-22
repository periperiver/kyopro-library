#pragma once
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<cassert>
using namespace std;
template<typename S,auto op,auto unit>
struct DynamicConnectivity{
private:
  #define get_id(d,u,v) (n2*(d)+(long long)n*u+v)
  #define get_e(d,u) (n*(d)+u)
  // #define e_upd(u) u->ech=(u->left?u->left->ech:0)|u->e|(u->right?u->right->ech:0)
  // #define v_upd(u) u->sum=u->left&&u->right?op(u->left->sum,op(u->v,u->right->sum)):(u->left?op(u->left->sum,u->v):(u->right?op(u->v,u->right->sum):u->v))
  #define e_upd(u) u->update()
  #define v_upd(u) u->update()
  struct node{
    node *left,*right,*par;
    S v,sum;
    int from,to;
    int sz;
    int8_t e,ech;
    node(){}
    node(int u,S x=unit()):left(nullptr),right(nullptr),par(nullptr),v(x),sum(x),from(u),to(u),sz(1),e(0),ech(0){}
    node(int from_,int to_,int):left(nullptr),right(nullptr),par(nullptr),v(unit()),sum(unit()),from(from_),to(to_),sz(0),e((from_<to_)<<1),ech((from_<to_)<<1){}
    inline void update(){
      sum=v;
      sz=from==to;
      ech=e;
      if(left)sum=op(left->sum,sum),sz+=left->sz,ech|=left->ech;
      if(right)sum=op(sum,right->sum),sz+=right->sz,ech|=right->ech;
    }
    void splay(){
      while(this->par){
        node *p=this->par;
        if(!this->par->par){
          if(p->left==this){
            if(this->right)this->right->par=p;
            p->left=this->right;
            this->right=p;
            this->par=nullptr;
            p->par=this;
          }
          else{
            if(this->left)this->left->par=p;
            p->right=this->left;
            this->left=p;
            this->par=nullptr;
            p->par=this;
          }
          p->update();
          this->update();
        }
        else{
          node *pp=p->par;
          if(p->left==this){
            if(pp->left==p){
              if(this->right)this->right->par=p;
              if(p->right)p->right->par=pp;
              p->left=this->right;
              this->right=p;
              pp->left=p->right;
              p->right=pp;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
            else{
              if(this->left)this->left->par=pp;
              if(this->right)this->right->par=p;
              pp->right=this->left;
              p->left=this->right;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              pp->par=this;
              p->par=this;
              this->left=pp;
              this->right=p;
            }
          }
          else{
            if(pp->left==p){
              if(this->left)this->left->par=p;
              if(this->right)this->right->par=pp;
              p->right=this->left;
              pp->left=this->right;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=this;
              this->left=p;
              this->right=pp;
            }
            else{
              if(this->left)this->left->par=p;
              if(p->left)p->left->par=pp;
              p->right=this->left;
              this->left=p;
              pp->right=p->left;
              p->left=pp;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
          }
          pp->update();
          p->update();
          this->update();
        }
      }
      this->update();
    }
  };
  unordered_map<long long,node*>ptr;
  vector<unordered_set<int>>edge;
  int dep,n;
  long long n2;
  void reroot(node *nd){
    nd->splay();
    if(!nd->left)return;
    node *le=nd->left;
    le->par=nullptr;
    nd->left=nullptr;
    nd->update();
    while(le->left)le=le->left;
    le->splay();
    le->left=nd;
    nd->par=le;
    le->update();
    nd->splay();
  }
  node *merge(node *l,node *r){
    if(!l)return r;
    if(!r)return l;
    while(l->right)l=l->right;
    l->splay();
    l->right=r;
    r->par=l;
    l->update();
    return l;
  }
  bool same(int d,int u,int v){
    node *uu=ptr[get_id(d,u,u)];
    node *vv=ptr[get_id(d,v,v)];
    uu->splay();
    vv->splay();
    return uu->par;
  }
  bool link(int d,int u,int v){
    node *uu=ptr[get_id(d,u,u)];
    node *vv=ptr[get_id(d,v,v)];
    uu->splay(),vv->splay();
    if(uu->par)return false;
    node *uv=new node(u,v,0);
    node *vu=new node(v,u,0);
    ptr[get_id(d,u,v)]=uv;
    ptr[get_id(d,v,u)]=vu;
    reroot(uu),reroot(vv);
    uv->left=uu,uu->par=uv;
    uv->right=vv,vv->par=uv;
    uv->par=vu;
    vu->left=uv;
    uv->update();
    vu->update();
    return true;
  }
  bool cut(int d,int u,int v){
    if(ptr.find(get_id(d,u,v))==ptr.end())return false;
    node *uv=ptr[get_id(d,u,v)];
    node *vu=ptr[get_id(d,v,u)];
    uv->splay(),vu->splay();
    node *p=uv->par;
    bool ri=uv==vu->right;
    if(p!=vu){
      ri=p==vu->right;
      p->par=nullptr;
      uv->splay();
    }
    if(uv->left)uv->left->par=nullptr;
    if(uv->right)uv->right->par=nullptr;
    if(ri){
      if(vu->left)vu->left->par=nullptr;
      merge(vu->left,uv->right);
    }
    else{
      if(vu->right)vu->right->par=nullptr;
      merge(uv->left,vu->right);
    }
    ptr.erase(get_id(d,u,v));
    ptr.erase(get_id(d,v,u));
    delete(uv);
    delete(vu);
    return true;
  }
  bool replace(int d,int u,int v){
    for(int i=0;i<d;i++)cut(i,u,v);
    bool res=false;
    for(int i=d;i>=0;i--){
      node *ndu=ptr[get_id(i,u,u)];
      node *ndv=ptr[get_id(i,v,v)];
      ndu->splay();
      ndv->splay();
      if(ndu->sz>ndv->sz)swap(u,v),swap(ndu,ndv);
      while(ndu->ech&2){
        while(((~ndu->e)&2)){
          if(ndu->left&&(ndu->left->ech&2))ndu=ndu->left;
          else ndu=ndu->right;
        }
        assert(ndu->ech&2);
        ndu->splay();
        ndu->e&=1;
        e_upd(ndu);
        link(i+1,ndu->from,ndu->to);
      }
      while(ndu->ech&1){
        while((~ndu->e)&1){
          if(ndu->left&&(ndu->left->ech&1))ndu=ndu->left;
          else ndu=ndu->right;
        }
        ndu->splay();
        int s=ndu->from;
        int is=get_e(i,s);
        for(auto itr=edge[is].begin();itr!=edge[is].end();){
          int t=*itr;
          itr=edge[is].erase(itr);
          edge[get_e(i,t)].erase(s);
          if(edge[get_e(i,t)].empty()){
            node *tt=ptr[get_id(i,t,t)];
            tt->splay();
            tt->e&=2;
            e_upd(tt);
          }
          if(edge[get_e(i,s)].empty()){
            node *ss=ptr[get_id(i,s,s)];
            ss->splay();
            ss->e&=2;
            e_upd(ss);
          }
          if(same(i,s,t)){
            if(edge[get_e(i+1,s)].empty()){
              node *nd=ptr[get_id(i+1,s,s)];
              nd->splay();
              nd->ech|=nd->e|=1;
            }
            if(edge[get_e(i+1,t)].empty()){
              node *nd=ptr[get_id(i+1,t,t)];
              nd->splay();
              nd->ech|=nd->e|=1;
            }
            edge[get_e(i+1,s)].insert(t);
            edge[get_e(i+1,t)].insert(s);
          }
          else{
            for(int j=0;j<=i;j++)link(j,s,t);
            res=true;
            break;
          }
        }
        if(edge[get_e(i,s)].empty()){
          node *ss=ptr[get_id(i,s,s)];
          ss->splay();
          ss->e&=2;
          e_upd(ss);
        }
        if(res)return true;
      }
    }
    return false;
  }
public:
  DynamicConnectivity(int n_):n(n_),dep(1),edge(n_){
    n2=(long long)n*n;
    for(int i=0;i<n;i++){
      node *nd=new node(i);
      ptr[get_id(0,i,i)]=nd;
    }
  }
  bool link(int u,int v){
    if(u==v)return false;
    if(link(0,u,v))return true;
    if(edge[u].empty()){
      node *nd=ptr[get_id(0,u,u)];
      nd->splay();
      nd->ech|=nd->e|=1;
    }
    if(edge[v].empty()){
      node *nd=ptr[get_id(0,v,v)];
      nd->splay();
      nd->ech|=nd->e|=1;
    }
    edge[u].insert(v);
    edge[v].insert(u);
    return true;
  }
  bool cut(int u,int v){
    if(u==v)return false;
    for(int i=0;i<dep;i++){
      edge[get_e(i,u)].erase(v);
      edge[get_e(i,v)].erase(u);
      if(edge[get_e(i,u)].empty()){
        node *nd=ptr[get_id(i,u,u)];
        nd->splay();
        nd->e&=2;
        e_upd(nd);
      }
      if(edge[get_e(i,v)].empty()){
        node *nd=ptr[get_id(i,v,v)];
        nd->splay();
        nd->e&=2;
        e_upd(nd);
      }
    }
    for(int i=dep-1;i>=0;i--){
      if(cut(i,u,v)){
        if(i+1==dep){
          edge.resize(edge.size()+n);
          for(int j=0;j<n;j++){
            ptr[get_id(dep,j,j)]=new node(j);
          }
          dep++;
        }
        return !replace(i,u,v);
      }
    }
    return true;
  }
  bool same(int u,int v){
    node *uu=ptr[get_id(0,u,u)];
    node *vv=ptr[get_id(0,v,v)];
    uu->splay();
    vv->splay();
    return uu->par;
  }
  void set(int u,const S&x){
    node *nd=ptr[get_id(0,u,u)];
    nd->splay();
    nd->v=x;
    v_upd(nd);
  }
  S get(int u){return ptr[get_id(0,u,u)]->v;}
  S sum(int u){
    node *nd=ptr[get_id(0,u,u)];
    nd->splay();
    return nd->sum;
  }
  int size(int u){
    node *nd=ptr[get_id(0,u,u)];
    nd->splay();
    return nd->sz;
  }
  void print(){
    vector<vector<pair<int,int>>>res(dep);
    for(auto [i,p]:ptr){
      int d=i/n2;
      int frm=(i%n2)/n;
      int to=(i%n2)%n;
      res[d].push_back({frm,to});
    }
    for(int i=0;i<dep;i++)sort(res[i].begin(),res[i].end()),debug(i,res[i]);
  }
  void baka(){
    for(int i=0;i<dep*n;i++){
      for(auto p:ptr)p.second->update();
    }
  }
  void ASSERT(){
    for(int i=0;i<edge.size();i++){
      for(auto j:edge[i]){
        if(i==j){
          cerr<<i<<endl;
          exit(1);
        }
      }
    }
    for(auto [i,p]:ptr){
      assert(p->left!=p);
      assert(p->right!=p);
      assert(p->par!=p);
    }
  }
  #undef get_id
  #undef get_e
  #undef e_upd
  #undef v_upd
};