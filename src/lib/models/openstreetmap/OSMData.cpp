//
// Created by tomas on 18.11.21.
//

#include <models/openstreetmap/OSMData.h>
#include <boolinq.h>
#include <Helper.h>

namespace MapGenerator {

    OSMData::OSMData(const std::shared_ptr<MetadataResult> &metadataResult) {
        this->metadataResult = metadataResult;
        const std::vector<std::string> tagTypes{"landuse", "highways", "waterways"};
        this->ways = boolinq::from(metadataResult->elements)
                .where([](const Node &x) {
                    return x.type == Type::way && x.tags != nullptr;
                }).where([&](const Node &x) {
            return boolinq::from(tagTypes).any([&](const std::string &t) { return mapContainsKey(*x.tags, t); });
        }).toStdVector();

        this->ways = boolinq::from(metadataResult->elements)
                .where([](const Node &x) {
                    return x.type == Type::way && x.tags != nullptr && x.geometry->at(0).lat == x.geometry->at(x.geometry->size() - 1).lat &&
                           x.geometry->at(0).lon == x.geometry->at(x.geometry->size() - 1).lon;
                }).toStdVector();
        this->relations = boolinq::from(metadataResult->elements)
                .where([](const Node &x) {
                    return x.type == Type::relation && x.tags != nullptr;
                }).toStdVector();
    }

    void OSMData::addNode(const Node &node) {
        //insert into correct position in an ordered list by id with binary search
        auto it = std::lower_bound(nodes.begin(), nodes.end(), node, [](const Node &a, const Node &b) {
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
                auto it = std::lower_bound(missingNodes.begin(), missingNodes.end(), nodeId,
                                           [](const long &a, const long &b) {
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
        auto it = std::lower_bound(nodes.begin(), nodes.end(), nodeId, [](const Node &a, const long &b) {
            return a.id < b;
        });
        return it != nodes.end() && it->id == nodeId;
    }

    std::vector<Node> OSMData::getNodes() {
        return nodes;
    }

    std::vector<Node> OSMData::getWays() {
        return ways;
    }

    Node OSMData::getNode(long id) {
        //use binary search to find the node
        auto it = std::lower_bound(nodes.begin(), nodes.end(), id, [](const Node &a, const long &b) {
            return a.id < b;
        });
        if (it != nodes.end() && it->id == id) {
            return *it;
        }
        return {};
    }

    const std::vector<Node> &OSMData::getRelations() const {
        return relations;
    }
}
