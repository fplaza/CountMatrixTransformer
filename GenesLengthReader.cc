/*
 * Copyright 2016-2017 Enterome
 */ 

#include "GenesLengthReader.hh"
#include <boost/unordered_map.hpp>
#include <fstream>
#include <sstream>

const size_t GenesLengthReader::MAX_LINE_LEN = 1024;

std::vector<size_t> GenesLengthReader::read(const std::string& genes_length_file, Matrix<float>& cm)
{
    boost::unordered_map<std::string, size_t> gene_to_length;
    gene_to_length.rehash(cm.nrow());

    std::ifstream ifs;
    ifs.open(genes_length_file.c_str());

    char line[MAX_LINE_LEN];
    std::string gene_name;
    size_t gene_length;

    //Read pairs of '<gene_name> <gene_length>'
    while (ifs.getline(line, MAX_LINE_LEN))
    {
        char* pos = line;

        while (*pos != '\0' && !std::isspace(*pos))
            ++pos;

        gene_name = std::string(line, pos-line);
        gene_length = strtoul(pos,&pos, 10);

        gene_to_length[gene_name] = gene_length;
    }

    if (gene_to_length.size() != cm.nrow())
    {
        throw std::runtime_error("error: count matrix and genes length file do not have the same number of genes.");
    }

    std::vector<size_t> genes_length(cm.nrow());
    // Assign a size to each gene
    for (size_t curr_gene = 0; curr_gene < cm.nrow(); ++curr_gene)
    {
        const std::string& curr_gene_name = cm.rownames()[curr_gene];

        boost::unordered_map<std::string, size_t>::const_iterator gene_to_length_it =
            gene_to_length.find(curr_gene_name);

        if (gene_to_length_it == gene_to_length.end())
        {
            std::ostringstream error_msg;
            error_msg << "error: gene '" << curr_gene_name << "' not found in genes length file.";
            throw std::runtime_error(error_msg.str());
        }

        const size_t gene_length = gene_to_length_it->second;

        if (gene_length == 0)
        {
            std::ostringstream error_msg;
            error_msg << "error: gene '" << curr_gene_name << "' has a length of zero.";
            throw std::runtime_error(error_msg.str());
        }

        genes_length[curr_gene] = gene_length;
    }
    
    return genes_length;
}
