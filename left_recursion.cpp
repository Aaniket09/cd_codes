#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
	int n;
	cout << "No of productions: ";
	cin >> n;
	vector<string> productions;
	
	for (int j=0;j<n;j++){
	string input{};
	cin >> input;
	productions.push_back(input);
	}
	
	for(auto s:productions) {
		size_t i{};
		char A = s[i];
		vector<string> alpha;
		vector<string> beta;
		i+=3;
		while(i<s.length()) {
		    string rhs{};
			while(s[i] != '|' && i<s.length()) {
			    rhs += s[i];
			    i++;
		    }
		    if(rhs[0] == A) {
			    rhs = rhs.substr(1, rhs.length());
			    alpha.push_back(rhs);
			} else {
		 	    beta.push_back(rhs);
		    }
		    i++;
	    }
	    if(alpha.size() == 0) {
		    cout << "\nNo alpha present and so there is no Left Recursion in " << s << endl;
	    } else if (beta.size() == 0) {
		    cout << "\nNo beta present and so there is no Left recursion in " << s << endl;
	    } else {
		    string rule1{};
		    rule1 += A;
		    rule1 += "->";
		    for(auto i:beta) {
			    rule1 += i + A + "'" + "|";
		    }
		    rule1 = rule1.substr(0,rule1.length()-1);
		
		    string rule2{};
		    rule2 += A;
		    rule2 += "'->";
		    for(auto i:alpha) {
			    rule2 += i + A + "'" + "|";
		    }
		    rule2 += "e";
		
		    cout << "\nAfter removing left recursion from " << s << " we get: \n";
		    cout << endl << rule1 << endl;
		    cout << rule2 << endl;
	    }
	}
	
	return 0;
}
