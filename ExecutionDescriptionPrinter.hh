/*
 * Copyright 2016-2017 Enterome
 */ 

#ifndef EXECUTION_DESCRIPTION_PRINTER_HH
#define EXECUTION_DESCRIPTION_PRINTER_HH

#include <vector>
#include <string>
#include "ExecutionDescription.hh"

class ExecutionDescriptionPrinter
{
    public:
        static void print(const ExecutionDescription& execution_description, const std::string& output_file);
};

#endif // EXECUTION_DESCRIPTION_PRINTER_HH
