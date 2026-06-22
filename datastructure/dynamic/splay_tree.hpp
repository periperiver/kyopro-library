#pragma once
#include<vector>
#include<utility>
template<typename M>
struct SplayTree{
  using S=typename M::S;
  using F=typename M::F;
private:
  struct node{
    static node nil;
    node *left,*right,*par;
    int sz;
    S v,sum;
    F lazy;
    bool rev,has_lazy;
    node():left(this),right(this),par(this),sz(0),v(M::e()),sum(M::e()),lazy(M::id()),rev(false),has_lazy(false){}
    node(const S&x):left(&nil),right(&nil),par(&nil),sz(1),v(x),sum(x),lazy(M::id()),rev(false),has_lazy(false){}
    inline void update(){
      sum=M::op(left->sum,M::op(v,right->sum));
      sz=1+left->sz+right->sz;
    }
    inline void push(){
      if(rev){
        left->reverse();
        right->reverse();
        rev=false;
      }
      if(has_lazy){
        left->propagate(lazy);
        right->propagate(lazy);
        lazy=M::id();
        has_lazy=false;
      }
    }
    inline void propagate(const F&f){
      if(this==&nil)return;
      v=M::mapping(f,v,1);
      sum=M::mapping(f,sum,sz);
      lazy=M::composition(f,lazy);
      has_lazy=true;
    }
    inline void reverse(){
      if(this==&nil)return;
      rev^=1;
      std::swap(left,right);
      M::revS(sum);
    }
  };
  void splay(node*&nd,int k){
    node *lnd=&node::nil,*rnd=&node::nil;
    while(k!=nd->left->sz){
      nd->push();
      if(k<nd->left->sz){
        node *ch=nd->left;
        ch->push();
        if(k==ch->left->sz){
          rnd->left=nd;
          nd=ch;
          rnd->left->par=rnd;
          rnd=rnd->left;
        }
        else if(k<ch->left->sz){
          node *lele=ch->left;
          lele->push();
          if(k==lele->left->sz){
            nd->left=ch->right;
            ch->right=nd;
            nd->update();
            rnd->left=ch;
            ch->par=rnd;
            rnd=ch;
            nd=lele;
          }
          else if(k<lele->left->sz){
            node *lelele=lele->left;
            lelele->push();
            if(k==lelele->left->sz){
              nd->left=ch->right;
              ch->right=nd;
              nd->update();
              rnd->left=ch;
              ch->par=rnd;
              lele->par=ch;
              rnd=lele;
              nd=lelele;
            }
            else if(k<lelele->left->sz){
              node *lelelele=lelele->left;
              nd->left=ch->right;
              ch->right=nd;
              nd->update();
              lele->left=lelele->right;
              lelele->right=lele;
              lele->update();
              rnd->left=ch;
              ch->left=lelele;
              ch->par=rnd;
              lelele->par=ch;
              rnd=lelele;
              nd=lelelele;
            }
            else{
              k-=1+lelele->left->sz;
              node *leleleri=lelele->right;
              nd->left=ch->right;
              ch->right=nd;
              nd->update();
              lnd->right=lelele;
              lelele->par=lnd;
              lnd=lelele;
              rnd->left=ch;
              ch->par=rnd;
              lele->par=ch;
              rnd=lele;
              nd=leleleri;
            }
          }
          else{
            k-=1+lele->left->sz;
            node *leleri=lele->right;
            leleri->push();
            if(k==leleri->left->sz){
              nd->left=ch->right;
              ch->right=nd;
              nd->update();
              rnd->left=ch;
              ch->par=rnd;
              rnd=ch;
              lnd->right=lele;
              lele->par=lnd;
              lnd=lele;
              nd=leleri;
            }
            else if(k<leleri->left->sz){
              node *lelerile=leleri->left;
              nd->left=ch->right;
              ch->right=nd;
              nd->update();
              rnd->left=ch;
              ch->left=leleri;
              ch->par=rnd;
              leleri->par=ch;
              rnd=leleri;
              lnd->right=lele;
              lele->par=lnd;
              lnd=lele;
              nd=lelerile;
            }
            else{
              node *leleriri=leleri->right;
              k-=1+leleri->left->sz;
              nd->left=ch->right;
              ch->right=nd;
              nd->update();
              lele->right=leleri->left;
              leleri->left=lele;
              lele->update();
              rnd->left=ch;
              ch->par=rnd;
              rnd=ch;
              lnd->right=leleri;
              leleri->par=lnd;
              lnd=leleri;
              nd=leleriri;
            }
          }
        }
        else{
          k-=1+ch->left->sz;
          node *leri=ch->right;
          leri->push();
          if(k==leri->left->sz){
            lnd->right=ch;
            ch->par=lnd;
            lnd=lnd->right;
            rnd->left=nd;
            nd->par=rnd;
            rnd=rnd->left;
            nd=leri;
          }
          else if(k<leri->left->sz){
            node *lerile=leri->left;
            lerile->push();
            if(k==lerile->left->sz){
              rnd->left=nd;
              nd->left=leri;
              nd->par=rnd;
              leri->par=nd;
              rnd=leri;
              lnd->right=ch;
              ch->par=lnd;
              lnd=ch;
              nd=lerile;
            }
            else if(k<lerile->left->sz){
              node *lerilele=lerile->left;
              leri->left=lerile->right;
              lerile->right=leri;
              leri->update();
              rnd->left=nd;
              nd->left=lerile;
              nd->par=rnd;
              lerile->par=nd;
              rnd=lerile;
              lnd->right=ch;
              ch->par=lnd;
              lnd=ch;
              nd=lerilele;
            }
            else{
              k-=1+lerile->left->sz;
              node *lerileri=lerile->right;
              rnd->left=nd;
              nd->left=leri;
              nd->par=rnd;
              leri->par=nd;
              rnd=leri;
              lnd->right=ch;
              ch->right=lerile;
              ch->par=lnd;
              lerile->par=ch;
              lnd=lerile;
              nd=lerileri;
            }
          }
          else{
            k-=1+leri->left->sz;
            node *leriri=leri->right;
            leriri->push();
            if(k==leriri->left->sz){
              rnd->left=nd;
              nd->par=rnd;
              rnd=nd;
              lnd->right=ch;
              ch->par=lnd;
              leri->par=ch;
              lnd=leri;
              nd=leriri;
            }
            else if(k<leriri->left->sz){
              node *leririle=leriri->left;
              rnd->left=nd;
              nd->left=leriri;
              nd->par=rnd;
              leriri->par=nd;
              rnd=leriri;
              lnd->right=ch;
              ch->par=lnd;
              leri->par=ch;
              lnd=leri;
              nd=leririle;
            }
            else{
              k-=1+leriri->left->sz;
              node *leririri=leriri->right;
              rnd->left=nd;
              nd->par=rnd;
              rnd=nd;
              leri->right=leriri->left;
              leriri->left=leri;
              leri->update();
              lnd->right=ch;
              ch->right=leriri;
              ch->par=lnd;
              leriri->par=ch;
              lnd=leriri;
              nd=leririri;
            }
          }
        }
      }//1
      else{
        k-=1+nd->left->sz;
        node *ch=nd->right;
        ch->push();
        if(k==ch->left->sz){
          lnd->right=nd;
          nd=ch;
          lnd->right->par=lnd;
          lnd=lnd->right;
        }
        else if(k<ch->left->sz){
          node *rile=ch->left;
          rile->push();
          if(k==rile->left->sz){
            lnd->right=nd;
            nd->par=lnd;
            lnd=lnd->right;
            rnd->left=ch;
            ch->par=rnd;
            rnd=rnd->left;
            nd=rile;
          }
          else if(k<rile->left->sz){
            node *rilele=rile->left;
            rilele->push();
            if(k==rilele->left->sz){
              lnd->right=nd;
              nd->par=lnd;
              lnd=nd;
              rnd->left=ch;
              ch->par=rnd;
              rile->par=ch;
              rnd=rile;
              nd=rilele;
            }
            else if(k<rilele->left->sz){
              node *rilelele=rilele->left;
              lnd->right=nd;
              nd->par=lnd;
              lnd=nd;
              rile->left=rilele->right;
              rilele->right=rile;
              rile->update();
              rnd->left=ch;
              ch->left=rilele;
              ch->par=rnd;
              rilele->par=ch;
              rnd=rilele;
              nd=rilelele;
            }
            else{
              k-=1+rilele->left->sz;
              node *rileleri=rilele->right;
              lnd->right=nd;
              nd->right=rilele;
              nd->par=lnd;
              rilele->par=nd;
              lnd=rilele;
              rnd->left=ch;
              ch->par=rnd;
              rile->par=ch;
              rnd=rile;
              nd=rileleri;
            }
          }
          else{
            k-=1+rile->left->sz;
            node *rileri=rile->right;
            rileri->push();
            if(k==rileri->left->sz){
              lnd->right=nd;
              nd->right=rile;
              nd->par=lnd;
              rile->par=nd;
              lnd=rile;
              rnd->left=ch;
              ch->par=rnd;
              rnd=ch;
              nd=rileri;
            }
            else if(k<rileri->left->sz){
              node *rilerile=rileri->left;
              lnd->right=nd;
              nd->right=rile;
              nd->par=lnd;
              rile->par=nd;
              lnd=rile;
              rnd->left=ch;
              ch->left=rileri;
              ch->par=rnd;
              rileri->par=ch;
              rnd=rileri;
              nd=rilerile;
            }
            else{
              k-=1+rileri->left->sz;
              node *rileriri=rileri->right;
              rile->right=rileri->left;
              rileri->left=rile;
              rile->update();
              lnd->right=nd;
              nd->right=rileri;
              nd->par=lnd;
              rileri->par=nd;
              lnd=rileri;
              rnd->left=ch;
              ch->par=rnd;
              rnd=ch;
              nd=rileriri;
            }
          }
        }
        else{
          k-=1+ch->left->sz;
          node *riri=ch->right;
          riri->push();
          if(k==riri->left->sz){
            nd->right=ch->left;
            nd->update();
            ch->left=nd;
            lnd->right=ch;
            ch->par=lnd;
            lnd=ch;
            nd=riri;
          }
          else if(k<riri->left->sz){
            node *ririle=riri->left;
            ririle->push();
            if(k==ririle->left->sz){
              nd->right=ch->left;
              ch->left=nd;
              nd->update();
              lnd->right=ch;
              ch->par=lnd;
              lnd=ch;
              rnd->left=riri;
              riri->par=rnd;
              rnd=riri;
              nd=ririle;
            }
            else if(k<ririle->left->sz){
              node *ririlele=ririle->left;
              nd->right=ch->left;
              ch->left=nd;
              nd->update();
              lnd->right=ch;
              ch->par=lnd;
              lnd=ch;
              riri->left=ririle->right;
              ririle->right=riri;
              riri->update();
              rnd->left=ririle;
              ririle->par=rnd;
              rnd=ririle;
              nd=ririlele;
            }
            else{
              k-=1+ririle->left->sz;
              node *ririleri=ririle->right;
              nd->right=ch->left;
              ch->left=nd;
              nd->update();
              lnd->right=ch;
              ch->right=ririle;
              ch->par=lnd;
              ririle->par=ch;
              lnd=ririle;
              rnd->left=riri;
              riri->par=rnd;
              rnd=riri;
              nd=ririleri;
            }
          }
          else{
            k-=1+riri->left->sz;
            node *ririri=riri->right;
            ririri->push();
            if(k==ririri->left->sz){
              nd->right=ch->left;
              ch->left=nd;
              nd->update();
              lnd->right=ch;
              ch->par=lnd;
              riri->par=ch;
              lnd=riri;
              nd=ririri;
            }
            else if(k<ririri->left->sz){
              node *riririle=ririri->left;
              nd->right=ch->left;
              ch->left=nd;
              nd->update();
              lnd->right=ch;
              ch->par=lnd;
              riri->par=ch;
              lnd=riri;
              rnd->left=ririri;
              ririri->par=rnd;
              rnd=ririri;
              nd=riririle;
            }
            else{
              k-=1+ririri->left->sz;
              node *riririri=ririri->right;
              nd->right=ch->left;
              ch->left=nd;
              nd->update();
              riri->right=ririri->left;
              ririri->left=riri;
              riri->update();
              lnd->right=ch;
              ch->right=ririri;
              ch->par=lnd;
              ririri->par=ch;
              lnd=ririri;
              nd=riririri;
            }
          }
        }
      }
    }
    nd->push();
    lnd->right=nd->left;
    while(lnd!=&node::nil){
      lnd->update();
      lnd=lnd->par;
    }
    rnd->left=nd->right;
    while(rnd!=&node::nil){
      rnd->update();
      rnd=rnd->par;
    }
    nd->left=node::nil.right;
    nd->right=node::nil.left;
    nd->update();
  }
  node* merge(node *l,node *r){
    if(l==&node::nil)return r;
    if(r==&node::nil)return l;
    splay(r,0);
    r->left=l;
    r->update();
    return r;
  }
  node *root;
public:
  SplayTree():root(nullptr){}
  SplayTree(const std::vector<S>&init){
    if(init.empty())root=nullptr;
    else{
      auto dfs=[&](auto self,int l,int r)->node* {
        if(l==r)return &node::nil;
        if(l+1==r)return new node(init[l]);
        int mid=(l+r)>>1;
        node *nd=new node(init[mid]);
        nd->left=self(self,l,mid);
        nd->right=self(self,mid+1,r);
        nd->update();
        return nd;
      };
      root=dfs(dfs,0,init.size());
    }
  }
  void insert(int i,const S&x){
    if(!root)root=new node(x);
    else if(root->sz==i){
      node *nd=new node(x);
      nd->left=root;
      root=nd;
      root->update();
    }
    else{
      splay(root,i);
      node *nd=new node(x);
      nd->left=root->left;
      nd->right=root;
      root->left=&node::nil;
      root->update();
      root=nd;
      root->update();
    }
  }
  void erase(int i){
    splay(root,i);
    node *le=root->left,*ri=root->right;
    delete(root);
    root=merge(le,ri);
  }
  void set(int i,const S&x){
    splay(root,i);
    root->v=x;
    root->update();
  }
  S get(int i){
    splay(root,i);
    return root->v;
  }
  void reverse(int l,int r){
    if(r-l<=1)return;
    if(l==0&&r==root->sz)root->reverse();
    else if(l==0){
      splay(root,r);
      root->left->reverse();
      root->update();
    }
    else if(r==root->sz){
      splay(root,l-1);
      root->right->reverse();
      root->update();
    }
    else{
      splay(root,l-1);
      splay(root->right,r-l);
      root->right->left->reverse();
      root->right->update();
      root->update();
    }
  }
  void apply(int l,int r,const F&f){
    if(l==r)return;
    if(l==0&&r==root->sz)root->propagate(f);
    else if(l==0){
      splay(root,r);
      root->left->propagate(f);
      root->update();
    }
    else if(r==root->sz){
      splay(root,l-1);
      root->right->propagate(f);
      root->update();
    }
    else{
      splay(root,l-1);
      splay(root->right,r-l);
      root->right->left->propagate(f);
      root->right->update();
      root->update();
    }
  }
  S prod(int l,int r){
    if(l==r)return M::e();
    if(l==0&&r==root->sz)return root->sum;
    else if(l==0){
      splay(root,r);
      return root->left->sum;
    }
    else if(r==root->sz){
      splay(root,l-1);
      return root->right->sum;
    }
    else{
      splay(root,l-1);
      splay(root->right,r-l);
      return root->right->left->sum;
    }
  }
  int size()const{return root?root->sz:0;}
};
template<typename M> typename SplayTree<M>::node SplayTree<M>::node::nil;