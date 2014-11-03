#include "headers.hpp"
#include "zip.h"

//using namespace boost;
using namespace std;
using namespace redi;
//using namespace ro;

double tree_predicts(const auto& s, const auto&t){
  //cout << "s " << s << endl;
  double leaf_value = 0.0;
  for(auto it=t.begin();distance(t.begin(),it)<t.size();){
    auto d = distance(t.begin(),it);
    //cout << d<< "   "<< *it << "  "<< endl;
    //cout << "---- pick a branch " << s[get<0>(*it)] << "  " << get<1>(*it) << endl;
    //cout << "{" << s[get<0>(*it)];
    if(s[get<0>(*it)]<get<1>(*it)){
      //cout << "<" << get<1>(*it) << ">" << get<2>(*it) << "}";
      auto jumpit = next(it,d+1);
      if(distance(t.begin(),jumpit)>=t.size()) leaf_value= get<2>(*it);
      it=jumpit;
    } else {
      //cout << ">" << get<1>(*it) << "<" << get<3>(*it) << "}";
      auto jumpit = next(it,d+2);
      if(distance(t.begin(),jumpit)>=t.size()) leaf_value= get<3>(*it);
      it=jumpit;
    }
  }
  return leaf_value;
}
