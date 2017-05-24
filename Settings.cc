/*
 * Copyright 2016-2017 Enterome
 */ 

#include "Settings.hh"
#include <iostream>
#include <fstream>
#include <exception>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

Settings Settings::parse(int argc, char* argv[])
{
    Settings settings;

    // Create options decription
    po::options_description all_settings("");
    po::options_description input_settings("[input]");
    po::options_description transformation_settings("[transformation]");
    po::options_description output_settings("[output]");
	po::options_description misc_settings("[miscellaneous]");

    input_settings.add_options()
        ("count-matrix,c", po::value<std::string>(&settings.input.count_matrix_file), "count matrix to transform")
        ("genes-length,l", po::value<std::string>(&settings.input.genes_length_file), "file with an header which contains line by line, tab separated pairs of values '<gene name> <gene length>'")
        ;

    transformation_settings.add_options()
        ("transformation,t", po::value<std::string>(&settings.transformation.type), "transformation to apply to the count matrix ('normalize' or 'denormalize')")
        ("min-non-null,m", po::value<double>(&settings.transformation.min_non_null)->default_value(1), "minimal non null value in raw matrix. (denormalize only)")
        ;

    output_settings.add_options()
        ("output-file,o", po::value<std::string>(&settings.output.output_file), "transformed count matrix")
        ;

    misc_settings.add_options()
        ("help,h", "display this help and exit.")
        ;

    all_settings.add(input_settings).add(transformation_settings).add(output_settings).add(misc_settings);

    // Retrieve and settingse command line settings
    settings.executable_name = argv[0];
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, all_settings), vm);

    // Print help
    if (argc == 1 || vm.count("help"))
    {
        std::cout << all_settings << std::endl;
        std::exit(0);
    }

    po::notify(vm);

    // required method is missing in old boost version
    check_setting_is_set("--count-matrix", settings.input.count_matrix_file);
    check_setting_is_set("--genes-length", settings.input.genes_length_file);
    check_setting_is_set("--transformation", settings.transformation.type);
    check_setting_is_set("--output-file", settings.output.output_file);

    check_file_is_readable(settings.input.count_matrix_file);
    check_file_is_readable(settings.input.genes_length_file);
    check_file_is_writable(settings.output.output_file);

    return settings;
}

void Settings::check_setting_is_set(const std::string& val_name, const std::string& value)
{
    if (value == "")
    {
        throw (std::invalid_argument("error: " + val_name + " setting is missing."));
    }
}

void Settings::check_file_is_readable(const std::string& filepath)
{
    std::ifstream ifs;
    ifs.open(filepath.c_str());

    if (ifs.good())
    {
        ifs.close();
    }
    else
    {
        throw (std::invalid_argument("error: " + filepath +
                    " cannot be opened. Check that the path is valid and that you have read permissions."));
    }
}

void Settings::check_file_is_writable(const std::string& filepath)
{
    std::ofstream ofs;
    ofs.open(filepath.c_str());

    if (ofs.good())
    {
        ofs.close();
    }
    else
    {
        throw (std::invalid_argument("error: " + filepath +
                    " cannot be created. Check that the path is valid and that you have write permissions."));
    }
}

std::ostream& operator<<(std::ostream& os, const Settings& settings)
{
	const char sep[] = "---------------------\n";

    os << sep;
	os << "Settings summary:\n";

    os << "\n[input]\n";
    os << "--count-matrix = " << settings.input.count_matrix_file << '\n';
    os << "--genes-length = " << settings.input.genes_length_file << '\n';

    os << "\n[transformation]\n";
    os << "--transformation = " << settings.transformation.type << '\n';
    os << "--min-non-null = " << settings.transformation.min_non_null << '\n';

    os << "\n[output]\n";
    os << "--output-file = " << settings.output.output_file << '\n';
    os << sep;

	return os;
}
