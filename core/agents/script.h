/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SCRIPT_H
#define SCRIPT_H
#include "core/agents/agent.h"

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>

#include <boost/noncopyable.hpp>

namespace {
void squirrel_print_function(HSQUIRRELVM, const SQChar *s, ...)
{
    va_list arglist;
    int final_n, n = 64;
    std::unique_ptr<char[]> dst;

    va_start(arglist, s);
    while(1) {
        dst.reset(new char[n]);
        final_n = vsnprintf(&dst[0], n, s, arglist);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }

    LOG(trace) << "SQVM: Print: " << dst.get();
    va_end(arglist);
}

void squirrel_stack_trace(HSQUIRRELVM sqvm)
{
    SQStackInfos stack_info;
    SQInteger stack_depth = 1;

    LOG(error) << "SQVM: Stack dump: ";
    while(SQ_SUCCEEDED(sq_stackinfos(sqvm, stack_depth, &stack_info))) {
        const SQChar *func_name = (stack_info.funcname) ?
                                   stack_info.funcname : "unknown_function";
        const SQChar *source_file = (stack_info.source) ?
                                     stack_info.source : "unknown_source_file";

        LOG(error) << sge::string_format("[%d]: function [%s()] %s line [%d]",
                                         stack_depth,
                                         func_name,
                                         source_file,
                                         stack_info.line);
        stack_depth++;
    }
    LOG(error) << "SQVM: Stack end";
}

SQInteger squirrel_runtime_error(HSQUIRRELVM sqvm)
{
    const SQChar* error_message = nullptr;
    if(sq_gettop(sqvm) >= 1) {
        if(SQ_SUCCEEDED(sq_getstring(sqvm,2,&error_message))) {
            LOG(error) << "SQVM: Runtime error: " << error_message;
        }
        squirrel_stack_trace(sqvm);
    }
    return 0;
}

void squirrel_compile_error(HSQUIRRELVM, const SQChar* description, const SQChar* file, SQInteger line, SQInteger column)
{
   LOG(error) << "SQVM: Compile error: " << sge::string_format("sqvm: '%s' (Ln:%d Col:%d) : %s.",
                                                               file,
                                                               line,
                                                               column,
                                                               description);
}

void squirrel_error_function(HSQUIRRELVM, const SQChar *s, ...)
{
    va_list arglist;
    int final_n, n = 64;
    std::unique_ptr<char[]> dst;

    va_start(arglist, s);
    while(1) {
        dst.reset(new char[n]);
        final_n = vsnprintf(&dst[0], n, s, arglist);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }

    LOG(trace) << "SQVM: Error: " << dst.get();
    va_end(arglist);
}
}

class Sqvm : boost::noncopyable
{
    HSQUIRRELVM sqvm;

protected:
    Sqvm()
    {
        LOG(trace) << "SQVM init";
        sqvm = sq_open(1024);

        sq_setprintfunc(sqvm, squirrel_print_function, squirrel_error_function);
        sq_setcompilererrorhandler(sqvm, squirrel_compile_error);
        sq_newclosure(sqvm, squirrel_runtime_error, 0);
        sq_seterrorhandler(sqvm);

        LOG(trace) << "SQVM done";

        const SQChar *program = "local val = 10;"
                                "local str = val.tostring();"
                                "local arr = [\"qwe\", \"rty\", \"uio\", \"ghj\"];"
                                "local last = arr.top();"
                                "::print(last);";
        if (SQ_FAILED(sq_compilebuffer(sqvm, program,
                                       sizeof(SQChar) * strlen(program),
                                       "program", SQTrue)))
            return;

        sq_pushroottable(sqvm);
        if (SQ_FAILED(sq_call(sqvm, 1, SQFalse, SQTrue)))
            return;
    }

public:
    ~Sqvm()
    {
        sq_pop(sqvm, 1); //root
        sq_close(sqvm);
        LOG(trace) << "SQVM closed";
    }

    static Sqvm &instance()
    {
        static auto inst = std::unique_ptr<Sqvm>( new Sqvm() );
        return *inst;
    }
};

class Script : public StaticAgent
{
public:
    SAGENT(Script)

    // Agent interface
public:
    void Deserialize(const rapidjson::Value &val) override;

    std::string source;
    std::string path;
};

REGISTER_AGENT(Script)

#endif // SCRIPT_H
