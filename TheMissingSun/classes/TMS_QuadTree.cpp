#include "TMS_QuadTree.hpp"

TMS_QuadTree::TMS_QuadTree(const tms::Rect window, const std::unique_ptr<TMS_EntityGrid>& grid, const int depth) :
    _window(window),
    _grid(grid)
{
    /* If depth is invalid, use the default depth. */
    if (depth > 0) _depth = depth;
    else _depth = DEFAULT_DEPTH;

    /* If grid is invalid, throw an exception. */
    if (grid == nullptr) throw "Cannot use a nullptr EntityGrid.\n";

    /* Create the tree. */
    /* Initialise the root. */
    _root = std::unique_ptr<Node>(new Node());
    _root->levelSpan = _window;
    /* Create the rest of the tree. */
    if (!_initTree(_root, 0)) throw "Error while creating QuadTree.\n";
}

void TMS_QuadTree::setWindowPos(const int x, const int y)
{
    int offsetX = x - _window.x;
    int offsetY = y - _window.y;
    _window.x = x;
    _window.y = y;

    _updateTreeLocation(_root, offsetX, offsetY);
}

bool TMS_QuadTree::_initTree(std::unique_ptr<Node>& currentNode, const int level)
{
    /* Base case, maximum depth reached. */
    if (level == _depth)
    {
        for (auto& quadrant : currentNode->quads)
        {
            quadrant = nullptr;
        }
        return true;
    }

    /* Compute new width and height. */
    int newWidth = static_cast<int>(currentNode->levelSpan.w / 2.0f);
    int newHeight = static_cast<int>(currentNode->levelSpan.h / 2.0f);
    if (newWidth == 0 || newHeight == 0) return false; // Do not accept quadrants with no size.

    /* Compute new coordinates. */
    int newX = currentNode->levelSpan.x + newWidth;
    int newY = currentNode->levelSpan.y + newHeight;

    /* Create each quadrant, set its span and move on recursively. */
    for (int i = 0; i < static_cast<int>(Quad::TOT); ++i)
    {
        currentNode->quads[i] = std::unique_ptr<Node>(new Node()); // Create the new node.
        /* Set new dimensions. */
        currentNode->quads[i]->levelSpan.w = newWidth;
        currentNode->quads[i]->levelSpan.h = newHeight;

        /* Set new positions. */
        switch (static_cast<Quad>(i))
        {
        case Quad::TOP_LEFT:
            currentNode->quads[i]->levelSpan.x = currentNode->levelSpan.x;
            currentNode->quads[i]->levelSpan.y = currentNode->levelSpan.y;
            break;
        case Quad::TOP_RIGHT:
            currentNode->quads[i]->levelSpan.x = newX;
            currentNode->quads[i]->levelSpan.y = currentNode->levelSpan.y;
            break;
        case Quad::BOTTOM_RIGHT:
            currentNode->quads[i]->levelSpan.x = newX;
            currentNode->quads[i]->levelSpan.y = newY;
            break;
        case Quad::BOTTOM_LEFT:
            currentNode->quads[i]->levelSpan.x = currentNode->levelSpan.x;
            currentNode->quads[i]->levelSpan.y = newY;
            break;
        }

        if (!_initTree(currentNode, level + 1)) return false; // Recursive call.
    }

    return true;
}

void TMS_QuadTree::_updateTreeLocation(std::unique_ptr<Node>& currentNode, const int offsetX, const int offsetY)
{
    /* Update the position of the current node. */
    currentNode->levelSpan.x += offsetX;
    currentNode->levelSpan.y += offsetY;

    /* If there are children, update them as well. */
    if (currentNode->quads[0] != nullptr)
    {
        for (auto& quadrant : currentNode->quads)
        {
            _updateTreeLocation(quadrant, offsetX, offsetY);
        }
    }
}
