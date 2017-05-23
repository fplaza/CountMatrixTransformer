/*
 * Copyright 2016-2017 Enterome
 */ 

#include "ExecutionDescription.hh"
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
namespace pt = boost::posix_time;
#include <unistd.h>

namespace
{
    std::string get_date()
    {
        const pt::ptime now = pt::second_clock::local_time();
        return to_iso_extended_string(now);
    }

    std::string get_user()
    {
        const std::string user(getlogin());
        return user;
    }

    std::string get_hostname()
    {
        char buf[HOST_NAME_MAX];
        gethostname(buf, HOST_NAME_MAX);

        const std::string& hostname(buf);

        return hostname;
    }

    std::string get_wd()
    {
        const std::string wd = 
            boost::filesystem::current_path().string();

        return wd;
    }

    std::string get_cmd(const int argc, char* const argv[])
    {
        std::ostringstream oss;

        for (int i = 0; i < argc; ++i)
        {
            oss << ' ' << argv[i];
        }

        return oss.str();
    }
};

ExecutionDescription::ExecutionDescription(const int argc, char* const argv[])
    : date(get_date()),
    user(get_user()),
    hostname(get_hostname()),
    wd(get_wd()),
    cmd(get_cmd(argc, argv)),
    program_version(__APP_VERSION__)
{}

