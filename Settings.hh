#ifndef SETTINGS_HH
#define SETTINGS_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include <string>

struct Settings
{
    std::string executable_name;

    struct
    {
        std::string count_matrix_file;
        std::string genes_metadata_file;
    } input;

    struct
    {
        std::string type;
        double min_non_null;
        unsigned long mean_read_length;
    } transformation;

    struct
    {
        std::string output_file;
        bool with_header;
    } output;

    static Settings parse(int argc, char* argv[]);
    static void check_setting_is_set(const std::string& val_name, const std::string& value);
    static void check_file_is_readable(const std::string& filepath);
    static void check_file_is_writable(const std::string& filepath);
    template <typename T>
    static void check_val_within_bounds(const std::string& val_name, T value, T lower, T higher);
};

std::ostream& operator<<(std::ostream& os, const Settings& settings);

#endif // SETTINGS_HH
