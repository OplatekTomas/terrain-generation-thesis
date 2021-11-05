
//
// Created by tomas on 03.11.21.
//

#include <common/LandTypeGenerator.h>


namespace MapGenerator {

    LandTypeGenerator::LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, int resolution) {

        vector<string> landTypes;
        xSize = std::fabs(lat1 - lat2);
        ySize = std::fabs(lon1 - lon2);
        this->resolution = resolution;
        xStep = xSize / resolution;
        yStep = ySize / resolution;
        yStart = min(lat1, lat2);
        xStart = max(lon1, lon2);
    }

    std::shared_ptr<std::vector<char>> LandTypeGenerator::generateTexture(const shared_ptr<MetadataResult> &metadata) {
        this->metadata = metadata;
        // C#'s Linq like stuff. Works surprisingly well.
        auto wayData = from(metadata->elements)
                .where([](const element &x) {
                    return x.type == nodeType::way && x.tags != nullptr;
                }).where([&](const element &x) {
            return from(tagTypes).any([&](const std::string &t) { return mapContainsKey(*x.tags, t); });
        }).toStdVector();
        nodes = from(metadata->elements)
                .where([](const element &x) { return x.type == nodeType::node; })
                .orderBy([](const element &x) { return x.id; }).toStdVector();
        std::shared_ptr<std::vector<char>> texture = std::make_shared<std::vector<char>>(resolution * resolution * 4);
        for (const auto &way: wayData) {
            AreaOnMap area;
            for (auto nodeId: *way.nodes) {
                auto node = getNode(nodeId);
                area.addNode(node);
            }
            ways.emplace(way, area);
            std::cout << endl;
        }

        for (int x = 0; x < resolution; x++) {
            for (int y = 0; y < resolution; y++) {

            }
        }
        return std::shared_ptr<std::vector<char>>();
    }

    element MapGenerator::LandTypeGenerator::getNode(long id) {
        return getNode(id, 0, nodes.size());
    }

    element MapGenerator::LandTypeGenerator::getNode(long id, int from, int to) {
        auto midPoint = (to + from) / 2;
        if (nodes[midPoint].id == id) {
            return nodes.at(midPoint);
        }
        if (nodes[midPoint].id > id) {
            return getNode(id, from, midPoint - 1);
        } else {
            return getNode(id, midPoint + 1, to);
        }
    }

}

