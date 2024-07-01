#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <optional>
#include <limits>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <array>
#include <chrono>

#define ANSI_RED		"\033[38;2;255;0;0m"
#define ANSI_RESET		"\033[38;2;255;255;255m"
#define ANSI_GREEN		"\033[38;2;0;255;0m"
#define ANSI_BLUE		"\033[38;2;0;0;255m"
#define ANSI_YELLOW		"\033[38;2;255;255;0m"
#define ANSI_PINK		"\033[38;2;200;100;100m"

#define _NODISCARD		[[nodiscard]]
