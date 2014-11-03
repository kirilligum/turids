#include "headers.hpp"
#include "get_data.hpp"
#include "build_tree.hpp"
#include "tree_predict.hpp"
#include "zip.h"

//using namespace boost;
using namespace std;
using namespace redi;
//using namespace ro;

int main(int argc, char* argv[]) {
  auto train =get_data(1,1000);
  auto test =get_data(1,1000);
  auto train_target = train.back();train.pop_back();
  vector<vector<tuple<int,double,double,double>>> trees(10);
  for(auto& itree:trees) itree = build_tree(train_target,train);;
  vector<int> test_indices(test.front().size());
  boost::iota(test_indices,0);
  boost::random_shuffle(test_indices);
  for(size_t i=0; i< 10;++i){
    vector<double> sample;
    for(size_t j=0; j<test.size()-1;++j)
      sample.push_back(test[j][test_indices[i]]);
    double preditcion = 0.0;
    cout << "{" << sample << "} ";
    for(auto itree:trees) {
      //preditcion += tree_predicts(sample,itree);
      auto tmp  = tree_predicts(sample,itree);
      //cout << itree ;
      //cout << "(" << tmp << ")";
      //cout << endl;
      preditcion += tmp;
    }
    //cout << endl;
    preditcion/=trees.size();
    cout << preditcion << " - " << test.back()[test_indices[i]];
    cout << "   -=-   " <<i <<  "   --   " << test_indices[i] << endl;
  }
  return 0;
}
