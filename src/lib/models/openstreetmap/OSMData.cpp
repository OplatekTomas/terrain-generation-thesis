//
// Created by tomas on 18.11.21.
//

#include <models/openstreetmap/OSMData.h>
#include <boolinq.h>
#include <Helper.h>

namespace MapGenerator {

    OSMData::OSMData(std::shared_ptr<MetadataResult> metadataResult) {
        this->metadataResult = metadataResult;
        const std::vector<std::string> tagTypes{"landuse", "highways", "waterways"};
        this->ways = boolinq::from(metadataResult->elements)
                .where([](const element &x) {
                    return x.type == nodeType::way && x.tags != nullptr;
                }).where([&](const element &x) {
            return boolinq::from(tagTypes).any([&](const std::string &t) { return mapContainsKey(*x.tags, t); });
        }).toStdVector();
        this->nodes = boolinq::from(metadataResult->elements)
                .where([](const element &x) { return x.type == nodeType::node; })
                .orderBy([](const element &x) { return x.id; }).toStdVector();
    }

    void OSMData::addNode(element node) {
        //insert into correct position in an ordered list by id with binary search
        auto it = std::lower_bound(nodes.begin(), nodes.end(), node, [](const element &a, const element &b) {
            return a.id < b.id;
        });
        nodes.insert(it, node);
    }

    std::vector<long> OSMData::getMissingNodes() {

        std::vector<long> missingNodes;

        for (const auto &way: ways) {
            for (const auto &nodeId: *way.nodes) {
                if (nodeExists(nodeId)) {
                    continue;
                }
                //Check if nodeId is in missingNodes
                auto it = std::lower_bound(missingNodes.begin(), missingNodes.end(), nodeId, [](const long &a, const long &b) {
                    return a < b;
                });
                if (it == missingNodes.end() || *it != nodeId) {
                    missingNodes.insert(it, nodeId);
                }
            }
        }
        return missingNodes;
    }

    bool OSMData::nodeExists(long nodeId) {
        //check if node with id nodeId exists in the list of nodes
        auto it = std::lower_bound(nodes.begin(), nodes.end(), nodeId, [](const element &a, const long &b) {
            return a.id < b;
        });
        return it != nodes.end() && it->id == nodeId;
    }

    std::vector<element> OSMData::getNodes() {
        return nodes;
    }

    std::vector<element> OSMData::getWays() {
        return ways;
    }

    element OSMData::getNode(long id) {
        //use binary search to find the node
        auto it = std::lower_bound(nodes.begin(), nodes.end(), id, [](const element &a, const long &b) {
            return a.id < b;
        });
        if (it != nodes.end() && it->id == id) {
            return *it;
        }
        return {};
    }
}
