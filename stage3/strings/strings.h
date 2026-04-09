// stage3/strings/strings.h
#pragma once
#include <string>
#include <vector>

namespace strings {

std::string to_upper(const std::string& s);
std::string join(const std::vector<std::string>& parts, const std::string& sep);

}  // namespace strings
