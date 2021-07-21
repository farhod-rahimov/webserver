#include "../headers/Parser.hpp"

Parser::Parser() {};

Parser::~Parser() {};

Parser::Parser(const Parser & src) {
    *this = src;
};

Parser & Parser::operator = (const Parser & src) {          // complete it
    if (this != &src) {
    }
    return (*this);
}


