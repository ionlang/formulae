#include <formulae/misc/util.h>

namespace formulae {
    bool Util::stringStartsWith(const std::string &subject, const std::string &value) {
        return subject.rfind(value, 0) == 0;
    }
}
