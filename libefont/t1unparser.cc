// -*- related-file-name: "../include/efont/t1unparser.hh" -*-
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <efont/t1unparser.hh>
namespace Efont {

CharstringUnparser::CharstringUnparser()
    : CharstringInterp(0, 0),
      _one_command_per_line(false), _start_of_line(true)
{
}

CharstringUnparser::CharstringUnparser(const CharstringUnparser &o)
    : CharstringInterp(o),
      _one_command_per_line(o._one_command_per_line),
      _start_of_line(o._start_of_line)
{
}

void
CharstringUnparser::clear()
{
    _sa.clear();
    _start_of_line = true;
}

bool
CharstringUnparser::number(double n)
{
    if (_start_of_line) {
	_sa << _indent;
	_start_of_line = false;
    } else
	_sa << ' ';
    _sa << n;
    return true;
}

bool
CharstringUnparser::type1_command(int cmd)
{
    if (_start_of_line) {
	_sa << _indent;
	_start_of_line = false;
    } else
	_sa << ' ';
    if (cmd >= 0 && cmd <= CS::cLastCommand)
	_sa << CS::command_names[cmd];
    else
	_sa << "UNKNOWN_12_" << (cmd - 32);
    if (_one_command_per_line) {
	_sa << '\n';
	_start_of_line = true;
    }
    return true;
}

bool
CharstringUnparser::type2_command(int cmd, const unsigned char *data, int *left)
{
    if (_start_of_line) {
	_sa << _indent;
	_start_of_line = false;
    } else
	_sa << ' ';
    
    if (cmd >= 0 && cmd <= CS::cLastCommand)
	_sa << CS::command_names[cmd];
    else
	_sa << "UNKNOWN_12_" << (cmd - 32);

    switch (cmd) {
      case CS::cHstem: case CS::cHstemhm: case CS::cVstem: case CS::cVstemhm:
      case CS::cHintmask: case CS::cCntrmask:
	CharstringInterp::type2_command(cmd, data, left);
	break;
    }
    
    if (_one_command_per_line) {
	_sa << '\n';
	_start_of_line = true;
    }

    return true;
}

void
CharstringUnparser::act_hintmask(int, const unsigned char *data, int nhints)
{
    _sa << '[';
    for (int i = 0; i < nhints; i++, data++)
	sprintf(_sa.extend(2), "%02X", *data);
    _sa << ']';
}

String
CharstringUnparser::value()
{
    _start_of_line = true;
    return _sa.take_string();
}

String
CharstringUnparser::unparse(const Charstring *cs)
{
    if (cs) {
	CharstringUnparser u;
	cs->run(u);
	return u.value();
    } else
	return "(null)";
}

String
CharstringUnparser::unparse(const Charstring &cs)
{
    CharstringUnparser u;
    cs.run(u);
    return u.value();
}

}
