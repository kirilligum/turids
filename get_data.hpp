#include "headers.hpp"

//using namespace boost;
using namespace std;
//using namespace ro;

vector<vector<double>> get_data(size_t col, size_t row) {
  random_device rd;
  mt19937 g(rd());
  normal_distribution<> d(0,0.01);
  vector<vector<double>> v(col+1,vector<double>(row));
  for(auto &i:v) boost::iota(i,0);
  auto t = v.back(); v.pop_back();
  //for(auto &i:v) for(auto &j:i) j=j/i.size();
  for(auto &i:v) for(auto &j:i) j=j/i.size()+d(g);
  for(auto &j:t) j=j*j/t.size();
  v.push_back(t);
  return v;
}

