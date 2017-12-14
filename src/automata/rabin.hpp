/**
 * The Rabin Automata type
 */

#ifndef safra_RABIN_HPP_
#define safra_RABIN_HPP_

#include "automata.hpp"

#include <vector>
#include <set>

namespace safra {

  // Pair of positive and negative condition, each a set of states
  typedef std::pair<std::set<int>, std::set<int>> rabin_pair;

  class Rabin : public Automata {
    public:
      // The acceptance criteria of Rabin Automata a set of Rabin pairs
      std::set<rabin_pair> rabin_pairs;

      Rabin(bool _use_letters);

      // Outputs the rabin pairs
      void create_pairs(std::ostream& stream) const;

      /**
       * Implement superclass methods
       */
      virtual void create_representation(std::ostream& stream);
      friend std::ostream& operator<< (std::ostream& stream, const Rabin& rab);

  };

}

#endif
