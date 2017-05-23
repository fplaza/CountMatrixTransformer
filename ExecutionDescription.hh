/*
 * Copyright 2016-2017 Enterome
 */ 

#ifndef EXECUTION_DESCRIPTION_HH
#define EXECUTION_DESCRIPTION_HH

#include <string>

struct ExecutionDescription
{
    const std::string date;
    const std::string user;
    const std::string hostname;
    const std::string wd;
    const std::string cmd;
    const std::string program_version;

    ExecutionDescription(const int argc, char* const argv[]);
};

#endif // EXECUTION_DESCRIPTION_HH
