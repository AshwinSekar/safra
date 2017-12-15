/**
 * Implements safras determinization algorithm using the 6 steps detailed in
 * https://swt.informatik.uni-freiburg.de/teaching/WS2015-16/AutomataTheory/Resources/Slides/safradeterminizationalgorithm-seminarslides-adityaoak.pdf
 */

#include "automata/buechi.hpp"
#include "safratree/safra_tree.hpp"
#include "safratree/safra_hash.cpp"

#include <assert.h>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <queue>

namespace safra {

  SafraNode Buechi::copy_and_remove_marks(const SafraNode& node) const {
    // Deep copy and set all marks to false
    SafraNode new_node(node.name, false);
    new_node.label = node.label;
    for (auto child : node.children) {
      new_node.children.push_back(copy_and_remove_marks(child));
    }
    return new_node;
  }

  void Buechi::branch_accept(SafraTree& tree, SafraNode& node) const {
    // If any label has accepting states in it, branch a new child
    for (auto child = node.children.begin(); child != node.children.end(); ++child) {
      branch_accept(tree, *child);
    }

    std::set<int> common;
    std::set_intersection(node.label.begin(), node.label.end(),
        accept_states.begin(), accept_states.end(),
        std::inserter(common, common.begin()));
    if (common.size() > 0) {
      SafraNode new_child(tree.next_name(), false);
      new_child.label = common;
      node.children.push_back(new_child);
    }
  }

  void Buechi::pset_const(SafraNode& node, int letter) const {
    // Transition all the labels
    std::set<int> new_label;
    for (int l : node.label) {
      auto trans_states = transitions.equal_range(std::pair<int, int>(l, letter));
      for (auto state = trans_states.first; state != trans_states.second; ++state) {
        new_label.insert(state->second);
      }
    }
    node.label = new_label;

    for (auto child = node.children.begin(); child != node.children.end(); ++child)
      pset_const(*child, letter);
  }

  void Buechi::horizontal_merge(SafraNode& node, std::set<int>& brothers) const {
    // Remove nodes that have been seen by older brothers
    std::set<int> diff;
    std::set_difference(node.label.begin(), node.label.end(),
        brothers.begin(), brothers.end(),
        std::inserter(diff, diff.begin()));
    node.label = diff;

    // The paper seems to indicate that we should reset the set when going to children
    // But this breaks the invariant so we reuse the same one
    for (auto child = node.children.begin(); child != node.children.end(); ++child) {
      horizontal_merge(*child, brothers);
    }

    // Add our label to what has been seen by the older brothers
    brothers.insert(node.label.begin(), node.label.end());
  }

  void Buechi::remove_empty(SafraTree& tree, SafraNode& node) const {
    if (node.label.empty()) {
      // This only happens for the root node
      tree.remove_names(node);
      node.children.clear();
      node.name = -1;
    }

    std::vector<SafraNode> new_children;
    for (auto child = node.children.begin(); child != node.children.end(); ++child) {
      if (child->label.empty()) {
        tree.remove_names(*child);
      } else {
        remove_empty(tree, *child);
        new_children.push_back(*child);
      }
    }

    node.children = new_children;
  }

  void Buechi::vertical_merge(SafraTree& tree, SafraNode& node) const {
    // Check to see if any states are the union of their children,
    // remove if that's the case
    std::set<int> child_union;
    for (auto child : node.children) {
      child_union.insert(child.label.begin(), child.label.end());
    }

    // Here we only have to compare sizes because by is_valid we are
    // guarenteed that the children are subsets
    if (child_union.size() == node.label.size()) {
      // Deep delete all the children
      for (auto child : node.children) {
        tree.remove_names(child);
      }
      node.children.clear();
      node.mark = true;
    } else {
      // We vertical merge the children
      for (auto child = node.children.begin(); child != node.children.end(); ++child) {
        vertical_merge(tree, *child);
      }
    }

  }


  SafraTree Buechi::compute_transition(const SafraTree& tree, int letter) const {
    //assert(tree.is_valid());

    if (tree.root.name == -1)
      return tree;

    // 6 steps
    SafraTree next(num_states);
    next.names = tree.names;

    next.root = copy_and_remove_marks(tree.root);
    branch_accept(next, next.root);
    pset_const(next.root, letter);
    std::set<int> brothers;
    horizontal_merge(next.root, brothers);
    remove_empty(next, next.root);
    vertical_merge(next, next.root);

    //assert(next.is_valid());
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
        } else if (status == -1) {
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
