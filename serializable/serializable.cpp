#include <iostream>
#include <map>
#include <optional>
#include <vector>

#include "item.hpp"
#include "operation.hpp"
#include "serializable.hpp"
#include "transaction.hpp"
#include "utils.hpp"

using item_states = std::map<item_id, item_state>;
using conflict_graph = std::map<t_id, std::vector<t_id>>;

namespace {
    bool has_cycle_(const conflict_graph& graph, std::map<t_id, bool>& visited, std::map<t_id, bool>& visiting, const t_id transaction) {
    //std::cout << "Visiting " << transaction << std::endl;
    if (!visited[transaction]) {
    visited[transaction] = true;
    visiting[transaction] = true;
    if (graph.count(transaction) > 0) {
        for (const auto& conflict : graph.at(transaction)) {
        //std::cout << "Visiting child " << conflict << std::endl;
        if (visiting[conflict] || !visited[conflict] && has_cycle_(graph, visited, visiting, conflict)) {
            return true;
        }
    }
    }
    
    }
    visiting[transaction] = false;
    return false;
}

bool has_cycle(const conflict_graph& graph) {

    const uint t_count = graph.size();
    std::map<t_id, bool> visited;
    std::map<t_id, bool> visiting;

    for (const auto& [transaction, conflicts] : graph) {
        if (!visited[transaction] && has_cycle_(graph, visited, visiting, transaction)) {
            return true;
        }
    }
    
    return false;
}

void add_conflict(conflict_graph& conflicts, const t_id src, const t_id dst) {
    if (src != dst) {
        conflicts[src].push_back(dst);
    }
}
}



bool is_serializable(operations_t history) {

  item_states items{};
  conflict_graph conflicts{};

  for (const operation_t &op : history) {
    std::visit(
        overload{[&conflicts, &items](const write_op &op) {
                   // All reads before this operation conflict with the current
                   // write
                   //std::cout << "Last readers of data item " << op.item << ":";
                   for (const t_id reader : items[op.item].last_readers) {
                    add_conflict(conflicts, reader, op.transaction);
                   }
                   //std::cout << std::endl;
                   // The last write before this operation conflicts with the
                   // current write
                   if (items[op.item].last_writer.has_value()) {
                    //std::cout << "Last writer " << items[op.item].last_writer.value() << " on data item " << op.item << " conflicts with current write by T_" << op.transaction << std::endl;
                     const t_id src = items[op.item].last_writer.value();
                     const t_id dst = op.transaction;
                     add_conflict(conflicts, src, dst);
                   }
                   // Update last writer as the current transaction
                   items[op.item].last_writer = op.transaction;

                   // Clear the list of last readers
                   items[op.item].last_readers.clear();
                 },
                 [&conflicts, &items](const read_op &op) {
                   // Add the current transaction to the last readers list
                   items[op.item].last_readers.push_back(op.transaction);
                   // The last write to the data item conflicts with the
                   // current write
                   if (items[op.item].last_writer.has_value()) {
                    //std::cout << "Last writer " << items[op.item].last_writer.value() << " on data item " << op.item << " conflicts with current read by T_" << op.transaction << std::endl;
                    const t_id src = items[op.item].last_writer.value();
                    const t_id dst = op.transaction;
                    add_conflict(conflicts, src, dst);
                   }
                 },
                 [&conflicts, &items](const commit_op &op) {
                   throw "Commit not implemented!";
                 },
                 [&conflicts, &items](const abort_op &op) {
                   throw "Abort not implemented!";
                 }},
        op);
  }
  return !has_cycle(conflicts);
}
