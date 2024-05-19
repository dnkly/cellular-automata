#include "config.hpp"

std::ostream& operator<<(std::ostream& os, const Pattern& pattern) {
    os  << "[pattern]\n"
        << "x = " << pattern.x << '\n'
        << "y = " << pattern.y << '\n'
        << "code = \"" << pattern.code << "\"\n";

    return os;
}

Pattern& Pattern::operator=(const Pattern& pattern) {
    if (this == &pattern) {
        return *this;
    }

    x = pattern.x;
    y = pattern.y;
    code = pattern.code;

    return *this;
}

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

    if (x.value() + 2 > gridSize.cols || y.value() + 2 > gridSize.rows) {
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

uint Config::getDelay() {
    auto delay = table_["delay"].value<uint>();

    if (!delay.has_value()) {
        throw std::runtime_error("Failed to parse delay");
    }

    return delay.value();
}

Pattern Config::convertToPattern(const std::vector<bool>& state) {
    auto gridSize = getGridSize();

    uint x_min = gridSize.cols - 1;
    uint x_max = 0;
    uint y_min = gridSize.rows - 1;
    uint y_max = 0;

    for (uint i = 0; i < gridSize.rows; ++i) {
        for (uint j = 0; j < gridSize.cols; ++j) {
            uint k = (gridSize.cols * i) + j;

            if (!state[k]) {
                continue;
            }

            if (i < y_min) y_min = i;
            if (i > y_max) y_max = i;

            if (j < x_min) x_min = j;
            if (j > x_max) x_max = j;
        }
    }

    if (x_max < x_min || y_max < y_min) {
        return Pattern(0, 0, "");
    }

    uint x = x_max - x_min + 1;
    uint y = y_max - y_min + 1;

    std::string code;
    uint count = 1;

    for (uint i = y_min; i <= y_max; ++i) {
        bool prev = state[(gridSize.cols * i) + x_min];

        for (uint j = x_min + 1; j <= x_max; ++j) {
            uint k = (gridSize.cols * i) + j;

            if (state[k] == prev) {
                ++count;
                continue;
            }

            if (count > 1) {
                code += std::to_string(count);
            }

            code += prev ? 'o' : 'b';
            prev = state[k];
            count = 1;
        }

        if (prev) {
            if (count > 1) {
                code += std::to_string(count);
            }

            code += 'o';
        }

        code += i == y_max ? '!' : '$';
        count = 1;
    }

    return Pattern(x, y, code);
}
