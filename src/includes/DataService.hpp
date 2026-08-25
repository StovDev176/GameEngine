#pragma once
#include <fstream>
#include "GridService.hpp"
#include <variant>
#include "ECS.hpp"

struct WorldFileHeader {
    uint32_t magicNumber;
    uint32_t version;

    uint32_t size;
    uint32_t height;
    uint16_t cellSize;
    uint16_t cellHeight;
    vector3 origin;

};

bool SaveWorld(const std::string& filepath, const Grid& grid, ComponentPool<CellComponent>* cellComponent) {
    if (!cellComponent) return false;

    std::ofstream saveFile(filepath, std::ios::binary);
    if (!saveFile.is_open()) return false;

    WorldFileHeader wfh;
    wfh.magicNumber = 0x4D475244;
    wfh.version = 1;
    wfh.size = grid.size;
    wfh.height = grid.height;
    wfh.cellSize = grid.cellSize;
    wfh.cellHeight = grid.cellHeight;
    wfh.origin = grid.origin;

    saveFile.write(reinterpret_cast<const char*>(&wfh), sizeof(wfh));

    size_t elementCount = cellComponent->dense.size();
    saveFile.write(reinterpret_cast<const char*>(&elementCount), sizeof(elementCount));

    size_t dataSizeBytes = elementCount * sizeof(cellComponent);
    saveFile.write(reinterpret_cast<const char*>(cellComponent->dense.data()), dataSizeBytes);

    saveFile.close();
    return true;
}

bool LoadWorld(const std::string& filepath, Grid& outGrid, ComponentPool<CellComponent>* outCellComponent) {
    if (!outCellComponent) return false;

    std::ifstream saveFile(filepath, std::ios::binary);
    if (!saveFile.is_open()) return false;

    WorldFileHeader wfh;
    saveFile.read(reinterpret_cast<char*>(&wfh), sizeof(wfh));

    if (wfh.magicNumber != 0x4D475244 || wfh.version != 1) {
        saveFile.close();
        return false; 
    }

    outGrid.size = wfh.size;
    outGrid.height = wfh.height;
    outGrid.cellSize = wfh.cellSize;
    outGrid.cellHeight = wfh.cellHeight;
    outGrid.origin = wfh.origin;

    size_t elementCount = 0;
    saveFile.read(reinterpret_cast<char*>(&elementCount), sizeof(elementCount));

    outCellComponent->dense.resize(elementCount);

    size_t dataSizeBytes = elementCount * sizeof(CellComponent);
    saveFile.read(reinterpret_cast<char*>(outCellComponent->dense.data()), dataSizeBytes);

    saveFile.close();
    return true; 
}