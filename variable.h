#pragma once

struct Variable
{
    string name;
    double value;
    bool is_const = false;

    Variable(string n, double v, bool c) : name{n}, value{v}, is_const{c} {}
};

class Symbol_table 
{
public:
    double get_value (const string& s) const;
    void set_value (const string& s, double d);
    bool is_declared (const string& s) const;
    double define_name (const string& var, double val, bool is_const);
private:
    vector<Variable> var_table;
};
