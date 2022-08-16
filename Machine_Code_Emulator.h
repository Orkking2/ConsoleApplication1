#pragma once
#ifndef _NSTD_MACHINE_CODE_EMULATOR_

#include "Defines.h"
#include <iostream>
#include <vector>

_NSTD_BEGIN

class emulator {
	struct Token {
		using storage_t = uchar;
		static enum Type {
			TERMINATE = 0,

			ADD,
			SUB,
			STORE,

			LD = 5,
			JMP,
			JMP_0,
			JMP_POS,

			IN = 901,
			OUT,

			UNKNOWN = 999,
		};
		Token(Type t, storage_t data) : type_(t), data_(data) {}

		Type type_;
		storage_t data_;
	};

public:
	static _STD vector<Token> parse(_STD vector<uint> input) {
		_STD vector<Token> out;
		_NSTD_FOR_I(input.size()) {
			Token::Type t = Token::Type::UNKNOWN;
			Token::storage_t data = input[_I] % 100;
			switch (input[_I] / 100) {
			case 0: if (input[_I] == 0) 
					t = Token::Type::TERMINATE;	break;
			case 1: t = Token::Type::ADD;		break;
			case 2: t = Token::Type::SUB;		break;
			case 3: t = Token::Type::STORE;		break;
			case 5: t = Token::Type::LD;		break;
			case 6: t = Token::Type::JMP;		break;
			case 7: t = Token::Type::JMP_0;		break;
			case 8: t = Token::Type::JMP_POS;	break;
			case 9: if (input[_I] == 901) t = Token::Type::IN; else if (input[_I] == 902) t = Token::Type::OUT; break;
			}
			out.push_back(Token(t, data));
		}
		return out;
	}

	static void run(_STD vector<uint> input) {
		run(parse(input));
	}

	static void run(_STD vector<Token> tokens) {
		tokens.resize(100, Token(Token::Type::TERMINATE, 0));
		Token::storage_t reg(0);
		
		_NSTD_FOR_I(tokens.size()) {
			switch (tokens[_I].type_) {
			case Token::Type::TERMINATE: return;
			case Token::Type::ADD:
				reg += tokens[tokens[_I].data_].data_;
				break;
			case Token::Type::SUB:
				reg -= tokens[tokens[_I].data_].data_;
				break;
			case Token::Type::STORE:
				tokens[tokens[_I].data_].data_ = reg;
				break;
			case Token::Type::LD:
				reg = tokens[_I].data_;
				break;
			case Token::Type::JMP:
				_I = tokens[_I].data_;
				break;
			case Token::Type::JMP_0:
				if (!reg)
					_I = tokens[_I].data_;
				break;
			case Token::Type::JMP_POS:
				if (reg > 0)
					_I = tokens[_I].data_;
				break;
			case Token::Type::IN:
				_STD cout << "\n901 called, please input data: ";
				uint cache;
				_STD cin >> cache;
				reg = static_cast<Token::storage_t>(cache);
				break;
			case Token::Type::OUT:
				_STD cout << "\n902 output: " << static_cast<uint>(reg);
				break;
			}
		}
	}


};




_NSTD_END
#endif // _NSTD_MACHINE_CODE_EMULATOR_