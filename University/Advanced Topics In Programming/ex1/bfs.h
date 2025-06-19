/**
 * @file bfs.h
 * @brief A file containing basic BFS related functions and declarations.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include <map>
#include <iostream>
#include "vertex.h"
#include "bfs_vertex.h"

class BFS {
    private:
        std::vector<std::pair<int, int>> BFSPath_;
        std::vector<BFS_Vertex> BFSVertices_;
        std::vector<BFS_Vertex> Q_;
        BFS_Vertex S_;
        BFS_Vertex V_;
        bool pathExists_;

    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default Constructor. */
        BFS();
        /** Constructs a new BFS-type object while reserving the given number of spaces for each of the
         * vectors this object has. */
        BFS(int spaces);
        /** Constructor with values for all fields. */
        BFS(const std::vector<std::pair<int, int>> bfs_path, const std::vector<BFS_Vertex> bfs_vertices,
            const std::vector<BFS_Vertex> Q, BFS_Vertex S, BFS_Vertex V, bool path_exists);
        /** Default Copy Constructor. */
        BFS(const BFS& bfs);
        /** Default Move Constructor. */
        BFS(BFS&& bfs) noexcept;
        /** Default Deconstructor. */
        ~BFS() = default;

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        BFS& operator=(const BFS& bfs);
        /** Default Move Assignment Operator. */
        BFS& operator=(BFS&& bfs) noexcept;
        /** Default Equality Operator. */
        bool operator==(const BFS& bfs) const;
        /** Default Inequality Operator. */
        bool operator!=(const BFS& bfs) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the vector of (X, Y) coordinated depicting the BFS path found between tankA's and
         * tankB's positions upon starting the BFS. */
        const std::vector<std::pair<int, int>>& getBFSPath() const;
        /** Gets the vector of BFS vertices depicting all the board's vertices as BFS vertices. */
        const std::vector<BFS_Vertex>& getBFSVertices() const;
        /** Gets the vector of BFS vertices depicting the current BFS queue. */
        const std::vector<BFS_Vertex>& getQ() const;
        /** Gets the BFS vertex depicting tankA's position upon starting the BFS. */
        BFS_Vertex getS() const;
        /** Gets the BFS vertex depicting tankB's position upon starting the BFS. */
        BFS_Vertex getV() const;
        /** Checks if a BFS path exists betwenn tankA's and tankB's position upon starting the BFS. */
        bool pathExists() const;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the vector of (X, Y) coordinated depicting the BFS path found between tankA's and
         * tankB's positions upon starting the BFS to the given one. */
        void setBFSPath(const std::vector<std::pair<int, int>>& bfs_path);
        /** Sets the vector of BFS vertices depicting all the board's vertices as BFS vertices to the
         * given one. */
        void setBFSVertices(const std::vector<BFS_Vertex>& bfs_vertices);
        /** Sets the vector of BFS vertices depicting the current BFS queue to the given one. */
        void setQ(const std::vector<BFS_Vertex>& Q);
        /** Sets the BFS vertex depicting tankA's position upon starting the BFS to the given one. */
        void setS(BFS_Vertex S);
        /** Sets the BFS vertex depicting tankB's position upon starting the BFS to the given one. */
        void setV(BFS_Vertex V);
        /** Sets the BFS path's existance status to the given one. */
        void setPathExistance(bool path_exists);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Adds a singular already existing BFS vertex to the vector of BFS vertices depicting all the
         * board's vertices as BFS vertices */
        void addBFSVertex(BFS_Vertex bfs_vertex);
        /** Updates the BFS fields according to the given input. */
        void updateBFSFields(std::pair<int, int> S_pos, std::pair<int, int> V_pos);
        /** Reconstructs the BFS path (in (X, Y) coordinates) between tankA's and tankB's positions upon
         * starting the BFS. */
        void reconstructBFSPath();
        /** Runs the BFS algorithm between tankA's and tankB's positions. */
        void runBFS(std::pair<int, int> S_pos, std::pair<int, int> V_pos,
            const std::map<std::pair<int, int>, Vertex>& vertices);
        /** Checks if a singular (X, Y) coordinate is on the BFS path. */
        bool isOnPath(std::pair<int, int> pos);
        /** Resets the BFS-type object. */
        void resetBFS() noexcept;
};