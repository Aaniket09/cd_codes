#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iomanip>

std::map<char, std::string> type_map;
std::map<char, std::string> rule_map;
std::map<char, std::set<char>> leading_map;
std::map<char, std::set<char>> trailing_map;

void get_leading_of(char);
void get_trailing_of(char);
void print_leading_and_trailing();

int main() {

    std::ifstream in_file;
    in_file.open("input3.txt");
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

    for(const auto& i: rule_map){
        type_map[i.first] = "non-terminal";
    }
    for(const auto& i: rule_map){
        for(auto j: i.second){
            if(type_map.find(j) == type_map.end()){
                if(j == 'e'){
                    type_map[j] = "epsilon";
                } else if (j == '|') {
                    continue;
                } else {
                    type_map[j] = "terminal";
                }
            }
        }
    }


    for(const auto& i: type_map){
        if(i.second == "non-terminal"){
            get_leading_of(i.first);
        }
    }

    for(const auto& i: type_map){
        if(i.second == "non-terminal"){
            get_trailing_of(i.first);
        }
    }
    

    print_leading_and_trailing();

    return 0;
}


void get_leading_of(char c){

    std::vector<std::string> rules;
    std::string rule = rule_map[c];
    for(size_t i{}; i<rule.length(); ++i){
        std::string s{};
        while(i<rule.length() && rule[i] != '|'){
            s += rule[i];
            i++;
        }

        rules.push_back(s);
    }

    for(const auto& i: rules){
        if(i.length() == 1 && i[0] != c && i[0] != c){
            if(type_map[i[0]] == "terminal"){
                leading_map[c].insert(i[0]);
            } else {
                if(leading_map.find(i[0]) == leading_map.end()){
                    get_leading_of(i[0]);
                }

                for(const auto &j: leading_map[i[0]]){
                    leading_map[c].insert(j);
                }
            }
        } else {
            size_t t{};
            while(type_map[i[t]] == "non-terminal"){
                t++;
            }

            leading_map[c].insert(i[t]);
        }
    }
}


void get_trailing_of(char c){

    std::vector<std::string> rules;
    std::string rule = rule_map[c];
    for(size_t i{}; i<rule.length(); ++i){
        std::string s{};
        while(i<rule.length() && rule[i] != '|'){
            s += rule[i];
            i++;
        }

        rules.push_back(s);
    }

    for(const auto& i: rules){
        if(i.length() == 1 && i[0] != c && i[0] != c){
            if(type_map[i[0]] == "terminal"){
                trailing_map[c].insert(i[0]);
            } else {
                if(trailing_map.find(i[0]) == trailing_map.end()){
                    get_trailing_of(i[0]);
                }

                for(const auto &j: trailing_map[i[0]]){
                    trailing_map[c].insert(j);
                }
            }
        } else {
            size_t t{i.length() - 1};
            while(type_map[i[t]] == "non-terminal"){
                t--;
            }

            trailing_map[c].insert(i[t]);
        }
    }
}


void print_leading_and_trailing(){
    std::cout << std::setw(60) << std::setfill('_') << "" << std::endl;
    std::cout << std::setw(45) << std::setfill(' ') << "LEADING & TRAILING of Non-terminals" << std::endl;
    std::cout << std::setw(60) << std::setfill('_') << "" << std::endl;
    std::cout << std::setw(20) << std::setfill(' ') << std::left << "  Non-terminals" << std::setw(20) << "LEADING"
              << std::setw(20) << "TRAILING" << std::endl;
    std::cout << std::setw(60) << std::setfill('-') << "" << std::endl;

    for(auto i: type_map){
        if(i.second == "non-terminal"){
            std::string leading{}, trailing{};

            for(auto j: leading_map[i.first]){
                leading += j;
                leading += ", ";
            }
            leading = leading.substr(0, leading.length()-2);
            leading = "{ " + leading + " }";

            for(auto j: trailing_map[i.first]){
                trailing += j;
                trailing += ", ";
            }
            trailing = trailing.substr(0, trailing.length()-2);
            trailing = "{ " + trailing + " }";


            std::cout << std::setw(8) << std::setfill(' ') << "" << std::setw(12) << i.first <<  std::setw(20)
                      << leading << std::setw(20) << trailing << std::endl;

        }
    }

    std::cout << std::setw(60) << std::setfill('_') << "" << std::endl << std::endl << std::endl;
}
