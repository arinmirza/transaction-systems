#pragma once

#include <vector>
#include <optional>
#include <string>

#include "transaction.hpp"

using item_id = std::string;

struct item_state {
    std::optional<t_id> last_writer{};
    std::vector<t_id> last_readers{};
};