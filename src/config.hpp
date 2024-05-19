#pragma once

#include <toml++/toml.h>
#include <iostream>
#include <string>
#include <vector>

struct GridSize {
    const uint rows;
    const uint cols;

    GridSize(uint rows_, uint cols_) :
        rows(rows_), cols(cols_) {}
};

struct Rule {
    const std::vector<uint> B;
    const std::vector<uint> S;

    Rule(const std::vector<uint>& B_, const std::vector<uint>& S_) :
        B(B_), S(S_) {}
};

struct Pattern {
    uint x;
    uint y;
    std::string code;

    Pattern(const uint x_, const uint y_, const std::string& code_) :
        x(x_), y(y_), code(code_) {}

    Pattern& operator=(const Pattern& pattern);
    friend std::ostream& operator<<(std::ostream& os, const Pattern& pattern);
};

class Config {
private:
    toml::table table_;
public:
    Config(const std::string& path) {
        table_ = toml::parse_file(path);
    }

    GridSize getGridSize();
    Rule getRule();

    std::vector<bool> getInitialState();
    uint getDelay();

    Pattern convertToPattern(const std::vector<bool>& state);
};
