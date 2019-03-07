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
#include "real.h"
#include "declare_function.h"
#include "statement_return.h"
#include "token_object.h"
#include "property.h"
#include "is_equal_to.h"
#include "is_not_equal_to.h"
#include "statement_while.h"
#include "method_call.h"
#include "lib/utils.h"

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

        std::wstring dbg_output::to_string(token *obj)
        {
            std::wstringstream tmp;
            dbg_output dbg(tmp);
            obj->accept(&dbg);
            return tmp.str();
        }

        void dbg_output::visit(function *ref)
        {
            add_indent();
            stream << L'$';
            print_token_list(ref->get_raw_args_list(), L"args (raw)");
            print_token_list(ref->get_args_list(), L"args");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_body(), L"body");
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
            stream << L'\"' << lib::escape_special_chars(ref->get_text()) << L'\"';
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
            print_token_list(ref->get_raw_list(), L"tokens");
        }

        void dbg_output::visit(function_call *ref)
        {
            add_indent();
            stream << L"call";
            dbg_output indented(stream, indent + 1);
            ref->get_func_object()->accept(&indented);
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
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

        void dbg_output::visit(real *ref)
        {
            add_indent();
            stream << lib::double_to_wstring(ref->get_value());
        }

        void dbg_output::visit(keyword_function *ref)
        {
            add_indent();
            stream << L"function";
        }

        void dbg_output::visit(declare_function *ref)
        {
            ref->get_func()->accept(this);
        }

        void dbg_output::visit(keyword_return *ref)
        {
            add_indent();
            stream << L"return";
        }

        void dbg_output::visit(statement_return *ref)
        {
            add_indent();
            stream << L"return";
            auto expr = ref->get_expression();
            if (expr)
            {
                dbg_output indented(stream, indent + 1);
                expr->accept(&indented);
            }
        }

        void dbg_output::visit(token_object *ref)
        {
            add_indent();
            stream << L"object";
            print_token_list(ref->get_raw_list(), L"raw");
            int count = ref->get_items_count();
            if (count > 0)
            {
                dbg_output indented(stream, indent + 2);
                for (int i = 0; i < count; i++)
                {
                    auto item = ref->get_item(i);
                    add_indent(indent + 1);
                    stream << L"key";
                    item.first->accept(&indented);
                    add_indent(indent + 1);
                    stream << L"value";
                    item.second->accept(&indented);
                }
            }
        }

        void dbg_output::visit(colon *ref)
        {
            add_indent();
            stream << L':';
        }

        void dbg_output::visit(dot *ref)
        {
            add_indent();
            stream << L'.';
        }

        void dbg_output::visit(property *ref)
        {
            add_indent();
            stream << L'.' << ref->get_name();
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
        }

        void dbg_output::visit(value_true *ref)
        {
            add_indent();
            stream << L"true";
        }

        void dbg_output::visit(value_false *ref)
        {
            add_indent();
            stream << L"false";
        }

        void dbg_output::visit(equals *ref)
        {
            add_indent();
            stream << L"==";
        }

        void dbg_output::visit(not_equal *ref)
        {
            add_indent();
            stream << L"!=";
        }

        void dbg_output::visit(is_equal_to *ref)
        {
            add_indent();
            stream << L"==";
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
            ref->get_right()->accept(&indented);
        }

        void dbg_output::visit(is_not_equal_to *ref)
        {
            add_indent();
            stream << L"!=";
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
            ref->get_right()->accept(&indented);
        }

        void dbg_output::visit(keyword_while *ref)
        {
            add_indent();
            stream << L"while";
        }

        void dbg_output::visit(statement_while *ref)
        {
            add_indent();
            stream << L"while";
            dbg_output indented(stream, indent + 2);
            add_indent(indent + 1);
            stream << L"expr";
            ref->get_expression()->accept(&indented);
            add_indent(indent + 1);
            stream << L"stmt";
            ref->get_statement()->accept(&indented);
        }

        void dbg_output::visit(method_call *ref)
        {
            add_indent();
            stream << L"vcall " << ref->get_name();
            dbg_output indented(stream, indent + 1);
            ref->get_left()->accept(&indented);
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
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

        void dbg_output::add_indent(int value)
        {
            if (!value)
                return;
            stream << L"\n";
            for (int k = 0; k < value; k++)
            {
                stream << "  ";
            }
        }

        void dbg_output::print_token_list(token_list *list, const wchar_t *title)
        {
            auto tok = list->first;
            if (!tok)
                return;
            add_indent(indent + 1);
            stream << title;
            dbg_output indented(stream, indent + 2);
            while(tok)
            {
                tok->accept(&indented);
                tok = tok->next;
            }
        }
    };
};