#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
  
  vector<int>  ec={3,3,2,3,45,534,4,3,999,0,3,9999999};
  sort(ec.begin(),ec.end());
  unique (ec.begin(),ec.end());
  cout<<ec.size()<<endl;
  for(auto e:ec)
  {
    cout<<e<<"=>";

  }
  return 0;
}
