#include <iostream>
#include <set>
#include <vector>


#include "cs225/PNG.h"
#include "maptiles.h"
#include "mosaiccanvas.h"
#include "sourceimage.h"
#include "util/util.h"

using namespace std;
using namespace util;
using namespace cs225;

void makePhotoMosaic(const string& inFile, const string& tileDir, int numTiles,
                     int pixelsPerTile, const string& outFile);
vector<TileImage> getTiles(string tileDir);
bool hasImageExtension(const string& fileName);

namespace opts
{
    bool help = false;
}


template <int K>
void _test_linear_constructor(int size) {
    vector<Point<K>> points;
    points.reserve(size);
    for (int i = 0; i < size; i++) {
        Point<K> p;
        for (int j = 0; j < K; j++)
            p[j] = i;
        points.push_back(p);
    }

    KDTree<K> tree(points);

    tree.printTree(cout);

}

template <int K>
void _test_linear_nearestNeighbor(int size) {
    vector<Point<K>> points;
    points.reserve(size);
    for (int i = 0; i < size; i++) {
        Point<K> p;
        for (int j = 0; j < K; j++)
            p[j] = i;
        points.push_back(p);
    }

    KDTree<K> tree(points);

//    cout << tree.findNearestNeighbor(points[]) << endl;
    for (int i = 0; i < size; i++) {
        cout << tree.findNearestNeighbor(points[i]) << "  " << points[i] <<endl;
    }
}

template <int Dim>
class MineActionFAIL : public Point<Dim>::MineAction {
public:
    void onMine(const Point<Dim> & point) const {
        // only fail if trigger is set;
        // gets set after construction and before findNN call
        if (trigger)
            cout<<"The point " << point << " should not be visited during this query."<<endl;
    }

    bool trigger = false;
};

void testcase2() {
    double coords[20][2] = {{84, 44},  // mine
                            {74, 0},   // mine
                            {54, 62},  // mine
                            {59, 0},   // mine
                            {34, 15},  // mine
                            {42, 63},
                            {96, 56},  // mine
                            {44, 79},
                            {44, 43},
                            {28, 10},  // mine
                            {60, 30},  // mine
                            {88, 72},  // mine
                            {75, 68},  // mine
                            {43, 65},
                            {48, 0},   // mine
                            {14, 15},  // mine
                            {49, 83},
                            {51, 35},
                            {95, 50},  // mine
                            {82, 20}}; // mine
    bool isMine[20] = {1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1};
    double targetCoords[2] = {45, 59};
    double expectedCoords[2] = {42, 63};


    vector<Point<2>> points;
    MineActionFAIL<2> action;
    for (int i = 0; i < 20; ++i)
        points.push_back(Point<2>(coords[i], isMine[i], &action));
    Point<2> target(targetCoords);
    Point<2> expected(expectedCoords);

    KDTree<2> tree(points);

    tree.printTree(cout);

    action.trigger = true;
    cout<<tree.findNearestNeighbor(target)<<endl;
//    REQUIRE(tree.findNearestNeighbor(target) == expected);
}
int main(int argc, const char** argv)
{
//    testcase2();
//    _test_linear_nearestNeighbor<1>(10);
//    _test_linear_constructor<1>(10);
//    _test_linear_constructor<3>(31);
    string inFile = "";
    string tileDir = "mp5_pngs/";
    string numTilesStr = "100";
    string pixelsPerTileStr = "50";
    string outFile = "mosaic.png";

    OptionsParser optsparse;
    optsparse.addArg(inFile);
    optsparse.addArg(tileDir);
    optsparse.addArg(numTilesStr);
    optsparse.addArg(pixelsPerTileStr);
    optsparse.addArg(outFile);
    optsparse.addOption("help", opts::help);
    optsparse.addOption("h", opts::help);
    optsparse.parse(argc, argv);

    if (opts::help) {
        cout << "Usage: " << argv[0]
             << " background_image.png tile_directory/ [number of tiles] "
                "[pixels per tile] [output_image.png]"
             << endl;
        return 0;
    }

    if (inFile == "") {
        cout << "Usage: " << argv[0]
             << " background_image.png tile_directory/ [number of tiles] "
                "[pixels per tile] [output_image.png]"
             << endl;
        return 1;
    }

    makePhotoMosaic(inFile, tileDir, lexical_cast<int>(numTilesStr),
                    lexical_cast<int>(pixelsPerTileStr), outFile);

    return 0;
}

void makePhotoMosaic(const string& inFile, const string& tileDir, int numTiles,
                     int pixelsPerTile, const string& outFile)
{
    PNG inImage;
    inImage.readFromFile(inFile);
    SourceImage source(inImage, numTiles);
    vector<TileImage> tiles = getTiles(tileDir);

    if (tiles.empty()) {
        cerr << "ERROR: No tile images found in " << tileDir << endl;
        exit(2);
    }

    MosaicCanvas::enableOutput = true;
    MosaicCanvas* mosaic = mapTiles(source, tiles);
    cerr << endl;

    if (mosaic == NULL) {
        cerr << "ERROR: Mosaic generation failed" << endl;
        exit(3);
    }

    PNG result = mosaic->drawMosaic(pixelsPerTile);
    cerr << "Saving Output Image... ";
    result.writeToFile(outFile);
    cerr << "Done" << endl;
    delete mosaic;
}

vector<TileImage> getTiles(string tileDir)
{
#if 1
    if (tileDir[tileDir.length() - 1] != '/')
        tileDir += '/';

    vector<string> allFiles = get_files_in_dir(tileDir);
    sort(allFiles.begin(), allFiles.end());

    vector<string> imageFiles;
    imageFiles.reserve(allFiles.size());
    for (size_t i = 0; i < allFiles.size(); i++)
        if (hasImageExtension(allFiles[i]))
            imageFiles.push_back(allFiles[i]);

    vector<TileImage> images;
    set<HSLAPixel> avgColors;
    for (size_t i = 0; i < imageFiles.size(); i++) {
        cerr << "\rLoading Tile Images... ("
             << (i + 1) << "/" << imageFiles.size()
             << ")" << string(20, ' ') << "\r";
        cerr.flush();
        PNG png;
        png.readFromFile(imageFiles.at(i));
        TileImage next(png);
        if (avgColors.count(next.getAverageColor()) == 0) {
            avgColors.insert(next.getAverageColor());
            images.push_back(next);
        }
    }
    cerr << "\rLoading Tile Images... ("
         << imageFiles.size() << "/" << imageFiles.size()
         << ")";
    cerr << "... " << images.size() << " unique images loaded" << endl;
    cerr.flush();

    return images;
#else
    PNG temp;
    vector<TileImage> tiles;
    *temp(0, 0) = RGBAPixel(0,   0,   0);   tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(0,   0,   127); tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(0,   0,   255); tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(0,   127, 0);   tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(0,   255, 0);   tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(127, 0,   0);   tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(255, 0,   0);   tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(0,   127, 127); tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(0,   255, 255); tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(127, 0,   127); tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(255, 0,   255); tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(127, 127, 0);   tiles.push_back(TileImage(temp));
    *temp(0, 0) = RGBAPixel(255, 255, 0);   tiles.push_back(TileImage(temp));
    return tiles;
#endif
}

bool hasImageExtension(const string& fileName)
{
    size_t dotpos = fileName.find_last_of(".");
    if (dotpos == string::npos)
        return false;
    string ext = toLower(fileName.substr(dotpos + 1));
    return (ext == "bmp" || ext == "png" || ext == "jpg" || ext == "gif"
            || ext == "tiff");
}
