/**
 * The Automata type with helper functions for output
 */

#ifndef safra_AUTOMATA_HPP_
#define safra_AUTOMATA_HPP_

#include <map>
#include <vector>
#include <set>
#include <iostream>

namespace safra {

  // State * Letter -> State
  typedef std::multimap<std::pair<int, int>, int> trans_relation;

  class Automata {
    public:
      int alphabet_size;
      int num_states;
      int num_transitions;
      trans_relation transitions;
      // Possibly multiple start states depending on non-determinism
      std::set<int> start_states;
      bool use_letters; // Use letters as alphabet

      Automata() {}

      /**
       * Print to graphviz format, a popular graphical format
       */
      void to_graphviz(std::ostream& stream);

      /**
       * Converts to pdf format
       */
      void to_pdf(std::string out_file);

      /**
       * This allows subclasses to specify its output format
       * for use with the other output functions
       */
      virtual void create_representation(std::ostream& stream);

      /**
       * Textual representation of the graph
       */
      friend std::ostream& operator<< (std::ostream& stream, const Automata& aut);
  };

}


#endif
