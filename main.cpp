#include "scop.hpp"
#include <cstdint>

int main(int ac, char **av)
{
    std::vector<Vertex> finalVertices;
    std::vector<uint32_t> finalIndices;
    if (ac != 2)
        return 1;
    try {
    parse(av[1], finalVertices, finalIndices);
    }
    catch (ParsingException e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "=== PARSING TERMINÉ ===" << std::endl;
    std::cout << "Nombre total de vertices : " << finalVertices.size() << std::endl;
    std::cout << "Nombre total d'indices  : " << finalIndices.size() << std::endl;
    std::cout << "=======================\n" << std::endl;
    return 0;
}