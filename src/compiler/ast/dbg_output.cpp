/*

Copyright (C) 17..19 Ivan Kniazkov

This file is part of interpreter of programming language
codenamed "Goat" ("Goat interpreter").

Goat interpreter is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goat interpreter is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "dbg_output.h"
#include "root.h"
#include "function.h"
#include "identifier.h"
#include "variable.h"
#include "bracket.h"
#include "static_string.h"
#include "brackets_pair.h"
#include "function_call.h"
#include "statement_expression.h"
#include "custom_operator.h"
#include "addition.h"
#include "integer.h"
#include "subtraction.h"
#include "negation.h"
#include "declare_variable.h"
#include "assignment.h"

namespace g0at
{
    namespace ast
    {
        dbg_output::dbg_output(std::wstringstream &_stream)
            : stream(_stream), indent(0)
        {
        }

        dbg_output::dbg_output(std::wstringstream &_stream, int _indent)
            : stream(_stream), indent(_indent)
        {
        }

        std::wstring dbg_output::to_string(lib::pointer<root> obj)
        {
            std::wstringstream tmp;
            dbg_output dbg(tmp);
            obj->accept(&dbg);
            return tmp.str();
        }

        void dbg_output::visit(function *ref)
        {
            add_indent();
            stream << L"$";
            print_token_list(ref->get_raw_list());
        }

        void dbg_output::visit(identifier *ref)
        {
            add_indent();
            stream << ref->get_name();
        }

        void dbg_output::visit(variable *ref)
        {
            add_indent();
            stream << L'^' << ref->get_name();
        }

        void dbg_output::visit(bracket *ref)
        {
            add_indent();
            stream << ref->get_symbol();
        }

        void dbg_output::visit(static_string *ref)
        {
            add_indent();
            stream << L'\"' << ref->get_text() << L'\"';
        }

        void dbg_output::visit(semicolon *ref)
        {
            add_indent();
            stream << L';';
        }

        void dbg_output::visit(brackets_pair *ref)
        {
            add_indent();
            stream << ref->get_symbol() << ref->get_inverse_symbol();
            print_token_list(ref->get_raw_list());
        }

        void dbg_output::visit(function_call *ref)
        {
            add_indent();
            stream << L"call";
            dbg_output indented(stream, indent + 1);
            ref->get_func_object()->accept(&indented);
            print_token_list(ref->get_raw_list());
        }

        void dbg_output::visit(statement_expression *ref)
        {
            add_indent();
            stream << L"stmt";
            dbg_output indented(stream, indent + 1);
            ref->get_expression()->accept(&indented);
        }

        void dbg_output::visit(plus *ref)
        {
            add_indent();
            stream << L'+';
        }

        void dbg_output::visit(custom_operator *ref)
        {
            add_indent();
            stream << ref->get_oper();
        }

        void dbg_output::visit(addition *ref)
        {
            add_indent();
            stream << L"+";
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
            ref->get_right()->accept(&indented);
        }

        void dbg_output::visit(integer *ref)
        {
            add_indent();
            stream << ref->get_value();
        }

        void dbg_output::visit(minus *ref)
        {
            add_indent();
            stream << L'-';
        }

        void dbg_output::visit(subtraction *ref)
        {
            add_indent();
            stream << L"-";
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
            ref->get_right()->accept(&indented);
        }

        void dbg_output::visit(negation *ref)
        {
            add_indent();
            stream << L"-";
            dbg_output indented(stream, indent + 1);
            ref->get_right()->accept(&indented);
        }

        void dbg_output::visit(value_void *ref)
        {
            add_indent();
            stream << L"void";
        }

        void dbg_output::visit(value_undefined *ref)
        {
            add_indent();
            stream << L"undefined";
        }

        void dbg_output::visit(value_null *ref)
        {
            add_indent();
            stream << L"null";
        }

        void dbg_output::visit(keyword_var *ref)
        {
            add_indent();
            stream << L"var";
        }

        void dbg_output::visit(declare_variable *ref)
        {
            for (int i = 0, cnt = ref->get_count(); i < cnt; i++)
            {
                add_indent();
                variable_info info = ref->get_variable(i);
                stream << L"var " << info.name;
                if (info.init_val)
                {
                    dbg_output indented(stream, indent + 1);
                    info.init_val->accept(&indented);
                }
            }
        }

        void dbg_output::visit(assign *ref)
        {
            add_indent();
            stream << L'=';
        }

        void dbg_output::visit(assignment *ref)
        {
            add_indent();
            stream << L'=';
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
            ref->get_right()->accept(&indented);
        }

        void dbg_output::visit(comma *ref)
        {
            add_indent();
            stream << L',';
        }

        void dbg_output::add_indent()
        {
            if (!indent)
                return;
            stream << L"\n";
            for (int k = 0; k < indent; k++)
            {
                stream << "  ";
            }
        }

        void dbg_output::print_token_list(token_list *list)
        {
            auto tok = list->first;
            dbg_output indented(stream, indent + 1);
            while(tok)
            {
                tok->accept(&indented);
                tok = tok->next;
            }
        }
    };
};