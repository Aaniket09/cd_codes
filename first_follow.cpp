#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iomanip>

std::map<char, std::string> type_map;
std::map<char, std::string> rule_map;
std::map<char, std::set<char>> first_map;
std::map<char, std::set<char>> follow_map;

std::map<char, std::map<char, std::string>> predictive_table;


void get_first_of(char, char);
void get_follow_of(char);

void print_first_and_follow();

void initialize_table();
void compute_predictive_table();
void print_table();


int main() {
       
    std::ifstream in_file;
    in_file.open("input.txt");
    if(!in_file){
        std::cerr << "Problem with opening file" << std::endl;
        return 1;
    }
    
    std::string line{};
    char starting_symbol{};
    bool got_starting_symbol{false};
    
    while(std::getline(in_file, line)){
        char non_terminal{line[0]};
        std::string rhs = line.substr(3, line.length());
        
        if(!got_starting_symbol){
            starting_symbol = non_terminal;
            got_starting_symbol = true;
        }
        
        rule_map[non_terminal] = rhs;
    }
    in_file.close();
    
    for(auto i: rule_map){
        type_map[i.first] = "non-terminal";
    }
    for(auto i: rule_map){
        for(auto j: i.second){
            if(type_map.find(j) == type_map.end()){
                if(j == 'e'){
                    type_map[j] = "epsilon";
                } else if (j == '|') {
                    continue;
                } else {
                    type_map[j] = "terminal";
                    
                    first_map[j].insert(j);
                }
            }
        }
    }
    
    
    for(auto i: type_map){        
        get_first_of(i.first, i.first);
    }
    
    follow_map[starting_symbol].insert('$');
    get_follow_of(starting_symbol);
    for(auto i: type_map){
        if(i.second == "non-terminal"){
            
            get_follow_of(i.first);
        }
    }
    
    print_first_and_follow();

    initialize_table();
    compute_predictive_table();
    print_table();
    
    std::cout << std::endl;
    
    return 0;
}


void compute_predictive_table(){
    
    for(auto i: rule_map){
        std::vector<std::string> rhs;
        size_t t{};
        while(t < i.second.length()){
            std::string s{};
            while(t < i.second.length() && i.second[t] != '|'){
                s += i.second[t];
                t++;
            }
            
            rhs.push_back(s);
            t++;
        }
    
        for(auto j: rhs){
            std::set<char> temp;
            
            temp = first_map[j[0]];
            if(temp.find('e') != temp.end()){
                temp = follow_map[i.first];
            }
            
            for(auto k: temp){
                predictive_table[i.first][k] = j;
            }
        }
    }
}


void initialize_table(){
    
    for(auto i: type_map){
        if(i.second == "non-terminal"){
            
            for(auto j: type_map){
                if(j.second == "terminal"){
                    
                    predictive_table[i.first][j.first] = "--";
                    
                }
            }
            predictive_table[i.first]['$'] = "--";
            
        }
    }
    
}


void get_first_of(char first_of, char c){

    if(type_map[c] == "terminal" || type_map[c] == "epsilon"){
        first_map[first_of].insert(c);
        return;
    }
    
    std::string rule = rule_map[c];
    size_t i{};
    while(i<rule.length()){
        get_first_of(first_of, rule[i]);
        while(rule[i] != '|' && i<rule.length()){
            i++;
        }
        i++;
    }
}

void get_follow_of(char c){
    
    for(auto rule: rule_map){
        for(size_t i{}; i<rule.second.length(); ++i){
            if(rule.second[i] == c){
                
                if(i+1 == rule.second.length() || rule.second[i+1] == '|'){
                    if(follow_map.find(rule.first) == follow_map.end()){
                        if(rule.first == c){
                            continue;
                        }
                        
                        get_follow_of(rule.first);
                    }
                    
                    for(auto j: follow_map[rule.first]){
                        follow_map[c].insert(j);
                    }
                } else {
                    bool has_e{false};
                    for(auto f: first_map[rule.second[i+1]]){
                        if(f == 'e'){
                            has_e = true;
                            continue;
                        }
                        
                        follow_map[c].insert(f);
                    }
                    
                    if(has_e){
                        
                        if(follow_map.find(rule.first) == follow_map.end()){
                            if(rule.first == c){
                                continue;
                            }
                            
                            get_follow_of(rule.first);
                        }
                        
                        for(auto f: follow_map[rule.first]){
                            follow_map[c].insert(f);
                        }
                    }
                }
            }
        }
    }
}

void print_first_and_follow(){
    std::cout << std::setw(60) << std::setfill('_') << "" << std::endl;
    std::cout << std::setw(45) << std::setfill(' ') << "FIRST & FOLLOW of Non-terminals" << std::endl;
    std::cout << std::setw(60) << std::setfill('_') << "" << std::endl;
    std::cout << std::setw(20) << std::setfill(' ') << std::left << "  Non-terminals" << std::setw(20) << "FIRST"
        << std::setw(20) << "FOLLOW" << std::endl;
    std::cout << std::setw(60) << std::setfill('-') << "" << std::endl;
    
    for(auto i: type_map){
        if(i.second == "non-terminal"){
            std::string first{}, follow{};
            
            for(auto j: first_map[i.first]){
                first += j;

            }

            first = "{ " + first + " }";
            
            for(auto j: follow_map[i.first]){
                follow += j;

            }

            follow = "{ " + follow + " }";
            
            
            std::cout << std::setw(8) << std::setfill(' ') << "" << std::setw(12) << i.first <<  std::setw(20)
                << first << std::setw(20) << follow << std::endl;
            
        }
    }
    
    std::cout << std::setw(60) << std::setfill('_') << "" << std::endl << std::endl << std::endl;
}


void print_table(){

    int total_len{};
    for(auto i: type_map){
        if(i.second == "terminal"){
            total_len++;
        }
    }
    total_len += 2;
    total_len *= 15;
    total_len += 5;

    std::cout << std::setw(total_len) << std::setfill('-') << "" << std::endl;
    std::cout << std::setw(total_len/2 + 12) << std::setfill(' ') << std::right << "PREDICTIVE PARSING TABLE" << std::endl;
    std::cout << std::setw(total_len) << std::setfill('-') << "" << std::endl;
    
    std::cout << std::setw(20) << std::setfill(' ') << "";
    for(auto i: type_map){
        if(i.second == "terminal"){
             std::cout << std::setw(15) << std::left << i.first;
        }
    }
    std::cout << std::setw(15) << std::left << "$" << std::endl;

    std::cout << std::setw(total_len) << std::setfill('-') << "" << std::endl;
    


    for(auto i: predictive_table){
        std::cout << std::setw(10) << std::setfill(' ') << std::right << i.first << std::setw(10) << "";
        for(auto j: i.second){
            if(j.first == '$'){
                continue;
            }
            std::string s{};
            if(j.second != "--"){
                s += i.first;
                s += "->";
            }
            
            std::cout << std::left << std::setw(15) << s + j.second;
        }
        std::string s{};
        if(i.second['$'] != "--"){
            s += i.first;
            s += "->";
        }
        
        std::cout << std::left << std::setw(15) << s + i.second['$'] << std::endl;
    }
    
    std::cout << std::setw(total_len) << std::setfill('-') << "" << std::endl;
}
