//
// Created by hansljy on 2022/2/15.
//

#ifndef FEM_SIMULATOR_LOGGER_H
#define FEM_SIMULATOR_LOGGER_H

#include <string>
using std::string;

class Logger {
public:
	enum class LogType {
		kWarning,
		kError,
		kInfo
	};

	static const Logger* GetLogger();
	void log(LogType type, const string& info) const;

private:
	static Logger* _theLogger;
};

#endif //FEM_SIMULATOR_LOGGER_H
