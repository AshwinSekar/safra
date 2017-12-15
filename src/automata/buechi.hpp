/**
 * The Buechi Automata type
 */

#ifndef safra_BUECHI_HPP_
#define safra_BUECHI_HPP_

#include "automata.hpp"
#include "rabin.hpp"

#include <set>

namespace safra {

  class SafraNode;
  class SafraTree;

  class Buechi : public Automata {
    public:
      // Acceptance criteria (accept states)
      std::set<int> accept_states;

      /**
       * Reads the Buechi Automata from a file
       */
      Buechi(const std::string input_file, bool _use_letters);


      /**
       * Convert to Rabin
       */
      Rabin to_rabin() const;

      /**
       * Implement superclass methods
       */
      virtual void create_representation(std::ostream& stream);
      friend std::ostream& operator<< (std::ostream& stream, const Buechi& buechi);

    private:
      // Helper methods to convert to rabin
      SafraTree compute_transition(const SafraTree& tree, int letter) const;
      // Step 1) Remove marks
      SafraNode copy_and_remove_marks(const SafraNode& tree) const ;
      // Step 2) Branch accepting
      void branch_accept(SafraTree& tree, SafraNode& node) const;
      // Step 3) Powerset construction
      void pset_const(SafraNode& node, int letter) const;
      // Step 4) Horizontal merge
      void horizontal_merge(SafraNode& node, std::set<int>& brothers) const;
      // Step 5) Remove empty
      void remove_empty(SafraTree& tree, SafraNode& node) const;
      // Step 6) Vertical merge
      void vertical_merge(SafraTree& tree, SafraNode& node) const;

  };


}

#endif
