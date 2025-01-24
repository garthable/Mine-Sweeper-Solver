#ifndef MINESWEEPERSOLVER
#define MINESWEEPERSOLVER

#include <stdint.h>
#include <string>
#include <array>
#include <vector>

#include "mineSweeperSolutionFinder.hpp"

struct Coordinate
{
    uint16_t x;
    uint16_t y;

    Coordinate(uint16_t _x, uint16_t _y)
    {
        x = _x;
        y = _y;
    }
};

// Runs the mine sweeper solving algorithm
// Optimized to be able to solve multiple games back to back of the same difficulty running.
// Receives input of map via update
// Gives output of next click via getReccomendedClick
class MineSweeperSolver
{
    public:
        // Parameters:
        // The length of the mine sweeper map
        // The height of the mine sweeper map
        // The number of bombs in the mine sweeper map
        MineSweeperSolver(const uint16_t& sizeX, const uint16_t& sizeY, const uint16_t& bombCount);

        // Parameters:
        // The current mine sweeper map output
        //
        // Purpose:
        // Runs logic to find probability of nodes
        void update(const std::string& mineSweeperMap);

        // Returns:
        // Returns what the solver sees for bug testing.
        std::string getSolverMap();

        // Purpose:
        // Resets solver for new game
        void reset(const uint16_t& bombCount);

        // Returns:
        // A location with one of the lowest probabilities of being a bomb
        Coordinate getReccomendedClick();

        // Returns:
        // A location that has a 100% chance of being a bomb
        Coordinate getReccomendedFlag();

    private:
        // Parameters:
        // The x coordinate of a potential solver tile location
        // The y coordinate of a potential solver tile location
        //
        // Returns:
        // A pointer to the solver tile with coordinates (x, y)
        // If a solver tile with said coords does not exist it returns the null pointer
        inline SolverTile* searchSolverTile(const uint16_t& x, const uint16_t& y);

        // Parameters:
        // The index of a solverTile
        //
        // Returns:
        // A pointer to the solver tile with coordinates (x, y)
        // If a solver tile with said coords does not exist it returns the null pointer
        inline SolverTile* searchSolverTile(const uint16_t& index);

        // Parameters:
        // Pointer to solver tile
        //
        // Returns:
        // Index of solverTile in solverTiles
        inline uint16_t getSolverTileIndex(SolverTile* solverTilePtr);

        // Returns:
        // Whether reccomendedFlags and reccomendedClicks are empty
        inline bool hasReccomendedSolverTiles();

        // Parameters:
        // out Solvertile to be flagged
        //
        // Outs:
        // Sets inputed tile to a flag and adds it to reccomended flags
        inline void flagSolverTile(SolverTile& solverTileOut);

        // Parameters:
        // out Solvertile to be clicked
        //
        // Outs:
        // Sets inputed tile to clicked and adds it to reccomended clicks
        inline void clickSolverTile(SolverTile& solverTileOut);

        // Purpose:
        // Generates the tiles and adds the pointers between them.
        void generateSolverTiles();

        // Parameters:
        // The current mine sweeper map output
        //
        // Purpose:
        // Parses input from mineSweeper class to derive current game state
        void readMineMap(const std::string& mineSweeperMap);

        // Parameters:
        // solverTilePtr
        //
        // Returns:
        // Number of non flagged bombs
        inline uint16_t getEffectiveBombCount(const SolverTile& solverTilePtr);

        // Parameters:
        // solverTilePtr
        //
        // Returns:
        // Number of unkown adjacent tiles
        inline uint16_t getAdjUnknownCount(const SolverTile& solverTilePtr);

        // Purpose:
        // Preforms checks that are in p time.
        void fastBombFinder();

        // Parameters:
        // Two solver tiles
        //
        // Returns:
        // The number of common unknown adjacents
        uint16_t getIntersectionSizeBetweenTileUnknownAdjs(const SolverTile* solverTilePtrA, const SolverTile* solverTilePtrB);

        // Parameters:
        // Two solver tiles
        //
        // Purpose:
        // Clicks all unknown tiles of A that are not shared by B
        void clickTilesInAMinusBSet(SolverTile* solverTilePtrA, SolverTile* solverTilePtrB);

        // Parameters:
        // Two solver tiles
        //
        // Purpose:
        // Flags all unknown tiles of A that are not shared by B
        void flagTilesInAMinusBSet(SolverTile* solverTilePtrA, SolverTile* solverTilePtrB);

        // Parameters:
        // Two solver tiles
        //
        // Purpose:
        // Flags all unknown tiles of A that are shared by B
        void flagTilesInAIntersectBSet(SolverTile* solverTilePtrA, SolverTile* solverTilePtrB);

        // Purpose:
        // Finds 1 2 x pattern
        // Finds 1 1 x pattern
        void patternBombFinder();

        // Purpose:
        // Reduces the amount of tiles in getAllSolutions function which is NP by grouping tiles.
        void groupTiles();

        // Parameters:
        // Next solvertile
        // 
        // Purpose:
        // Preforms DFS to group tiles together
        void groupTilesReccursion(SolverTile* currTilePtr, const uint16_t& group);

        // Parameters:
        // Two unknown/not visible tiles
        //
        // Purpose:
        // Makes sure both tiles have a numbered tile in common.
        bool shareSameNumbered(const SolverTile* tileA, const SolverTile* tileB);

        // Purpose:
        // Gets every possible arrangement of mines.
        void getAllSolutions();

        // Parameters:
        // Solver Tile
        //
        // Returns:
        // Probability that tile has zero adjacent bombs
        double getProbOfHavingNoAdjBombs(const SolverTile& solverTile);

        // Purpose:
        // Clicks unknown with lowest prob
        void clickLowestProb();
    private:
        enum solverTileStates
        {
            unknown = 0u,
            visible = 1u,
            flagged = 2u,
            clicked = 4u
        };

        uint16_t m_sizeX;
        uint16_t m_sizeY;
        uint16_t m_bombCount;
        uint16_t m_unknownTileCount;

        svec::SVector<SolverTile, MAX_TILE_COUNT> m_solverTiles;

        svec::SVector<SolverTile*, MAX_RECCOMENDED> m_reccomendedClicks;
        svec::SVector<SolverTile*, MAX_RECCOMENDED> m_reccomendedFlags;

        GroupedSolverTiles m_groupedVisibleSolverTiles;
        GroupedSolverTiles m_groupedHiddenSolverTiles;
};

#endif