/*
 * Copyright 2016-2017 Enterome
 */ 

#include "ExecutionDescriptionPrinter.hh"
#include <fstream>

void ExecutionDescriptionPrinter::print(const ExecutionDescription& execution_description, const std::string& output_file)
{
    std::ofstream ofs;
    ofs.open(output_file.c_str(), std::ios_base::app);

    ofs << "##\n";

    ofs << "##\tdate: " <<  execution_description.date << '\n';

    ofs << "##\tuser: " << execution_description.user << '\n';

    ofs << "##\thostname: " << execution_description.hostname << '\n';

    ofs << "##\twd: " << execution_description.wd << '\n';

    ofs << "##\tcmd: " << execution_description.cmd << '\n';

    ofs << "##\tprogram version: " << execution_description.program_version << '\n';

    ofs << "##\n";

    ofs.close();
}
