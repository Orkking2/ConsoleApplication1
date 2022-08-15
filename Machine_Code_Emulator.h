#pragma once
#ifndef _NSTD_MACHINE_CODE_EMULATOR_

#include "Defines.h"
#include <string>

_NSTD_BEGIN

class emulator {
	struct Token {
		static enum Type {
			DATA = 0,
			COMMAND,
			UNKNOWN,
		};
		Token(Type t, uint data) : type_(t), data_(data) {}

		Type type_;
		uint data_;
	};

public:
	emulator() {}
	emulator(const _STD string& command) {
		construct(command);
	}

	void construct(const _STD string& command) {
		_NSTD_FOR_I(command.length()) {

		}
	}
};




_NSTD_END
#endif // _NSTD_MACHINE_CODE_EMULATOR_