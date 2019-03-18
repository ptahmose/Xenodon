#include "config/Config.h"

Parser::Parser(std::istream& input):
    input(input),
    line(1),
    column(1) {
}

int Parser::peek() {
    int c = this->input.peek();

    // skip comments
    if (c == '#') {
        while (c != '\n' && c > 0) {
            c = this->input.get();
        }
    }

    return c;
}

int Parser::consume() {
    if (this->input.eof()) {
        throw ParseError(this->error("Unexpected end of file"));
    }

    int c = this->input.get();
    if (c == '\n') {
        ++this->line;
        this->column = 0;
    }

    ++this->column;        

    return c;
}

void Parser::expect(int expected) {
    int actual = this->consume();
    if (actual != expected) {
        throw ParseError(this->fmt_error([expected, actual](auto& ss) {
            ss << "Expected character '" << static_cast<char>(expected)
                << "', found '" << static_cast<char>(actual) << "'";
        }));
    }
    this->consume();
}

void Parser::optws() {
    while (std::isspace(this->peek())) {
        this->consume();
    }
}

void Parser::expectws() {
    int c = this->peek();
    if (!std::isspace(c)) {
        throw ParseError(this->fmt_error([c](auto& ss) {
            ss << "Expected whitespace character, found '" << static_cast<char>(c) << "'";
        }));
    }

    this->optws();
}

std::string Parser::parse_key() {
    int c = this->consume();

    if (!std::isalpha(c)) {
        throw ParseError(this->fmt_error([c](auto& ss) {
            ss << "Expected alphabetic key, found '" << c << '\'';
        }));
    }

    auto key = std::stringstream();
    key << static_cast<char>(c);
    while (std::isalpha(this->peek())) {
        key << static_cast<char>(this->consume());
    }

    return key.str();
}

size_t Parse<size_t>::operator()(Parser& p) {
    int c = p.consume();
    if (c < '0' || c > '9') {
        throw ParseError(p.fmt_error([c](auto& ss) {
            ss << "Expected number value, found '" << c << '\'';
        }));
    }

    size_t value = static_cast<size_t>(c - '0');

    c = p.peek();
    while (c >= '0' && c <= '9') {
        value *= 10;
        value += static_cast<size_t>(c - '0');
        p.consume();
        c = p.peek();
    }

    return value;
}