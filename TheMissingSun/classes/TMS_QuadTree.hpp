#ifndef TMS_QUADTREE_HPP
#define TMS_QUADTREE_HPP

#include "../include/tms.hpp"
#include "TMS_EntityGrid.hpp"

/* Tree structure for quick collision checks. */
class TMS_QuadTree
{
public:
    /* Ids for each quadrant and total number of quadrants. */
    enum class Quad
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT,
        TOT
    };

    /**************** CONSTANTS ****************/
    static constexpr int DEFAULT_DEPTH = 2; // Default depth of the QuadTree.

    /* Node of the tree. */
    struct Node
    {
        tms::Rect levelSpan; // Position and size of the current node.
        std::unique_ptr<Node> quads[static_cast<int>(TMS_QuadTree::Quad::TOT)]; // Children of the current level: 4 quadrants per level.
    };
 
    TMS_QuadTree(const tms::Rect window, const std::unique_ptr<TMS_EntityGrid>& grid, const int depth = DEFAULT_DEPTH); // Build a QuadTree of the specified depth.

    /* Setters. */
    void setWindowPos(const int x, const int y);

private:
    bool _initTree(std::unique_ptr<Node>& currentNode, const int level); // Create and initialise the tree structure.
    void _updateTreeLocation(std::unique_ptr<Node>& currentNode, const int offsetX, const int offsetY);

    int _depth; // Tree depth.
    tms::Rect _window; // Position and size of the window.
    std::unique_ptr<Node> _root; // Root of the tree.
    const std::unique_ptr<TMS_EntityGrid>& _grid; // Ordered grid of game entities.
};

#endif