#pragma once

#include <variant>

#include "item.hpp"
#include "transaction.hpp"

struct read_op {
    const t_id transaction;
    const item_id item;
};

struct write_op {
    const t_id transaction;
    const item_id item;
};

struct commit_op {
    const t_id transaction;
};

struct abort_op {
    const t_id transaction;
};

using operation_t = std::variant<read_op, write_op, commit_op, abort_op>;
using operations_t = std::vector<operation_t>;