#include <bits/stdc++.h>
using namespace std;

int main(){

    function<int(int, int)> binom;
    binom = [&binom](int n, int k) -> int{
        if(n == k || k == 0)
            return 1;
        return binom(n - 1, k - 1) + binom(n - 1, k);
    };

    vector<pair<int,int>> tests = {
        {1, 1},
        {3, 0},
        {10, 4},
        {4, -5}
    };

    for(auto &v : tests){
        cout << binom(v.first, v.second) << endl;
    }
}
