#pragma once

#include <string>
#include <optional>

namespace formulae {
    template<typename T>
    class Lexer {
    private:
        std::string input;

        size_t length;

        size_t index;

    protected:
        char getChar() const {
            // Return null character if reached end of input.
            if (!this->hasNext()) {
                return '\0';
            }

            // Otherwise, return the corresponding character.
            return this->input[this->index];
        }

        std::string getCharString() const {
            return std::string(1, this->getChar());
        }

        size_t getLength() const {
            return this->length;
        }

        size_t setIndex(size_t index) {
            // Index cannot be negative.
            if (index < 0) {
                index = 0;
            }
                // Keep index within bounds.
            else if (index >= this->length) {
                index = this->length;
            }

            this->index = index;

            return this->index;
        }

        size_t skip(size_t amount = 1) {
            return this->setIndex(this->index + amount);
        }

    public:
        explicit Lexer(std::string input) : input(input), length(input.length()) {
            // Input string must contain at least one character.
            if (!this->length || this->length < 1) {
                throw std::invalid_argument("Input must be a string with one or more character(s)");
            }

            // TODO: Avoid invoking virtual member functions from constructor.
            // Reset the index, setting its initial value.
            this->begin();
        }

        size_t getIndex() const {
            return this->index;
        }

        virtual void begin() {
            this->index = 0;
        }

        /**
         * Whether the index has not reached the input's length.
         */
        virtual bool hasNext() const {
            return this->index < this->length;
        }

        /**
         * Process the next token.
         */
        virtual std::optional<T> tryNext() {
            // No more possible tokens to retrieve.
            if (!this->hasNext()) {
                return std::nullopt;
            }

            // First, ignore all whitespace if applicable.
            this->processWhitespace();

            // No more possible tokens to retrieve.
            if (!this->hasNext()) {
                return std::nullopt;
            }

            // Set the initial Token buffer as Unknown.
            Token token = Token(TokenKind::Unknown, this->getCharAsString(), this->index);

            // Abstract the Token's value for easier access.
            std::string tokenValue = token.getValue();

            // Begin by testing against all simple until a possible match is found.
            for (const auto pair : this->simpleIds) {
                // Test the first letter of the subject to continue.
                if (tokenValue[0] == pair.first[0]) {
                    /**
                     * Produce a Regex instance to match the exact value of the
                     * simple identifier. It is important that the initial value
                     * is escaped of any Regex special characters.
                     */
                    std::regex regex = Util::createPureRegex(pair.first);

                    /**
                     * If the match starts with a letter, modify the regex to force
                     * either whitespace or EOF at the end.
                     */
                    if (std::regex_match(tokenValue, Regex::identifier)) {
                        // Modify the plain regex to meet requirements at the end.
                        regex = std::regex(pair.first + "(?:\\s|$)");
                    }

                    MatchResult matchResult = this->matchExpression(MatchOpts{
                        token,
                        pair.second,
                        regex
                    });

                    if (matchResult.success) {
                        // Reduce the position.
                        // TODO: Causing problems, works when commented HERE.
                        //this.SetPosition(this.Position - token.Value.Length - pair.Key.Length);

                        // Skim the last character off.
                        token = Token(token.getKind(), pair.first, token.getStartPosition());

                        // Return the token, no need to skip its value.
                        return token;
                    }
                }
            }

            // No simple was matched, proceed to test complex.
            for (const auto pair : this->complexIds) {
                MatchResult matchResult = this->matchExpression(MatchOpts{
                    token,
                    pair.second,
                    pair.first,
                    true
                });

                // If it matches, return the token (already modified by the matchExpression function).
                if (matchResult.success) {
                    return token;
                }
            }

            // At this point the token was not identified. Skip over any captured value.
            this->skip(tokenValue.length());

            // Return the default token. The token kind defaults to Unknown.
            return token;
        }

        std::string getInput() const {
            return this->input;
        }

        std::vector<T> scan() {
            // Reset index to avoid carrying over previous information.
            this->begin();

            // TODO: Should be a list, then converted to a vector?
            std::vector<Token> tokens = {};

            std::optional<Token> token;

            while (this->hasNext()) {
                token = this->tryNext();

                // No more tokens to process.
                if (!token.has_value()) {
                    break;
                }

                // Display a warning if the token's type is unknown.
                if (token->getKind() == TokenKind::Unknown) {
                    // TODO: Issue warning instead of plain std::cout.
                    std::cout << "Warning: Unknown token encountered" << std::endl;
                }

                // Append the token to the result.
                tokens.push_back(*token);
            }

            return tokens;
        }
    };
}
