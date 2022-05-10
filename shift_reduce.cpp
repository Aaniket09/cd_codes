#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>

std::map<std::string, char> rule_map_rev;

void simulation(char);

int main() {

    std::ifstream in_file;
    in_file.open("input2.txt");
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

        std::vector<std::string> vec;
        for(size_t i{}; i<rhs.length(); ++i){
            std::string rule{};
            while(i<rhs.length() && rhs.at(i) != '|'){
                rule += rhs.at(i);
                i++;
            }

            vec.push_back(rule);
        }
        for(const auto &i: vec){
            rule_map_rev[i] = non_terminal;
        }
    }
    in_file.close();

    simulation(starting_symbol);

    return 0;
}

void simulation(char starting_symbol){

    while(true){
        std::string input{};
        std::cout << "Enter your string to check >>> ";
        std::cin >> input;

        if(input == "exit"){
            std::cout << std::endl;
            break;
        }


        std::string stack{}, rule{}, fin_stack{"$"}, prev_stack{};
        fin_stack += starting_symbol;
        input += "$";
        stack += "$";

        std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;
        std::cout << std::setw(15) << std::setfill(' ') << std::left << "STACK" << std::setw(15) << std::right << "INPUT" << std::left << "   Rules" << std::endl;
        std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;

        std::cout << std::setw(15) << std::setfill(' ') << std::left << stack << std::setw(15) << std::right << input << "   " << rule << std::endl;

        stack += input[0];
        input = input.substr(1, input.length());
        rule = "Shift";
        prev_stack = stack;

        std::cout << std::setw(15) << std::setfill(' ') << std::left << stack << std::setw(15) << std::right << input << "   " << rule << std::endl;

        while(stack.length() != 1){

            std::string applied_rule{};
            bool next_step{false};

            if(stack == fin_stack && input == "$"){
                break;
            }

            for(size_t i{1}; i<stack.length(); ++i){
                std::string temp{};
                temp = stack.substr(stack.length()-i, stack.length());

                if(rule_map_rev.find(temp) != rule_map_rev.end()){
                    stack = stack.substr(0, stack.length()-i);
                    stack += rule_map_rev[temp];

                    applied_rule = "Reduce by ";
                    applied_rule += rule_map_rev[temp];
                    applied_rule += "->" + temp;
                    next_step = true;
                    break;
                }
            }

            if(!next_step && input.length() > 1){
                stack += input[0];
                input = input.substr(1, input.length());
                applied_rule = "Shift";
            }

            if(prev_stack == stack){
                break;
            }


            rule = applied_rule;
            prev_stack = stack;
            std::cout << std::setw(15) << std::setfill(' ') << std::left << stack << std::setw(15) << std::right << input << "   " << rule << std::endl;

        }

        std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;


        if(stack == fin_stack && input == "$"){
            std::cout << "String Accepted" << std::endl << std::endl;
        } else {
            std::cout << "String Rejected" << std::endl << std::endl;
        }
    }

}
