/**
 * Implements safras determinization algorithm using the 6 steps detailed in
 * https://swt.informatik.uni-freiburg.de/teaching/WS2015-16/AutomataTheory/Resources/Slides/safradeterminizationalgorithm-seminarslides-adityaoak.pdf
 */

#include "automata/buechi.hpp"
#include "safratree/safra_tree.hpp"
#include "safratree/safra_hash.cpp"

#include <assert.h>
#include <unordered_map>
#include <queue>

namespace safra {

  SafraTree Buechi::copy_and_remove_marks(const SafraTree& tree) const {
    SafraTree next(num_states);
    return next;
  }

  void Buechi::branch_accept(SafraTree& tree) const {

  }

  void Buechi::pset_const(SafraTree& tree, int letter) const {

  }

  void Buechi::horizontal_merge(SafraTree& tree) const {

  }

  void Buechi::remove_empty(SafraTree& tree) const {

  }

  void Buechi::vertical_merge(SafraTree& tree) const {

  }

  SafraTree Buechi::compute_transition(const SafraTree& tree, int letter) const {
    assert(tree.is_valid());

    if (tree.root.name == -1)
      return tree;

    // 6 steps
    SafraTree next = copy_and_remove_marks(tree);
    branch_accept(next);
    pset_const(next, letter);
    horizontal_merge(next);
    remove_empty(next);
    vertical_merge(next);

    assert(next.is_valid());
    return next;
  }

  Rabin Buechi::to_rabin() const {
    std::unordered_map<SafraTree, int> states;
    std::queue<SafraTree> to_explore;

    SafraTree start(*this);
    states[start] = 0;
    to_explore.push(start);

    Rabin rab(use_letters);
    rab.num_states = 1;
    rab.alphabet_size = alphabet_size;
    rab.start_states.insert(0);

    // Create states and transitions
    while (!to_explore.empty()) {
      SafraTree tree = to_explore.front();
      for (int l = 0; l < alphabet_size; l++) {
        SafraTree next = compute_transition(tree, l);
        if (states.find(next) == states.end()) {
          // New state!
          states[next] = rab.num_states++;
          to_explore.push(next);
        }
        // Add the transition
        rab.transitions.emplace(std::pair<int, int>(states[tree], l), states[next]);
      }
      to_explore.pop();
    }

    // Acceptance criteria
    // Check which states have names 0 to 2n marked / unmarked
    for (int i = 0; i < num_states * 2; i++) {
      rabin_pair pair;
      for (auto state : states) {
        int status = state.first.find_name(i);
        if (status == 1) {
          // Positive condition
          pair.second.insert(state.second);
        } else if (status == 0) {
          // Negative condition
          pair.first.insert(state.second);
        }
      }
      // If not empty insert
      if (pair.second.size() > 0) {
        rab.rabin_pairs.insert(pair);
      }
    }

    return rab;
  }

}
