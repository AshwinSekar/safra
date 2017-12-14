#include "automata.hpp"
#include "rabin.hpp"

namespace safra {

  Rabin::Rabin(bool _use_letters) {
    use_letters = _use_letters;
  }

  void Rabin::create_pairs(std::ostream& stream) const {
    stream << "[Rabin pairs] (negative, positive)" << std::endl;
    for (auto pairs : rabin_pairs) {
      stream << "({";
      for (auto state = pairs.first.cbegin(); state != pairs.first.cend(); ++state) {
        stream << "q" << *state;
        if (std::next(state, 1) != pairs.first.cend()) {
          stream << ", ";
        }
      }
      stream << "}, {";
      for (auto state = pairs.second.cbegin(); state != pairs.second.cend(); ++state) {
        stream << "q" << *state;
        if (std::next(state, 1) != pairs.first.cend()) {
          stream << ", ";
        }
      }
      stream << "})" << std::endl;
    }
  }

  void Rabin::create_representation(std::ostream& stream) {
    Automata::create_representation(stream);

    // Add in the pairs
    stream << "    rabin_pairs [shape=none, label=\"";
    create_pairs(stream);
    stream << "    \"]" << std::endl;
  }

  std::ostream& operator<< (std::ostream& stream, const Rabin& rab) {
    stream << "Rabin automata" << std::endl;

    stream << (Automata)rab;

    rab.create_pairs(stream);

    return stream;
  }

}
