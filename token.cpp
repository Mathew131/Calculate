#include "std_lib_facilities.h"
#include "token.h"

bool g_is_new_line = true;

void Token_stream::putback(Token t)
{
    if (full)
        error("putback() into a full buffer");

    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }
    
    char ch;
    while(true) 
    {
        cin.get(ch);
        if (ch == '\n' || ch != ' ') break;
    }
    

    switch (ch)
    {
    case '(':
    case ')':
    case '+':
    case '^':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case ';':
        char ch0;
        cin.get(ch0);
        if (ch0 == '\n') g_is_new_line = true;
        else cin.putback(ch0);
        return Token{ch};
    case '\n': 
        g_is_new_line = true;
        return Token(';');
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token{number, val};
    }

    default:
        if (isalpha(ch))
        {
            string s;
            s += ch; 
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))
                s += ch;
            cin.putback(ch);
            if (s == "help") 
                return Token{help};
            else if (s == "quit") 
                return Token{quit};
            else if (s == declkey)
                return Token{let};
            else if (s == const_declkey)
                return Token{const_let};
            return Token{name, s};
        }
        error("Bad token");
    }
}

void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind)
    {
        full = false;
        return;
    }
    full = false;

    char ch;
    while (cin >> ch) if (ch == c) return;
}