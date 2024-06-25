#include <iostream>
#include <vector>

int clipMax(int x, int maxX) {
    return (x > 0) ? ((x < maxX) ? x : maxX-1) : 0;
}

bool canTravelTo(std::vector<std::vector<bool>>& gameMatrix, int fromRow, int fromColumn, int toRow, int toColumn)
{
    int maxY = gameMatrix.size();
    int maxX = gameMatrix[0].size();
    int bbox_ulx = clipMax(fromColumn - 1, maxX);
    int bbox_uly = clipMax(fromRow - 1, maxY);
    int bbox_drx = clipMax(fromColumn + 2, maxX);
    int bbox_dry = clipMax(fromRow + 1, maxY);

    if (toColumn > bbox_drx || toColumn < bbox_ulx || toRow > bbox_dry || toRow < bbox_uly) 
        return false;
    
    int dx = toColumn-fromColumn;
    int dy = toRow - fromRow;

    if ((dx == 0) && (dy >= -1 && dy <= 1)) {
        return gameMatrix[toRow][toColumn];
    }
    if ((dy == 0) && (dx >= -1 && dx <= 2)) {
        if (toColumn < fromColumn) 
            std::swap(toColumn, fromColumn);
        for (auto i = fromColumn; i < toColumn; ++i) {
            if (!gameMatrix[toRow][i]) 
                return false;
        }
        return gameMatrix[toRow][toColumn];
    }

    return false;
    
}

#ifndef RunTests
int main()
{
  std::vector<std::vector<bool>> gameMatrix = {
      {false, true,  true,  false, false, false},
      {true,  true,  true,  false, false, false},
      {true,  true,  true,  true,  true,  true},
      {false, true,  true,  false, true,  true},
      {false, true,  true,  true,  false, true},
      {false, false, false, false, false, false},
  };

  std::cout << canTravelTo(gameMatrix, 3, 2, 2, 2) << std::endl; // true, Valid move
  std::cout << canTravelTo(gameMatrix, 3, 2, 3, 4) << std::endl; // false, Can't travel through land
  std::cout << canTravelTo(gameMatrix, 3, 2, 6, 2) << std::endl; // false, Out of bounds
}
#endif