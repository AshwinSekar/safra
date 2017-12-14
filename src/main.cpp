/**
 * Reads Buechi Automata and create Rabin Automata
 *
 * Takes a variety of output formats, see usage
 */

#include "automata/buechi.hpp"
#include "automata/rabin.hpp"

#include <fstream>

//int main(int argc, char* argv[]) {
int main() {

  /* safra::Buechi buechi("tests/monster5.aut", true);
  std::ofstream out("test.dot");
  buechi.to_graphviz(out);
  buechi.to_pdf("test.pdf");*/
  safra::Rabin rab(true);
  rab.alphabet_size = 3;
  rab.num_states = 4;
  rab.num_transitions = 5;
  rab.transitions.emplace(std::pair<int, int>(0, 0), 0);
  rab.transitions.emplace(std::pair<int, int>(0, 2), 1);
  rab.transitions.emplace(std::pair<int, int>(1, 1), 2);
  rab.transitions.emplace(std::pair<int, int>(2, 0), 3);
  rab.transitions.emplace(std::pair<int, int>(3, 2), 0);
  rab.start_states.emplace(0);
  rab.start_states.emplace(1);
  rab.rabin_pairs.emplace(std::pair<std::set<int>, std::set<int>>(std::set<int>({0, 1}), std::set<int>({2, 3})));
  rab.rabin_pairs.emplace(std::pair<std::set<int>, std::set<int>>(std::set<int>({0, 1, 2, 3}), std::set<int>({2, 3})));
  rab.rabin_pairs.emplace(std::pair<std::set<int>, std::set<int>>(std::set<int>({1}), std::set<int>({2, 3})));
  rab.rabin_pairs.emplace(std::pair<std::set<int>, std::set<int>>(std::set<int>({1, 2, 3}), std::set<int>({})));
  std::ofstream out("test.dot");
  rab.to_graphviz(out);
  rab.to_pdf("test.pdf");

}
