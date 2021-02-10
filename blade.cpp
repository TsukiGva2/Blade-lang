/* i made this code for a youtube series so just
   ignore the "we"s and "our"s
*/

/*  just for some context
	i'm making Blade
	this is the name of the
	language we(or i) will
	be implementing,

	1 - it is functional.
	this means that most of
	the time the programmer
	will be using functions
	to control the program.
	
	2 - no for/while loops
	do stuff with recursion
	instead.

	3 - it has macros
	maybe the hardest thing
	we will be implementing.
*/


/*
	example program:
	[	print "hello"   ]
		  f     arg      
	think about a function call
	as a pair of brackets
	surrounding a name and some
	arguments
            f   arg1
	[ map {print $1} <1,2,3> ]
	   f  f2          arg
	this seems complicated at 
	first, but it's just a
	function that takes a 	
	function

	the "map" function takes
	two arguments, a function
	and a list, and then applies
	the function to each element
	of the element

	map f v
		you can think of it like
		a for loop in python

		for element in v:
			f(element)

------------------------------

	this is our last example 
	program:
		
  [ first [ rest <1,2,3> ] ]
	it is actually simple!
	the key is to read from
	inside-out.

	[ rest <1,2,3> ]
	-- will return: <2,3>
	rest(also called butfirst),
	returns a new list with all
	the elements but the first.
	
	and now we use the return
	value of the last expression
	as our value.
	[ first <2,3> ]
	-- will return: 2
	first is even simpler, it
	simply returns the first
	element of a given list.
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <regex>
#define endl std::endl

 /* if you read the page on
 our side you may know what
 "first" does now, it returns
 an "iterator"(something i 
 dont know) pointing to the
 first element of the vector
 */

template <typename T>
T first(const std::vector<T>& a){
	return *a.begin();
}

 /* well, we now know it is
 a pointer, so we DEREFERENCE
 it, dereferencing is to get
 the value it is pointing to,
 a pointer is written as a
 star and a name, therefore
 its value is acessed by
 reversing this.
 */

std::string read_a_line(std::string p){
	std::string read;
	std::cout << p;
	if (!std::getline(std::cin, read)){
		std::cout << "error reading line\n";
		exit(1); // 1 is an error code
	}
	return read;
}

int max_element(std::vector<int> arr){
	int grtr = 0;
	for (int i : arr) {
		grtr = std::max(i, grtr);
	}
	return grtr;
}

std::string err (std::vector<std::string> dummy){
	return "error";
}

std::string replace_str (std::string str, int start, int end, std::string new_str) {
	std::string result = str.substr(0,start);
	result += new_str;
	result += str.substr(std::min((int)str.size(), end));
	return result;
}

typedef std::string (*FnPtr)(std::vector<std::string>);
std::map<std::string, FnPtr> funcs_map;
std::map<std::string, std::string> usfuns_map;
std::vector<std::string> globl_args;
std::map<std::string, std::string> vars;

std::string lc_eval(std::string expr){
/* 
1. detect the innermost function [x]
2. call that function            [x]
3. return to 1                   [x]
*/
	std::cout << "got: " << expr << endl;
	FnPtr tempfun = err;
	std::string temp;
	std::string ufun;
	bool userfun = false;
    
	//       f a a
	// [ + [ - 9 9] 9 ]
	//   f   arg1  arg2	

	// so, it works like this
	//-         1 1 1     1 1 1
	//  [ [ [ [ ] ] ] [ [ ] ] ]
	//+ 1 1 1 1       1 1   
	//  you do + 1 for the opening parens
	//  and - 1 for the closing ones

	int acc = 0;
	std::map<int,int> depth_index;
	std::vector<int> depths;
	std::vector<std::string> args;

	bool instr = false;
	
	for (int i = 0; i < expr.size(); i++){
		if (expr[i] == '"') instr = !instr;

		if (expr[i] == '[' && !instr) {
			acc += 1;
			depths.push_back(acc);
		}

		else if (expr[i] == ']' && !instr) {
			acc -= 1;
			depths.push_back(acc);
		}

		if (depth_index.find(acc) == depth_index.end())
			depth_index[acc] = i;
	}

	if (instr) return "SYNTAX-ERROR: no matching double quote. err-unfinished-str";

	if (acc != 0){
		return "SYNTAX-ERROR: wrong number of parenthesis. err-unbalanced-parens";
	}

	int maxd_index = depth_index[max_element(depths)];
	int replace_end = 0;

	for (int i = maxd_index+1; i < expr.size(); i++){
		if (expr[i] == ']' && !instr) {
			replace_end = i+1;
			break;
		}

		if (expr[i] == '"' && !instr){
			instr = true;
			continue;
		}

		if (!((expr[i] == ' ' && !instr && expr[i-1] != '"') || (expr[i] == '"' && instr))){ // XXX
			temp += expr[i];
		} else {
			if (!instr){
				std::remove(temp.begin(), temp.end(), ' ');
			}

			if (funcs_map.find(temp)
				!= funcs_map.end() && !instr){
				tempfun = funcs_map[temp];
			} else if (usfuns_map.find(temp) != usfuns_map.end() && !instr){
				ufun = temp;
				userfun = true;
			} else {
				if (vars.find(temp) != vars.end() && !instr) {
					args.push_back(vars[temp]);
				} else if (!temp.empty()){
					args.push_back(temp);
				}
			}

			instr = false;
			temp = "";
		}
	}
	std::remove(temp.begin(), temp.end(), ' ');
	if (funcs_map.find(temp)
		!= funcs_map.end() && !instr){
		tempfun = funcs_map[temp];
	}
	else if (usfuns_map.find(temp) != usfuns_map.end() && !instr){
		ufun = temp;
		userfun = true;
	}
	else {
		if (vars.find(temp) != vars.end() && !instr) {
			args.push_back(vars[temp]);
		}
		else if (!temp.empty()){
			args.push_back(temp);
		}
	}
	std::string result;
	if (!userfun)
		result = tempfun(args);
	else {
		globl_args = args;
		result = lc_eval(usfuns_map[ufun]);
	}
	if (max_element(depths) < 2) return result;
	return lc_eval(replace_str(expr,maxd_index,replace_end,result));
}


