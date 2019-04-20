/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

Point<3> convertToLAB(HSLAPixel pixel) {
    Point<3> result(pixel.h/360, pixel.s, pixel.l);
    return result;
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    // First, use theTiles to build up the KDTree
    vector<Point<3>> treePoints;
    map<Point<3>, int> tile_avg_map;
    for(unsigned i=0; i<theTiles.size(); i++) {
        auto tileImage = theTiles[i];
        auto tilePoint = convertToLAB(tileImage.getAverageColor());
        tile_avg_map[tilePoint] = i;
        treePoints.push_back(tilePoint);
    }

    KDTree<3> tree(treePoints);

    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    for(int row = 0; row < theSource.getRows(); row++)
        for(int col = 0; col < theSource.getColumns(); col++) {
            TileImage* tile = get_match_at_idx(tree, tile_avg_map, theTiles, theSource, row, col);
            canvas->setTile(row, col, tile);
        }

    return canvas;
}

TileImage* get_match_at_idx(const KDTree<3>& tree,
                                  map<Point<3>, int> tile_avg_map,
                                  vector<TileImage>& theTiles,
                                  const SourceImage& theSource, int row,
                                  int col)
{
    // Create a tile which accurately represents the source region we'll be
    // using
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);
    Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

    // Check to ensure the point exists in the map
    map< Point<3>, int >::iterator it = tile_avg_map.find(nearestPoint);
    if (it == tile_avg_map.end())
        cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

    // Find the index
    int index = tile_avg_map[nearestPoint];
    return &theTiles[index];

}
