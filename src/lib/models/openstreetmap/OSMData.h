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
        explicit OSMData(const std::shared_ptr<MetadataResult>& metadataResult);
        std::vector<long> getMissingNodes();
        void addNode(const Node& node);
        Node getNode(long id);

        std::vector<Node> getNodes();
        std::vector<Node> getWays();

        double lon1;
        double lon2;
        double lat1;
        double lat2;

    private :
        std::shared_ptr<MetadataResult> metadataResult;
        std::vector<Node> ways;
        std::vector<Node> relations;
    public:
        const std::vector<Node> &getRelations() const;
    private:

        std::vector<Node> nodes;
        bool nodeExists(long nodeId);
    };
}




