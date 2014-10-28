#include "headers.hpp"

using namespace boost;
using namespace std;
//using namespace ro;

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

double score_regression(size_t split_position, const vector<double>& feature_samples, const vector<double>& targets){
  double low_mean=0, high_mean=0, low_var=0, high_var=0,mean=0,var=0;
  int low_count=0, high_count=0, count=0;
  for(size_t i=0; i<feature_samples.size();++i){
    if(feature_samples[i]<feature_samples[split_position]){
      low_mean+=targets[i];
      ++low_count;
    }else{
      high_mean+=targets[i];
      ++high_count;
    }
  }
  mean = low_mean+high_mean;
  count = low_count+high_count;
  mean /= count;
  low_mean/=low_count;
  high_mean/=high_count;
  for(size_t i=0; i<feature_samples.size();++i){
    if(feature_samples[i]<feature_samples[split_position]){
      low_var= pow(targets[i]-low_mean,2);
    }else{
      high_var= pow(targets[i]-high_mean,2);
    }
    var = pow(targets[i]-mean,2);
  }
  low_var/=low_count;
  high_var/=high_count;
  var/=count;
  return 1 - (low_count*low_var+high_count*high_var)/(count*var);
  //return (var-low_count/count*low_var-high_count/count*high_count)/var;
}

pair<int,double> extra_split(const vector<vector<double>>& samples, const vector<double>& targets){
  random_device rd; mt19937 g(rd());
  uniform_int_distribution<> d_feature(0,samples.size()-1);
  uniform_int_distribution<> d_sample(0,samples.front().size()-1);
  size_t n_splits = 5;
  size_t feature = d_feature(g);
  size_t split_position = d_sample(g);
  vector<int> fs(n_splits); for(auto& i:fs) i=d_feature(g);
  vector<int> sps(n_splits); for(auto& i:sps) i=d_sample(g);
  vector<double> scores;
  for(auto i:boost::combine(fs,sps)){
    scores.push_back(score_regression(get<1>(i),samples[get<0>(i)],targets));
  }
  auto split_index = distance(std::begin(scores),boost::max_element(scores));
  //cout << fs << "  " <<sps << endl;
  //cout << split_index << "  " <<  fs[split_index] << "  " <<samples[fs[split_index]][sps[split_index]] << endl;
  return make_pair(fs[split_index],samples[fs[split_index]][sps[split_index]]);
}

pair<pair<vector<double>,vector<vector<double>>>,pair<vector<double>,vector<vector<double>>>> split_data(const pair<int,double> &split, const pair<vector<double>,vector<vector<double>>> & data){
  pair<vector<double>,vector<vector<double>>> lower,higher;
  lower.second.resize(data.second.size());
  higher.second.resize(data.second.size());
  for(size_t row=0;row<data.first.size();++row){
    if(data.second.at(split.first).at(row)<split.second){
      lower.first.push_back(data.first[row]);
      for(size_t i=0; i<data.second.size();++i)
        lower.second.at(i).push_back(data.second[i][row]);
    } else {
      higher.first.push_back(data.first[row]);
      for(size_t i=0; i<data.second.size();++i)
        higher.second.at(i).push_back(data.second[i][row]);
    }
  }
  //cout << "split sizes: " << lower.second.front().size() << " " << higher.second.front().size() << endl;
  return make_pair(lower,higher);
}

vector<pair<int,double>> make_decision_tree(const vector<double>& targets, const vector<vector<double>> &input_data){
  size_t tree_depth = 2;
  vector<pair<int,double>> splits;
  queue<pair<vector<double>,vector<vector<double>>>> data_to_split;
  data_to_split.push(make_pair(targets,input_data));
  while(!data_to_split.empty()){
    auto data = data_to_split.front(); data_to_split.pop();
    splits.push_back(extra_split(data.second,data.first));
    //cout <<  "last splits   " <<  splits.back().first  << "  " <<splits.back().second<< endl;
    auto lh_data= split_data(splits.back(),data);
    data_to_split.push(lh_data.first);
    data_to_split.push(lh_data.second);
    if(splits.size()==pow(2,tree_depth)-1) break;
  }
  cout << splits << endl;
  return splits;
}

int main(int argc, char* argv[]) {
  auto train =make_data(15,100);
  auto test =make_data(15,100);
  auto dt = make_decision_tree(get<0>(train),get<1>(train));
  return 0;
}
