/**
 * @file bfs_vertex.h
 * @brief A file depicting the BFS_Vertex class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include <string>
#include <limits>

class BFS_Vertex {
    private:
        std::pair<int, int> vertexPos_;
        std::pair<int, int> parentPos_;
        std::string color_;
        double dis_;
        bool hasParent_;
    
    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default Constructor. */
        BFS_Vertex();
        /** Constructs a BFS vertex at the given (X, Y) coordinates. */
        BFS_Vertex(std::pair<int, int> coords);
        /** Constructor with values for all fields. */
        BFS_Vertex(std::pair<int, int> vertex_pos, std::pair<int, int> parent_pos, std::string color,
                    double dis, bool has_parent);
        /** Default Copy Constructor. */
        BFS_Vertex(const BFS_Vertex& bfs_vertex);
        /** Default Move Constructor. */
        BFS_Vertex(BFS_Vertex&& bfs_vertex) noexcept;
        /** Default Deconstructor. */
        ~BFS_Vertex() = default;

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        BFS_Vertex& operator=(const BFS_Vertex& bfs_vertex);
        /** Default Move Assignment Operator. */
        BFS_Vertex& operator=(BFS_Vertex&& bfs_vertex) noexcept;
        /** Default Equality Operator. */
        bool operator==(const BFS_Vertex& bfs_vertex) const;
        /** Default Inequality Operator. */
        bool operator!=(const BFS_Vertex& bfs_vertex) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the (X, Y) coordinates of the current vertex. */
        std::pair<int, int> getVertexPos() const noexcept;
        /** Gets the (X, Y) coordinates of the current vertex's parent vertex. */
        std::pair<int, int> getParentPos() const noexcept;
        /** Gets the current vertex's color. */
        std::string getColor() const;
        /** Gets the distance between the current vertex and the vertex where tankA was upon starting the
         * BFS. */
        double getDis() const;
        /** Checks if the current vertex has a parent vertex. */
        bool hasParent() const noexcept;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the current vertex's (X, Y) coordinates to the given pair of (X, Y) integers to the
         * given one. Both coordinates must be non-negative. */
        void setVertexPos(std::pair<int, int> pos);
        /** Sets the current vertex's parent vertex's (X, Y) coordinates to the given pair of (X, Y)
         * integers to the given one. Both coordinates must be non-negative. */
        void setParentPos(std::pair<int, int> pos);
        /** Sets the current vertex's color to the given one. */
        void setColor(const std::string& color);
        /** Sets the distance between the current vertex and the vertex where tankA was upon starting the
         * BFS to the given one. Must be non-negative. */
        void setDis(double dis);
        /** Sets the current vertex's "has parent" status to the given one. */
        void setHasParent(bool has_parent);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Validates the color string. */
        bool isValidColor(const std::string& color);
        /** Resets the BFS_Vertex-type object. */
        void resetBFSVertex();
};