#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>

#include "VMTranslator.h"

using namespace std;



int main(){

VMTranslator a;

cout << a.vm_push("constant", 7);


}