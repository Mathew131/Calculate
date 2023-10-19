/*
После символа '>' введите выражение, которое следует посчитать, и завершите ввод символом ';' или переходом на новую строку.
Можно создать переменные: 
Изменяемые: let 'название переменной' = значение;
Константные: const_let 'название переменной' = значение;
Поддерживаемые операторы: +, -, *, /, ^, ()
*/

#include "std_lib_facilities.h"
#include "token.h"
#include "variable.h"

Symbol_table Symbol_t;
Token_stream Token_s;

double expression ();

double primary ()
{
    Token t = Token_s.get();
    switch (t.kind)
    {
    case '(':
    {
        double d = expression();
        t = Token_s.get();
        if (t.kind != ')')
            error("'(' expected");
        return d;
    }

    case '-':
        return -primary();
    case '+':
        return +primary();
    case number:
        return t.value;

    case name:
        return Symbol_t.get_value(t.name);
    default:
        error("primary expected");
    }
}

double pow () {
    double left = primary();

    while (true)
    {
        Token t = Token_s.get();

        switch (t.kind)
        {
        case '^':
            left = pow(left, primary());
            break;

        default:
            Token_s.putback(t);
            return left;
        }
    }
}

double term ()
{
    double left = pow();

    while (true)
    {
        Token t = Token_s.get();

        switch (t.kind)
        {
        case '*':
            left *= pow();
            break;

        case '/':
        {
            double d = pow();
            if (d == 0)
                error("divide by zero");
            left /= d;
            break;
        }

        default:
            Token_s.putback(t);
            return left;
        }
    }
}

double expression ()
{
    double left = term();

    while (true)
    {
        Token t = Token_s.get();

        switch (t.kind)
        {
        case '+':
            left += term();
            break;

        case '-':
            left -= term();
            break;
        
        default:
            Token_s.putback(t);
            return left;
        }
    }
}

double declaration (bool is_const)
{
    Token t = Token_s.get();
    if (t.kind != name)
        error("name expected in declaration");

    string var = t.name;
    if (Symbol_t.is_declared(var))
        error(var, " declared twice");

    t = Token_s.get();
    if (t.kind != '=')
        error("'=' missing in declaration of ", var);

    return Symbol_t.define_name(var, expression(), is_const);
}

double statement ()
{
    Token t = Token_s.get();
    switch (t.kind)
    {
    case let:
        return declaration(0);
    case const_let:
        return declaration(1);
    default:
        // изменение значения переменной
        if (t.kind == name) {
            char c;
            cin >> c;
            if (c == '=') {
                double a0 = expression();
                Symbol_t.set_value(t.name, a0);
                return a0;
            } 
            cin.putback(c);
            
            Token_s.putback(t);
            return expression();
        }
        Token_s.putback(t);
        return expression();
    }
}

void clean_up_mess () { Token_s.ignore(print); }

void calculate ()
{
    while (true)
        try
        {
            if (g_is_new_line)
                cout << prompt;
                g_is_new_line = false;
            Token t = Token_s.get();
            while (t.kind == print) t = Token_s.get();
            if (t.kind == help) {
                cout << "Это калькулятор.\nПосле значка '>' вводите либо выражение," \
                " которое нужно посчиать, либо объявите переменную с помощью ключевых слов 'let' и 'const_let'(константа)'\n";
                continue;
            }
            if (t.kind == quit) return;
            Token_s.putback(t);
            
            cout << result << statement() << endl;
        }
        catch (runtime_error& e)
        {
            cerr << e.what() << endl;
            clean_up_mess();
        }
}

int main ()
try
{
    // const_let - добавление константы
    Symbol_t.define_name("pi", 3.141592653589793, 1);
    Symbol_t.define_name("e", 2.718281828459045, 1);

    calculate();
}
catch (exception& e)
{
    cerr << "exception: " << e.what() << endl;
    return 1;
}
catch (...)
{
    cerr << "Oops, unknown exception" << endl;
    return 2;
}
