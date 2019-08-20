/*
 * Copyright 2016-2017 Enterome
 */ 

#include "GenesMetadataReader.hh"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <iostream>

std::vector<GeneMetadata> GenesMetadataReader::read(const std::string& genes_metadata_file, const unsigned long expected_num_genes)
{
    std::vector<GeneMetadata> genes_metadata;
    genes_metadata.reserve(expected_num_genes);

    std::ifstream ifs;
    ifs.open(genes_metadata_file.c_str());

    std::string gene_name;
    unsigned long gene_id, gene_length;

    ifs >> gene_id >> gene_name >> gene_length;
    while (!ifs.eof())
    {
        const GeneMetadata new_gene_metadata = {gene_id, gene_name, gene_length};
        genes_metadata.push_back(new_gene_metadata);

        ifs >> gene_id >> gene_name >> gene_length;
    }

    ifs.close();

    if (genes_metadata.size() != expected_num_genes)
    {
        throw std::runtime_error("error: count matrix and genes length file do not have the same number of genes.");
    }

    return genes_metadata;

    /*
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
    */

    //return genes_length;
}
