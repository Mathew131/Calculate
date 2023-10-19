#include "std_lib_facilities.h"
#include "variable.h"

double Symbol_table::get_value (const string& s) const
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s)
            return var_table[i].value;

    error("get: undefined name ", s);
}

void Symbol_table::set_value (const string& s, double d)
{
    for (int i = 0; i <= var_table.size(); ++i)
    {
        if (var_table[i].name == s)
        {
            if (var_table[i].is_const) error("modified constant ", s);
            var_table[i].value = d;
            return;
        }
    }

    error("set: undefined name ", s);
}

bool Symbol_table::is_declared (const string& s) const
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s)
            return true;

    return false;
}

double Symbol_table::define_name (const string& var, double val, bool is_const)
{
    if (is_declared(var))
        error(var, " declared twice");

    var_table.push_back(Variable{var, val, is_const});

    return val;
}