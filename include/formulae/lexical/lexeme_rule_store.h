#pragma once

#include <vector>
#include <string>
#include <regex>
#include <map>
#include <utility>
#include <formulae/misc/util.h>
#include "lexeme_rule.h"

namespace formulae {
    typedef uint32_t RuleStoreMatch;

    template<typename TFirst, typename TSecond>
    using VectorPair = std::vector<std::pair<TFirst, TSecond>>;

    template<typename T>
    class LexemeRuleStore {
    private:
        std::map<SimpleLexemeRule, T> simpleRules;

        VectorPair<PatternLexemeRule, T> patternRules;

        VectorPair<ComplexLexemeRule, T> complexRules;

    public:
        void registerRule(std::pair<SimpleLexemeRule, T> rulePair) {
            this->simpleRules[rulePair.first] = rulePair.second;
        }

        void registerRule(std::pair<PatternLexemeRule, T> rulePair) {
            this->patternRules.push_back(rulePair);
        }

        void registerRule(std::pair<ComplexLexemeRule, T> rulePair) {
            this->complexRules.push_back(rulePair);
        }

        RuleStoreMatch match(std::string value) {
            // Attempt to match simple rules first.
            for (const &[rule, type] : this->simpleRules) {
                if (Util::stringStartsWith(value, rule)) {
                    return value.length();
                }
            }

            // TODO: Continue matching over other sets.

            // No rule was matched.
            return 0;
        }
    };
}
