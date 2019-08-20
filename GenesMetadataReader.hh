#ifndef GENES_METADATA_READER_HH
#define GENES_METADATA_READER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include "GeneMetadata.hh"
#include <vector>
#include <string>

class GenesMetadataReader
{
    public:
        static std::vector<GeneMetadata> read(const std::string& genes_metadata_file, const unsigned long expected_num_genes);
    private:
        static const size_t MAX_LINE_LEN;
};

#endif //GENES_METADATA_READER_HH

