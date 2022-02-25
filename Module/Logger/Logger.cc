//
// Created by hansljy on 2022/2/15.
//

#include "Logger.h"
#include <iostream>
using std::cerr;
using std::endl;

Logger* Logger::_theLogger = nullptr;


const Logger *Logger::GetLogger() {
	if (_theLogger == nullptr) {
		_theLogger = new Logger;
	}
	return _theLogger;
}

void Logger::log(LogType type, const string &info) const {
	switch (type) {
		case LogType::kWarning:
			cerr << "[WARN]:\t";
			break;
		case LogType::kError:
			cerr << "[ERR]:\t";
			break;
		case LogType::kInfo:
			cerr << "[INFO]:\t";
			break;
	}
	cerr << info << endl;
}