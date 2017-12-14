/**
 * The Buechi Automata type
 */

#ifndef safra_BECHI_HPP_
#define safra_BECHI_HPP_

#include "automata.hpp"

#include <set>

namespace safra {

  class Buechi : public Automata {
    public:
      // Acceptance criteria (accept states)
      std::set<int> accept_states;

      /**
       * Reads the Buechi Automata from a file
       */
      Buechi(const std::string input_file, bool _use_letters);

      /**
       * Implement superclass methods
       */
      virtual void create_representation(std::ostream& stream);
      friend std::ostream& operator<< (std::ostream& stream, const Buechi& buechi);

  };

}

#endif
