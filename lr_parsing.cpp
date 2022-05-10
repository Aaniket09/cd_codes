#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iomanip>

std::map<char, std::set<std::string>> rule_map;
std::vector<std::set<std::string>> item_sets(20);

void compute_I0();
void make_item_sets(int, char);
std::set<std::string> closure(std::string);
void compute_sets();

int main() {

    std::ifstream in_file;
    in_file.open("input4.txt");
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

        int i{};
        while(i < rhs.length()){
            std::string s{};
            while(i < rhs.length() && rhs[i] != '|'){
                s += rhs[i];
                i++;
            }

            rule_map[non_terminal].insert(s);
            i++;
        }

    }
    in_file.close();

    std::string first_rule;
    first_rule += starting_symbol;
    rule_map['Z'].insert(first_rule);

    compute_I0();
    compute_sets();


    for(int i{}; !item_sets.at(i).empty(); ++i){
        std::cout << "Item set I" << i << ":" << std::endl;
        for(auto j: item_sets.at(i)){
            std::cout << std::setw(8) << "" << j << std::endl;
        }
        std::cout << std::endl;
    }



    return 0;
}

void compute_sets(){
    for(int i{}; i<20; i++){
        for(auto j: item_sets.at(i)) {
            int k{};
            while (j[k] != '.') {
                k++;
            }
            k++;
            make_item_sets(i, j[k]);
        }
    }
}

std::set<std::string> closure(std::string rule){

    std::set<std::string> s;
    s.insert(rule);

    size_t i{};
    while(rule[i] != '.'){
        i++;
    }

    if(i!=rule.length()-1 && rule[0] != rule[i+1]){
        for(auto j: item_sets.at(0)){
            if(j[0] == rule[i+1]) {
                std::string r{};
                r += rule[i + 1];
                r += "->";
                r += j.substr(3, j.length());


                s.insert(r);
                for(const auto& k: closure(r)){
                    s.insert(k);
                }
            }
        }
    }

    return s;
}

void make_item_sets(int n, char c){
    std::set<std::string> set_of_n = item_sets.at(n);
    std::set<std::string> rules;
    std::set<std::string> final;

    for(auto i: set_of_n){
        int j{};
        while(i[j] != '.'){
            j++;
        }

        if(j+1 == i.length()){
            continue;
        }

        if(i[j+1] == c){
            char temp = i[j];
            i[j] = i[j+1];
            i[j+1] = temp;
            rules.insert(i);
        }
    }

    for(const auto& i: rules){
        for(const auto& j: closure(i)){
            final.insert(j);
        }
    }

    std::vector<std::set<std::string>>::iterator it;
    it = std::find(item_sets.begin(), item_sets.end(), final);
    if(it == item_sets.end()) {
        int i{};
        while(!item_sets.at(i).empty()){
            i++;
        }
        item_sets.at(i) = (final);
    }

}

void compute_I0(){
    std::set<std::string> s;
    item_sets.push_back(s);

    for(const auto& i: rule_map){
        for(const auto& j: i.second){
            std::string rule{};
            rule += i.first;
            rule += "->.";
            rule += j;

            item_sets.at(0).insert(rule);
        }
    }
}
