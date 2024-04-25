#include "config.hpp"

GridSize Config::getGridSize() {
    auto rows = table_["grid"]["rows"].value<uint>();
    auto cols = table_["grid"]["cols"].value<uint>();

    if (!rows.has_value() || !cols.has_value()) {
        throw std::runtime_error("Failed to parse grid");
    }

    return GridSize(rows.value(), cols.value());
}

Rule Config::getRule() {
    auto B = table_["rule"]["B"];
    auto S = table_["rule"]["S"];

    if (!B.is_array() || !S.is_array()) {
        throw std::runtime_error("Failed to parse rule");
    }

    std::vector<uint> B_;
    std::vector<uint> S_;

    B.as_array()->for_each([&B_](auto&& el) {
        if constexpr (toml::is_number<decltype(el)>) {
            B_.push_back(*el);
        }
    });

    S.as_array()->for_each([&S_](auto&& el) {
        if constexpr (toml::is_number<decltype(el)>) {
            S_.push_back(*el);
        }
    });

    return Rule(B_, S_);
}

std::vector<bool> Config::getInitialState() {
    auto x = table_["pattern"]["x"].value<uint>();
    auto y = table_["pattern"]["y"].value<uint>();
    auto code = table_["pattern"]["code"].value<std::string>();

    if (!x.has_value() || !y.has_value() || !code.has_value()) {
        throw std::runtime_error("Failed to parse pattern");
    }

    auto gridSize = getGridSize();

    if (x.value() > gridSize.cols || y.value() > gridSize.rows) {
        throw std::runtime_error("Failed to create initial state");
    }

    std::vector<bool> initialState(gridSize.rows * gridSize.cols, false);
    uint idx = (gridSize.rows / 2) - (y.value() / 2);
    uint jdx = (gridSize.cols / 2) - (x.value() / 2);

    auto start = initialState.begin();
    auto it = start + (gridSize.cols * idx) + jdx;
    std::string num;

    for (auto ch : code.value()) {
        if (isdigit(ch)) {
            num += ch;
            continue;
        }

        uint n = 1;

        if (!num.empty()) {
            n = std::stoi(num);
            num.clear();
        }

        if (ch == 'o') {
            it = std::fill_n(it, n, true);
        } else if (ch == 'b') {
            it += n;
        } else if (ch == '$') {
            idx += n;
            it = start + (gridSize.cols * idx) + jdx;
        }
    }

    return initialState;
}
