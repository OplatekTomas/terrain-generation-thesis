//
// Created by tomas on 18.11.21.
//

#pragma once

#include <models/openstreetmap/MetadataResult.h>
#include <memory>
#include <vector>
namespace MapGenerator{

    class OSMData{
    public:
        explicit OSMData(std::shared_ptr<MetadataResult> metadataResult);
        std::vector<long> getMissingNodes();
        void addNode(element node);
        element getNode(long id);

        std::vector<element> getNodes();
        std::vector<element> getWays();

    private :
        std::shared_ptr<MetadataResult> metadataResult;
        std::vector<element> ways;
        std::vector<element> nodes;
        bool nodeExists(long nodeId);
    };
}




