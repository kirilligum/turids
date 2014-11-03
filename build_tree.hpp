#include "headers.hpp"

//using namespace boost;
using namespace std;
//using namespace ro;

tuple<double,double,double> score_regression(size_t split_position, const vector<double>& feature_samples, const vector<double>& targets){
  double low_mean=0, high_mean=0, low_var=0, high_var=0,mean=0,var=0;
  int low_count=0, high_count=0, count=0;
  for(size_t row=0; row<feature_samples.size();++row){
    if(feature_samples[row]<feature_samples[split_position]){
      low_mean+=targets[row];
      ++low_count;
    }else{
      high_mean+=targets[row];
      ++high_count;
    }
  }
  mean = low_mean+high_mean;
  count = low_count+high_count;
  if(count>0) mean /= count;
  else cout << "error: count is 0\n"; // TODO put a check on non-emptysample size
  if(low_count>0) low_mean/=low_count;
  else low_mean = mean/count;
  if(high_count>0) high_mean/=high_count;
  else high_mean = mean/count;
  for(size_t row=0; row<feature_samples.size();++row){
    if(feature_samples[row]<feature_samples[split_position]){
      low_var+= pow(targets[row]-low_mean,2);
    }else{
      high_var+= pow(targets[row]-high_mean,2);
    }
    var += pow(targets[row]-mean,2);
  }
  if(count>0) var/=count;
  else cout << "error: count is 0\n"; // TODO put a check on non-emptysample size
  if(low_count>0) low_var/=low_count;
  else low_var = var;
  if(high_count>0) high_var/=high_count;
  else high_var = var;
  //if(low_mean>high_mean) {
    //cout << "*"<< low_mean<<"_"<< low_count<<"|"<<high_mean<<"_"<< high_count<<"*";
    //low_mean=0, high_mean=0, low_var=0, high_var=0,mean=0,var=0;
    //low_count=0, high_count=0, count=0;
    //for(size_t row=0; row<feature_samples.size();++row){
      //if(feature_samples[row]<feature_samples[split_position]){
        //low_mean+=targets[row];
        //++low_count;
      //}else{
        //high_mean+=targets[row];
        //++high_count;
      //}
    //}
    //cout <<"("<< low_mean << "_"<<low_count<<"|"<<high_mean<<"_"<<high_count<<")";
  //}
  return make_tuple(1 - (low_count*low_var+high_count*high_var)/(count*var),low_mean,high_mean);
  //return (var-low_count/count*low_var-high_count/count*high_count)/var;
}

tuple<int,double,double,double> extra_split(const vector<vector<double>>& samples, const vector<double>& targets){
  random_device rd; mt19937 g(rd());
  uniform_int_distribution<> d_feature(0,samples.size()-1);
  uniform_int_distribution<> d_sample(0,samples.front().size()-1);
  size_t n_splits = 5;
  vector<int> fs(n_splits); for(auto& i:fs) i=d_feature(g);
  vector<int> sps(n_splits); for(auto& i:sps) i=d_sample(g);
  vector<double> scores,low_means,high_means;
  for(auto i:boost::combine(fs,sps)){
    auto score_and_means = score_regression(get<1>(i),samples[get<0>(i)],targets);
    scores.push_back(get<0>(score_and_means));
    low_means.push_back(get<1>(score_and_means));
    high_means.push_back(get<2>(score_and_means));
  }
  auto split_index = distance(std::begin(scores),boost::max_element(scores));
  //cout << "["<< *boost::max_element(scores)<<"|"<<scores<<"]";
  //cout << fs << "  " <<sps << endl;
  //cout << split_index << "  " <<  fs[split_index] << "  " <<samples[fs[split_index]][sps[split_index]] << endl;
  return make_tuple(fs[split_index],samples[fs[split_index]][sps[split_index]], low_means[split_index],high_means[split_index]);
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
  if(lower.first.size()<2) lower=data;
  if(higher.first.size()<2) higher=data;
  return make_pair(lower,higher);
}

vector<tuple<int,double,double,double>> build_tree(const vector<double>& targets, const vector<vector<double>> &input_data){
  //cout << "t";
  size_t tree_depth = 6;
  vector<tuple<int,double,double,double>> splits;
  queue<pair<vector<double>,vector<vector<double>>>> data_to_split;
  data_to_split.push(make_pair(targets,input_data));
  while(!data_to_split.empty()){
    auto data = data_to_split.front(); data_to_split.pop();
    splits.push_back(extra_split(data.second,data.first));
    //cout <<  "last splits   " <<  splits.back().first  << "  " <<splits.back().second<< endl;
    auto lh_data= split_data(make_pair(get<0>(splits.back()),get<1>(splits.back())),data);
    //cout << lh_data.first.first.size() << "-";
    //cout << lh_data.second.first.size() << "  ";
    data_to_split.push(lh_data.first);
    data_to_split.push(lh_data.second);
    if(splits.size()==pow(2,tree_depth)-1) break;
  }
  //cout << "  ss: " << splits.size() << endl;
  return splits;
}
