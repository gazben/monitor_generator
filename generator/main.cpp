#include <iostream>
#include <fstream>
#include <cmath>

#include <SyntX/util/languages/ltl.h>
#include <SyntX/util/parser/parser.h>
#include "Generator.h"
#include "ConnectionNormalFormGenerator.h"

using namespace util::parser;
using namespace std;

int main(int argc, char* argv[]) {
  base_rule::set_build_ast(true);

  //std::string input = "G (((8 | 9) ^ 4) U (1 & 2))\n";

  std::string input = "G(1 => (2 U 3))";
  base_rule::match_range context(input.cbegin(), input.cend());
  base_rule::match_range result;
  std::shared_ptr<base_rule::node> root;

  if (ltl().match(context, result, root)) {
    std::cout << "Matched: " << std::string(result.first, result.second) << std::endl;

    ast_draw printer(root);
    ast_optimizer::optimize_ast(root);
    printer.to_formatted_string();     std::cout << std::endl << std::endl;
    ConnectionNormalFormGenerator converter;
    converter.convertToConnectionNormalForm(root);
    printer.to_formatted_string();
    printer.draw_to_file(root);

  }
  else {
    std::cout << "Didn't match" << std::endl;
  }

  /*
  if (argc <= 1) {
    cout << "ROS Automatic monitor generator ALFA. Made by Gazder Bence" << endl;
    cout << "Please enter the expression!" << endl;
    getline(std::cin, input);
  }
  else
  {
    input = std::string(argv[1]);
  }
  Generator gen;

  if (gen.Parse(input)) {
    gen.Generate();
    cout << "Generation completed." << endl;
    cout << "Given expression: " + input << endl;
  }

  cout << endl << "Press enter to quit." << endl;
  */
  getchar();
  return 0;
}