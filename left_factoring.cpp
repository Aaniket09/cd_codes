#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

map<char, string> mp;

void remove_left_factoring(string);

int main() {

	#ifndef ONLINE_JUDGE
	freopen("output.txt", "w", stdout);
	#endif
    
    
    ifstream in_file;
    in_file.open("input.txt");
    if(!in_file){
        cerr << "Problem with opening file" << endl;
        return 1;
    }
    
    string line{};
    vector<string> rules;
    
    while(getline(in_file, line)){
        char non_terminal{line[0]};
        string rhs = line.substr(3, line.length());
        
        mp[non_terminal] = rhs;
        
        rules.push_back(line);
    }
    
    for(auto i: rules){
        remove_left_factoring(i);        
    }
    
    return 0;
}


void remove_left_factoring(string input){
    size_t i{};
    char A = input[i];
    
    vector<string> rhs;
    i+=3;
    
    while(i<input.length()){
        string s{};
        while(input[i] != '|' && i<input.length()){
            s += input[i];
            i++;
        }

        rhs.push_back(s);
        i++;
    }
    
    sort(rhs.begin(), rhs.end());
    
    string alpha{};
    for(size_t i{}; i<rhs.size()-1; ++i){
        if(rhs.at(i)[0] == rhs.at(i+1)[0]){
            string s{};
            for(size_t j{}; j<rhs.at(i).size() && j<rhs.at(i+1).size(); ++j){
                if(rhs.at(i)[j] == rhs.at(i+1)[j]){
                    s += rhs.at(i)[j];
                }
            }
            
            if(alpha.length() > s.length()){
                alpha = s;
            }
        }
    }
    
    if(alpha.length() == 0){
        cout << input << endl;
        return;
    }
    
    string rule1_rhs{}, rule2_rhs{};
    bool add_e{false};
    char dash{'Z'};
    
    while(mp.find(dash) != mp.end()){
        dash = (char)((int)dash - 1);
    }

    
    rule1_rhs = alpha;
    rule1_rhs += dash;
    
    for(auto i: rhs){
        if(i.find(alpha) != string::npos){
            if(alpha == i){
                add_e = true;
            } else if (i[0] == alpha[0]) {
                string beta = i.substr(alpha.length(), i.length());
                rule2_rhs += "|" + beta;
            }
        } else {
            rule1_rhs += "|" + i;
        }
    }
    rule2_rhs = rule2_rhs.substr(1, rule2_rhs.length());
    if(add_e){
        rule2_rhs += "|e";
    }
    

    
    mp[A] = rule1_rhs;

    mp[dash] = rule2_rhs;
    
    string rule1{}, rule2{};
    rule1 += A;
    rule1 += "->" + rule1_rhs;
    rule2 += dash;
    rule2 += "->" + rule2_rhs;
    
    remove_left_factoring(rule1);
    remove_left_factoring(rule2);

}
