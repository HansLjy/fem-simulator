//
// Created by hansljy on 2022/2/15.
//

#include "Exception.h"

Exception::Exception(const std::string &info) : _info(info) {}

const char *Exception::what() const noexcept {
	return _info.c_str();
}