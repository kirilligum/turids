#include "headers.hpp"

using namespace std;

pair<vector<double>,vector<vector<double>>> make_data(size_t col, size_t row) {
  random_device rd;
  mt19937 g(rd());
  normal_distribution<> d(0,0.1);
  vector<vector<double>> v(col,vector<double>(row));
  for(auto &i:v) boost::iota(i,0);
  vector<double> t(row);
  boost::iota(t,0);
  for(auto &i:v) for(auto &j:i) j=j/i.size()+d(g);
  for(auto &j:t) j=j/t.size();
  return make_pair(t,v);
}

pair<int,double> make_split(const pair<vector<double>,vector<vector<double>>> & data){
  return make_pair(0,0.0);
}

pair<vector<double>,vector<vector<double>>> get_data_split(bool higher, const pair<int,double> &split, const pair<vector<double>,vector<vector<double>>> & data){
  return make_pair(vector<double>(),vector<vector<double>>());
}

pair<vector<int>,vector<double>> make_decision_tree(const vector<double>& target, const vector<vector<double>> &input_data){
  vector<size_t> col_names(input_data.size());
  boost::iota(col_names,0);
  size_t tree_depth = 2;
  vector<pair<int,double>> splits;
  queue<pair<vector<double>,vector<vector<double>>>> data_to_split;
  data_to_split.push(make_pair(target,input_data));
  while(data_to_split.empty()){
    auto data = data_to_split.front(); data_to_split.pop();
    splits.push_back(make_split(data));
    data_to_split.push(get_data_split(0,splits.back(),data));
    data_to_split.push(get_data_split(1,splits.back(),data));
    if(splits.size()==pow(2,tree_depth)-2) break;
  }
  return make_pair(vector<int>(),vector<double>());
}

int main(int argc, char* argv[]) {
  auto train =make_data(5,10);
  auto test =make_data(5,10);
  auto dt = make_decision_tree(get<0>(train),get<1>(train));
  cout << train << endl;
  return 0;
}
