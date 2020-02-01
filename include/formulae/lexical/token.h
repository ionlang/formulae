#pragma once

#include <string>
#include "token_kind.h"

namespace formulae {
    template<typename T>
    class Token {
    private:
        T kind;

        std::string value;

        uint32_t startPosition;

        uint32_t lineNumber;

    public:
        Token(T kind, std::string value, uint32_t startPosition, uint32_t lineNumber)
            : kind(kind), value(value), startPosition(startPosition), lineNumber(lineNumber) {
            //
        }

        TokenKind getKind() const {
            return this->kind;
        }

        std::string getValue() const {
            return this->value;
        }

        uint32_t getStartPosition() const {
            return this->startPosition;
        }

        uint32_t getEndPosition() const {
            return this->startPosition + this->value.length();
        }

        uint32_t getLineNumber() const {
            return this->lineNumber;
        }

        bool Token::operator==(const Token &other) const {
            return this->value == other.value &&
                this->kind == other.kind &&
                this->startPosition == other.startPosition;
        }

        bool Token::operator!=(const Token &other) const {
            return !(*this == other);
        }

        std::ostream &operator<<(std::ostream &stream, Token &token) {
            // TODO: Include line number as well.
            return stream << "Token(" << token.getValue() << ", " << token.getKind() << ", " << token.getStartPosition()
                << "-" << token.getEndPosition() << ")";
        }
    };

    /**
     * An iterable list of tokens.
     */
    typedef Iterable<Token> TokenStream;
}