std::string add(std::vector<std::string> nums){
	int acc = 0;
	for (std::string i : nums) {
		acc += std::stoi(i);
	}
	return std::to_string(acc);
}

std::string sub(std::vector<std::string> nums){
	int acc = std::stoi(nums[0]);
	std::vector<std::string> numsbf(nums.begin()+1, nums.end());
	for (std::string i : numsbf) {
		acc -= std::stoi(i);
	}
	return std::to_string(acc);
}

std::string mul(std::vector<std::string> nums){
	long int acc = 1;
	for (std::string i : nums) {
		acc *= std::stol(i);
	}
	return std::to_string(acc);
}


std::string div(std::vector<std::string> nums){
	float acc = std::stof(nums[0]);
	std::vector<std::string> numsbf(nums.begin()+1, nums.end());
	for (std::string i : numsbf) {
		if (std::stof(i) != 0){
			acc /= std::stof(i);
		}
		else {
			return "can't divide by 0";
		}
	}
	// solution
	// [/ 6 3 1]
	// (6 / 3) / 1
	return std::to_string(acc);
}

std::string execfun(std::vector<std::string> args){
	// replace all ('s and )'s for ['s and ]'s
	// also replace single quotes for double quotes
	// then finally replace $X's for the right globl_args's index
	// pass the resulting string to lc_eval
	std::string txt = args[0];
	std::regex re("'");
	txt = std::regex_replace(txt,re,"\"");
	re = "\\$([1-9])";
	std::smatch m;
	std::regex sx("");
	std::string temp_ = "";
	while (std::regex_search(txt,m,re)){
		temp_ = "\\$";
		temp_ += m[1];
		sx = temp_;
		txt = std::regex_replace(txt,sx,globl_args[std::stoi(m[1])-1]);
	}
	return lc_eval(txt);
}

std::string makefun(std::vector<std::string> exprs){
	std::string aexpr = *(exprs.begin()+1);
	std::cout << aexpr << endl;
	usfuns_map[*exprs.begin()] = "[_xfun \"" + aexpr + "\"]";
	return usfuns_map[*exprs.begin()];
}

std::string bit_(std::vector<std::string> binds){
	if (binds.size() < 2){
		return "too few arguments for _bit";
	}
	if (binds[1].size() > 1){
		return "this value is to big, use 'bits' or 'let' instead";
	}
	vars[binds[0]] = binds[1];
	return vars[binds[0]];
}

std::string let_(std::vector<std::string> binds){
	if (binds.size() < 2){
		return "too few arguments for _let";
	}
	vars[binds[0]] = binds[1];
	return vars[binds[0]];
}

std::string id_(std::vector<std::string> args){
	return *(args.begin());
}

std::string id_2(std::vector<std::string> args){
	return *(args.begin()+1);
}

std::string cmp_(std::vector<std::string> args){
	if (args.size() < 2){
		return "too few arguments for compare";
	}
	return std::to_string(args[0] == args[1]);
}

std::string _if(std::vector<std::string> args){
		if (args.size() < 3){
			return "too few arguments for if";
		}
        if (std::stoi(args[0])){
          return lc_eval("[_xfun \"" + args[1] + "\"]");
        } else {
          return lc_eval("[_xfun \"" + args[2] + "\"]");
        }
}

std::string _at(std::vector<std::string> args){ // @ -> at -> Append To.
	if (args.size() < 2){
		return "too few arguments for appendTo";
	}
	std::string result = args[0];
	std::vector<std::string> argsbf(args.begin()+1, args.end());
	for (std::string i : argsbf){
		result += i;
	}
	return result;
}

std::string split_(std::vector<std::string> args){
	std::string txt = args[0];
	std::vector<std::string> strs;
	std::remove(args[1].begin(), args[1].end(), ' ');
	char ch = args[1][0];
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

	std::string res = "";

	for (std::string b : strs) {
		res += b + " ";
	}

    return res;
}

int main (int argc, char** argv) {
// first, i created a map
// what the map does is simple,
// so it maps a std::string, i.e "double quoted text"
// to a function pointer(FnPtr).
	std::string input = "a";
	funcs_map = {
				{"+", 		add},
				{"-",  		sub},
				{"*",  		mul},
				{"/",  		div},
				{"_xfun",   execfun},
				{"makefun", makefun},
				{"_bit",    bit_},
				{"let",     let_},
				{"bits",    let_},
				{"id",      id_},
				{"cmp", 	cmp_},
				{"@", 		_at},
				{"appendto",_at},
                {"if", 		_if},
                {"split", 	split_},
                {"id_2", id_2}};
	std::string res = "";
	while (input != "exit") {
		input = read_a_line("blade -|>>> ");
		res = lc_eval(input);
		std::cout << "retrieved: " << res << endl;
	}
	return 0;
}
