#pragma once

#include "RegConfig.h"
#include <string>

bool ParseRegConfigFromJSON(const std::string& json_str, RegConfig& config);

