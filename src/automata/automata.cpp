#include "automata.hpp"

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

namespace safra {

  static const char remap[] = "abcdefghijklmnopqrstuvwxyz";

  void Automata::to_graphviz(std::ostream& stream) {
    stream << "digraph G{" << std::endl;
    create_representation(stream);
    stream << "}" << std::endl;
  }

  void Automata::to_pdf(const std::string out_file) {
    // Generate dot file
    // TODO: catch if this fails
    std::string temp_filename = "/tmp/safra-" + out_file + ".dot";
    std::ofstream temp_file(temp_filename);
    to_graphviz(temp_file);
    temp_file.close();

    // Convert to pdf
    // TODO: catch if this fails
    std::string command = "dot -Tpdf " + temp_filename + " -o " + out_file;
    system(command.c_str());
    std::remove(temp_filename.c_str());
  }

  void Automata::create_representation(std::ostream& stream) {
    // In the default representation we just create all the states and
    // edge transitions
    for (int i = 0; i < num_states; i++) {
      stream << "    q" << i
        << " [shape = circle, label=<q<SUB>" << i << "</SUB>>];" << std::endl;
    }

    // Start states, create a blank states and attach them
    for (int start_state : start_states) {
      stream << "    start_state"
        << start_state << " [shape=none, label=\"\"];" << std::endl;
      stream << "    start_state"
        << start_state << " -> q" << start_state << std::endl;
    }

    // Create all transition arrows
    for (auto it : transitions) {
      stream << "    q" << it.first.first << " -> q" << it.second;
      stream << " [label=\"";
      if (use_letters) {
        stream << remap[it.first.second];
      } else {
        stream << it.first.second;
      }
      stream << "\"];" << std::endl;
    }

  }


  std::ostream& operator<< (std::ostream& stream, const Automata& aut) {
    stream << "[Alphabet size]   " << aut.alphabet_size << std::endl;
    stream << "[Num states]      " << aut.num_states << std::endl;
    stream << "[Num transitions] " << aut.num_transitions << std::endl;
    stream << "[Start states]    ";
    for (int i : aut.start_states) {
      stream << i << " ";
    }
    stream << std::endl;

    stream << "State * Letter -> State" << std::endl;
    for (auto it : aut.transitions) {
      stream << "q" << it.first.first << " * ";
      if (aut.use_letters) {
        stream << remap[it.first.second];
      } else {
        stream << it.first.second;
      }
      stream  << " -> q" << it.second << std::endl;
    }

    return stream;
  }

}
