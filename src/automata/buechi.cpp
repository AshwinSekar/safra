#include "automata.hpp"
#include "buechi.hpp"

#include <fstream>
#include <cstring>

namespace safra {

  std::string read_line_ignore_comments(std::ifstream& in) {
    std::string line;
    while(getline(in, line)) {
      if (line[0] != '#') {
        return line;
      }
    }
    return NULL;
  }

  Buechi::Buechi(const std::string input_file, bool _use_letters) {
    use_letters = _use_letters;
    std::ifstream in(input_file);
    std::string cur_line;

    // The format of input files is as follows:
    // TODO: add checks for if this fails
    // Header (BUECHI)
    read_line_ignore_comments(in);
    // Number of states
    num_states = std::stoi(read_line_ignore_comments(in));
    // Alphabet size
    alphabet_size = std::stoi(read_line_ignore_comments(in));
    // Number of transitiions
    // NOTE: 1 indexed
    // state * letter -> state
    num_transitions = std::stoi(read_line_ignore_comments(in));
    for (int i = 0; i < num_transitions; i++) {
      std::string transition_line = read_line_ignore_comments(in);
      char* line_tok = strtok(strdup(transition_line.c_str()), " ");

      int from_state = atoi(line_tok) - 1;
      line_tok = strtok(NULL, " ");
      int letter = atoi(line_tok) - 1;
      line_tok = strtok(NULL, " ");
      int to_state = atoi(line_tok) - 1;

      transitions.emplace(std::pair<int, int>(from_state, letter), to_state);
    }
    // Initial (start states)
    // NOTE: 1 indexed
    std::string initial_line = read_line_ignore_comments(in);
    char* line_tok = strtok(strdup(initial_line.c_str()), " ");
    while(line_tok) {
      start_states.emplace(atoi(line_tok) - 1);
      line_tok = strtok(NULL, " ");
    }
    // Final (accept states)
    // NOTE: 1 indexed
    std::string final_line = read_line_ignore_comments(in);
    line_tok = strtok(strdup(initial_line.c_str()), " ");
    while(line_tok) {
      accept_states.emplace(atoi(line_tok) - 1);
      line_tok = strtok(NULL, " ");
    }

  }

  void Buechi::create_representation(std::ostream& stream) {
    for (int i : accept_states) {
      stream << "    q" << i << " [peripheries = 2];" << std::endl;
    }
    Automata::create_representation(stream);
  }

  std::ostream& operator<< (std::ostream& stream, const Buechi& buechi) {
    stream << "Buechi Automata" << std::endl;

    stream << (Automata)buechi;

    stream << "[Accept states]" << std::endl;
    for (int i : buechi.accept_states) {
      stream << "    q" << i << std::endl;
    }

    return stream;
  }

}
