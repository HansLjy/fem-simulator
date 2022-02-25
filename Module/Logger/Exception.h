//
// Created by hansljy on 2022/2/15.
//

#ifndef FEM_SIMULATOR_EXCEPTION_H
#define FEM_SIMULATOR_EXCEPTION_H

#include <exception>
#include <string>

class Exception : public std::exception {
public:
	Exception(const std::string& info);
	const char * what() const noexcept override;

private:
	std::string _info;
};

#endif //FEM_SIMULATOR_EXCEPTION_H
