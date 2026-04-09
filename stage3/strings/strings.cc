// stage3/strings/strings.cc
#include "stage3/strings/strings.h"
#include <algorithm>
#include <sstream>

namespace strings {

std::string to_upper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string join(const std::vector<std::string>& parts, const std::string& sep) {
    std::ostringstream oss;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) oss << sep;
        oss << parts[i];
    }
    return oss.str();
}

}  // namespace strings
