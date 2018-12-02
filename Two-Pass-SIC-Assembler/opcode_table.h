#pragma once
#include <iostream>
#include <map>

using namespace std;

map<string, int> opcode_tab;

void init_opcode_tab()
{
	opcode_tab.insert(pair<string, int>("ADD", 0x18));
	opcode_tab.insert(pair<string, int>("ADDF", 0x58));
	opcode_tab.insert(pair<string, int>("ADDR", 0x90));
	opcode_tab.insert(pair<string, int>("AND", 0x40));
	opcode_tab.insert(pair<string, int>("CLEAR,", 0xb4));
	opcode_tab.insert(pair<string, int>("COMP", 0x28));
	opcode_tab.insert(pair<string, int>("COMPF", 0x88));
	opcode_tab.insert(pair<string, int>("COMPR", 0xa0));
	opcode_tab.insert(pair<string, int>("DIV", 0x24));
	opcode_tab.insert(pair<string, int>("DIVF", 0x64));
	opcode_tab.insert(pair<string, int>("DIVR", 0x9c));
	opcode_tab.insert(pair<string, int>("FIX", 0xc4));
	opcode_tab.insert(pair<string, int>("FLOAT", 0xc0));
	opcode_tab.insert(pair<string, int>("HIO", 0xf4));
	opcode_tab.insert(pair<string, int>("J", 0x3c));
	opcode_tab.insert(pair<string, int>("JEQ", 0x30));
	opcode_tab.insert(pair<string, int>("JGT", 0x34));
	opcode_tab.insert(pair<string, int>("JLT", 0x38));
	opcode_tab.insert(pair<string, int>("JSUB", 0x48));
	opcode_tab.insert(pair<string, int>("LDA", 0x00));
	opcode_tab.insert(pair<string, int>("LDB", 0x68));
	opcode_tab.insert(pair<string, int>("LDCH", 0x50));
	opcode_tab.insert(pair<string, int>("LDF", 0x70));
	opcode_tab.insert(pair<string, int>("LDL", 0x08));
	opcode_tab.insert(pair<string, int>("LDS", 0x6c));
	opcode_tab.insert(pair<string, int>("LDT", 0x74));
	opcode_tab.insert(pair<string, int>("LDX", 0x04));
	opcode_tab.insert(pair<string, int>("LPS", 0xd0));
	opcode_tab.insert(pair<string, int>("MUL", 0x20));
	opcode_tab.insert(pair<string, int>("MULF", 0x60));
	opcode_tab.insert(pair<string, int>("MULR", 0x98));
	opcode_tab.insert(pair<string, int>("NORM", 0xc8));
	opcode_tab.insert(pair<string, int>("OR", 0x44));
	opcode_tab.insert(pair<string, int>("RD", 0xd8));
	opcode_tab.insert(pair<string, int>("RMO", 0xac));
	opcode_tab.insert(pair<string, int>("RSUB", 0x4c));
	opcode_tab.insert(pair<string, int>("SHIFTL", 0xa4));
	opcode_tab.insert(pair<string, int>("SHIFTR", 0xa8));
	opcode_tab.insert(pair<string, int>("SIO", 0xf0));
	opcode_tab.insert(pair<string, int>("SSK", 0xec));
	opcode_tab.insert(pair<string, int>("STA", 0x0c));
	opcode_tab.insert(pair<string, int>("STB", 0x78));
	opcode_tab.insert(pair<string, int>("STCH", 0x54));
	opcode_tab.insert(pair<string, int>("STF", 0x80));
	opcode_tab.insert(pair<string, int>("STI", 0xd4));
	opcode_tab.insert(pair<string, int>("STL", 0x14));
	opcode_tab.insert(pair<string, int>("STS", 0x7c));
	opcode_tab.insert(pair<string, int>("STSW", 0xe8));
	opcode_tab.insert(pair<string, int>("STT", 0x84));
	opcode_tab.insert(pair<string, int>("STX", 0x10));
	opcode_tab.insert(pair<string, int>("SUB", 0x1c));
	opcode_tab.insert(pair<string, int>("SUBF", 0x5c));
	opcode_tab.insert(pair<string, int>("SUBR", 0x94));
	opcode_tab.insert(pair<string, int>("SVC", 0xb0));
	opcode_tab.insert(pair<string, int>("TD", 0xe0));
	opcode_tab.insert(pair<string, int>("TIO", 0xf8));
	opcode_tab.insert(pair<string, int>("TIX", 0x2c));
	opcode_tab.insert(pair<string, int>("TIXR", 0xb8));
	opcode_tab.insert(pair<string, int>("WD", 0xdc));
}

int get_opcode(string opcode)
{
	map<string, int>::iterator iter = opcode_tab.find(opcode);
	if (iter == opcode_tab.end())
	{
		return -1;
	}
	else
	{
		return iter->second;
	}
}