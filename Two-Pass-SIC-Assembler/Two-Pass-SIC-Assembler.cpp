#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "opcode_table.h"

using namespace std;

// *********************************************************** //

map<std::string, int> sym_tab;
map<std::string, int>::iterator iter;
vector<std::string> errors;

string line;
int loc;
string label;
string opcode;
string operand;

int program_len;
string program_name;;
int str_addr;

// *********************************************************** //

string _dec_to_dex(int dec)
{
	char* dex = new char[30];
	sprintf(dex, "%X", dec);

	return string(dex);
}

string _dec_to_dex(int dec, int count)
{
	char* dex = new char[count];

	string format = "%0"+to_string(count)+"X";
	sprintf(dex, format.c_str(), dec);

	return string(dex);
}

string _get_zero_filled_string(string str, int count)
{
	for (int i = str.length(); i < count; i++)
	{
		str = "0"+str;
	}

	return str;
}

bool _is_symbol(std::string &str)
{
	if (str.empty())
		return false;

	for (int i = 0; i < str.length(); i++)
	{
		if (!isalpha(str[i]))
		{
			return false;
		}
	}
	return true;
}

bool _is_comment_line(std::string str)
{
	if (str[0] == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void _set_error_flag(std::string str, int line)
{
	errors.push_back("loc "+to_string(line)+" : "+str);
}

void _read_source_line(ifstream &in_file)
{
	string token;
	int token_type = 0;
#define LABEL_TYPE 0
#define OPCODE_TYPE 1
#define OPERAND_TYPE 2

	label = "";
	opcode = "";
	operand = "";

	getline(in_file, line);

	for (int i = 0; i < line.length(); i++)
	{
		token = "";

		while (i < line.length() && (line[i] != ' ' && line[i] != '\t'))
		{
			token += line[i];
			i++;
		}

		switch (token_type)
		{
		case LABEL_TYPE:
			if (token[0] == '.')
			{
				// comment line
				return;
			}
			else
			{
				if (token.compare("") == 0)
				{
					label = "";
				}
				else
				{
					label = token;
				}
			}
			break;
		case OPCODE_TYPE:
			opcode = token;
			break;
		case OPERAND_TYPE:
			operand = token;
			break;
		default:
			return;
		}

		token_type++;
	}
}

void _write_intermeidate_line(ofstream &out_file, int loc, bool is_end = false)
{
	static int _loc = loc;
	if(!is_end)
		out_file << _dec_to_dex(_loc) << "\t" << label << "\t" << opcode << "\t" << operand << endl;
	else
		out_file << "\t" << label << "\t" << opcode << "\t" << operand;
	_loc = loc;
}

void sic_assembler_pass1(string src_path)
{
	ifstream in_file(src_path);
	string out_path = src_path.substr(0, src_path.rfind(".")) + "_mid.txt";
	ofstream out_file(out_path);

	int loc_ctr;

	if (in_file.is_open())
	{
		_read_source_line(in_file);

		if (opcode.compare("START") == 0)
		{
			program_name = label;
			str_addr = strtoul(operand.c_str(), NULL, 16);
			loc_ctr = str_addr;
			_write_intermeidate_line(out_file, loc_ctr);
			_read_source_line(in_file);
		}
		else
		{
			loc_ctr = 0;
		}
		
		while (opcode.compare("END") != 0)
		{
			if (!_is_comment_line(line))
			{
				if (_is_symbol(label))
				{
					 iter = sym_tab.find(label);
					 if (iter != sym_tab.end())
					 {
						 _set_error_flag("duplicate flag",loc_ctr);
					 }
					 else
					 {
						 sym_tab.insert(pair<string, int>(label, loc_ctr));
					 }
				}

				int opcode_int = get_opcode(opcode);
				if (opcode_int != -1)
				{
					loc_ctr += 3;
				}
				else if (opcode.compare("WORD") == 0)
				{
					loc_ctr += 3;
				}
				else if (opcode.compare("RESW") == 0)
				{
					loc_ctr += 3 * atoi(operand.c_str());
				}
				else if (opcode.compare("RESB") == 0)
				{
					loc_ctr += atoi(operand.c_str());
				}
				else if (opcode.compare("BYTE") == 0)
				{
					int identify_first_pos = operand.find('\'');
					int identify_second_pos = operand.rfind('\'');

					char const_type = operand.substr(identify_first_pos - 1, 1)[0];

					int len;
					float temp;
					
					switch(const_type)
					{
					case 'C':
					case 'c':
						len = identify_second_pos - identify_first_pos - 1;
						break;
					case 'X':
					case 'x':
						temp = (identify_second_pos - identify_first_pos - 1)/2;
						len =  temp > (int)temp ? (int)temp+1 : (int)temp;
						break;
					}
					loc_ctr += len;
				}
				else
				{
					_set_error_flag("invaild operation code", loc_ctr);
				}
			}

			_write_intermeidate_line(out_file, loc_ctr);
			_read_source_line(in_file);
		}

		_write_intermeidate_line(out_file, loc_ctr, true);
		program_len = loc_ctr - str_addr;
		in_file.close();
		out_file.close();
	}
	else
	{
		std::cout << "Can not open file" << std::endl;
	}
}

void _read_intermediate_line(ifstream &in_file)
{
	string token;
	int token_type = 0;
#define LOCATION_TYPE	0
#define LABEL_TYPE		1
#define OPCODE_TYPE	2
#define OPERAND_TYPE	3

	getline(in_file, line);

	loc = 0;
	label = "";
	opcode = "";
	operand = "";

	for (int i = 0; i < line.length(); i++)
	{
		token = "";

		while (i < line.length() && (line[i] != ' ' && line[i] != '\t'))
		{
			token += line[i];
			i++;
		}

		switch (token_type)
		{
		case LOCATION_TYPE:
			loc = strtoul(token.c_str(), NULL, 16);
			break;
		case LABEL_TYPE:
			if (token[0] == '.')
			{
				// comment line
				return;
			}
			else
			{
				if (token.compare("") == 0)
				{
					label = "";
				}
				else
				{
					label = token;
				}
			}
			break;
		case OPCODE_TYPE:
			opcode = token;
			break;
		case OPERAND_TYPE:
			operand = token;
			break;
		default:
			return;
		}

		token_type++;
	}
}

string _assemble_obj_code(int opcode, int operand)
{
	return _dec_to_dex(opcode, 2) + _dec_to_dex(operand, 4);
}

void _write_listing_line(ofstream &out_file, string object_code)
{
	if (loc != 0)
		out_file << line << "\t" << object_code << endl;
	else
		out_file << line << "\t" << object_code;
}

void _write_object_line(ofstream &out_file, char record_type, int code_size, string code_arr[], int arr_len, int loc = 0)
{
	string program_name;
	switch (record_type)
	{
	case 'H':
		program_name = code_arr[0];

		for (int i = program_name.length(); i < 6; i++)
		{
			program_name += " ";
		}

		out_file << record_type << program_name << _get_zero_filled_string(code_arr[1], 6) << _get_zero_filled_string(code_arr[2], 6) << endl;
		break;
	case 'T':
		out_file << record_type << _dec_to_dex(loc, 6) << _dec_to_dex(code_size, 2);
		for (int i = 0; i < arr_len; i++)
		{
			out_file << code_arr[i];
		}
		out_file << endl;
		break;
	case 'E':
		out_file << record_type << _get_zero_filled_string(code_arr[0], 6);
		break;
	}
}

void _write_object_line(ofstream &out_file, char record_type, vector<string> codes, int loc = 0)
{
	string* code_arr = new string[codes.size()];
	int size = 0;
	for (int i = 0; i < codes.size(); i++)
	{
		code_arr[i] = codes[i];
		size += codes[i].length() / 2;
	}

	_write_object_line(out_file, record_type, size, code_arr, codes.size(), loc);
}

void sic_assembler_pass2(string path)
{
	ifstream in_file(path);
	ofstream obj_file(path.substr(0, path.rfind("_")) + "_obj.txt");
	ofstream listing_file(path.substr(0, path.rfind("_")) + "_list.txt");

	vector<string> codes;
	codes.reserve(10);
	int start_code_addr = -1;

	int operand_addr = 0;
	string object_code;

	if (in_file.is_open())
	{
		_read_intermediate_line(in_file);
		if (opcode.compare("START") == 0)
		{
			_write_listing_line(listing_file, "");
			_read_intermediate_line(in_file);
		}

		_write_object_line(obj_file, 'H', 3, new string[3]{program_name, _dec_to_dex(str_addr, 6), _dec_to_dex(program_len,6)},3);
		codes.clear();
		start_code_addr = -1;
		while (opcode.compare("END") != 0)
		{
			if (!_is_comment_line(line))
			{
				int opcode_int = get_opcode(opcode);
				if (opcode_int != -1)
				{
					if (_is_symbol(operand))
					{
						iter = sym_tab.find(operand);
						if (iter != sym_tab.end())
						{
							operand_addr = iter->second;
						}
						else
						{
							operand_addr = 0;
							_set_error_flag("undefined symbol", loc);
						}
					}
					else
					{
						operand_addr = 0;
					}

					object_code = _assemble_obj_code(opcode_int, operand_addr);
				}
				else if (opcode.compare("BYTE") == 0)
				{
					int identify_first_pos = operand.find('\'');
					int identify_second_pos = operand.rfind('\'');

					int len = identify_second_pos - identify_first_pos - 1;

					char const_type = operand.substr(identify_first_pos - 1, 1)[0];
					string const_val = operand.substr(identify_first_pos + 1, len);

					float temp;

					switch (const_type)
					{
					case 'C':
					case 'c':
						object_code = "";
						for (int i = 0; i < len; i++)
						{
							object_code += _dec_to_dex(const_val[0], 2);
						}
						break;
					case 'X':
					case 'x':
						object_code = const_val;

						if (len % 2 == 1)
						{
							object_code = "0" + object_code;
						}
						break;
					}
				}
				else if (opcode.compare("WORD") == 0)
				{
					int dec_val = atoi(operand.c_str());
					object_code = _dec_to_dex(dec_val, 6);
				}
				else
				{
					_write_listing_line(listing_file, "");
					_read_intermediate_line(in_file);
					continue;
				}

				if (start_code_addr == -1)
				{
					start_code_addr = loc;
				}

				if (codes.size() >= codes.capacity())
				{
					_write_object_line(obj_file, 'T', codes, start_code_addr);
					codes.clear();
					start_code_addr = -1;
				}
				codes.push_back(object_code);
			}
			_write_listing_line(listing_file, object_code);
			_read_intermediate_line(in_file);
		}
		_write_object_line(obj_file, 'T', codes, start_code_addr);
		_write_object_line(obj_file, 'E', 1, new string[1]{ _dec_to_dex(str_addr,2) },1);
		_write_listing_line(listing_file, "");

		if (errors.size() > 0)
		{
			listing_file << endl << "ERROR";
			for (int i = 0; i < errors.size(); i++)
				listing_file << endl << errors[i];
		}

		in_file.close();
		listing_file.close();
		obj_file.close();
	}
	else
	{
		std::cout << "Can not open file" << std::endl;
	}
}

void generate_object_file(string path)
{
	sic_assembler_pass1(path);
	sic_assembler_pass2(path.substr(0, path.rfind(".")) + "_mid.txt");
}

void main()
{
	init_opcode_tab();
	generate_object_file("source.txt");
}