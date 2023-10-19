#pragma once

extern bool g_is_new_line;

struct Token
{
    char kind;
    double value;
    string name;

    Token(char ch) : kind{ch}, value{0} {}

    Token(char ch, double val) : kind{ch}, value{val} {}

    Token(char ch, string n) : kind{ch}, name{n} {}
};

class Token_stream
{
  
    bool full{false};
    Token buffer{0};

  public:
    Token_stream() {}

    Token get ();
    void putback (Token t);

    void ignore (char);
};

constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char let = 'L';
constexpr char const_let = 'C';
constexpr char help = 'h';

const string prompt = "> ";
const string result = "= ";
const string declkey = "let";
const string const_declkey = "const_let";
