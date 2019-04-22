/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include "kdtree.h"

using namespace std;

/** * This function judge if value in the first point is smaller than the second. if it is smaller, return true.**/
template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
     if(first[curDim] != second[curDim])
        return first[curDim]<second[curDim];
     return first<second;
}

/** * This function calculate the distance between two points. **/
template <int Dim>
double KDTree<Dim>::calcEucDis(const Point<Dim> &current, const Point<Dim> &target) const {
    double ans = 0;
    for(int i=0; i<Dim; i++) {
        ans+=(current[i]-target[i])*(current[i]-target[i]);
    }
    return ans;
}

/** * This function judge if the distance of target point is smaller than that of currentBest point **/
template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    // First let's work out the Euclidean distance
    double pEuc = calcEucDis(potential, target), curEuc = calcEucDis(currentBest, target);
    if(pEuc != curEuc)
        return pEuc < curEuc;
    return potential<currentBest;
}

/** * This function swap two points. **/
template <int Dim>
void pSwap(Point<Dim>& p1, Point<Dim>&p2) {
    Point<Dim> tmp = p1;
    p1 = p2;
    p2 = tmp;
}


/** * This function partition the list and the standard is the middle point of original list. **/
/** the result list is smaller points before the standard points, larger points behind the standard points**/
template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& pList, int start, int end, int d) {
    int pivotIndex = (start+end)/2;
    Point<Dim> pivotVal = pList[pivotIndex];
    pSwap(pList[pivotIndex], pList[end]);
    int storeIndex = start;
    for(int i=start; i<end; i++) {
        if(smallerDimVal(pList[i], pivotVal, d)) {
            pSwap(pList[storeIndex], pList[i]);
            storeIndex++;
        }
    }
    pSwap(pList[end], pList[storeIndex]);
    return storeIndex;
}

/**
 * Find the kth Point in pList[l,r]
 * @tparam Dim
 * @param pList
 * @param k
 * @param start
 * @param end
 * @return
 */
template <int Dim>
Point<Dim> KDTree<Dim>::quickSelect(vector<Point<Dim>>& pList, int start, int end, int k, int d) {
    while(start!=end) {
        int parIdx = partition(pList, start, end, d);
        if(k == parIdx)
            return pList[parIdx];
        else if (k < parIdx)
            end = parIdx-1;
        else
            start = parIdx+1;
    }
    return pList[start];
}

/** * This function buld a KDtree. **/
template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    /**
     * Several steps:
     * 0. a(the lower bound) b(the upper bound)
     * 1. quick select the median point with respect to dimension d
     * 2. put the point on m=(a+b)/2, such that [a,m-1] vd<rd  and [m+1,b] vd>rd
     * 3. create a root based on median, and using (d+1)%k recursively build for [a,m-1] and [m+1,b]
     *
     */
     root=NULL;
     auto newPointsCopy = newPoints;
    KDTreeBuild(root, newPointsCopy, 0, newPointsCopy.size()-1, 0);
}

/** * the helper function to build the KDtree. **/
template <int Dim>
void KDTree<Dim>::KDTreeBuild(KDTreeNode*& subroot, vector<Point<Dim>> &newPoints, int a, int b, int d) {
    if(a>b)
        return;
    Point<Dim> median = quickSelect(newPoints, a, b, (a+b)/2, d);
    if(subroot == NULL) {
        subroot = new KDTreeNode(median);
    } else {
        subroot->point = median;
    }
    int m = (a+b)/2;
    KDTreeBuild(subroot->left, newPoints, a, m-1, (d+1)%Dim);
    KDTreeBuild(subroot->right, newPoints, m+1, b, (d+1)%Dim);
}

/** * the copy funtion. **/
template <int Dim>
KDTree<Dim>::KDTree(const KDTree& other) : root(copy_(other)), size(other.size) {
  /**
   * @todo Implement this function!
   */
}

/** * overload the assign operator**/
template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs) {
  /**
   * @todo Implement this function!
   */
    if(&rhs != this) {
        clear_(root);
        size = rhs.size;
        root = copy_(rhs.root);
    }
  return *this;
}

/** *helper function of copy function. **/
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy_(const KDTreeNode* subroot) {
    if(subroot == NULL) {
        return NULL;
    }
    KDTreeNode* newNode = new KDTreeNode(subroot);
    newNode->left = copy_(subroot->left);
    newNode->right = copy_(subroot->right);
    return newNode;
}

/** helper function of destruct function. **/
template <int Dim>
void KDTree<Dim>::clear_(KDTree<Dim>::KDTreeNode *subroot) {
    if(subroot == NULL) {
        return;
    }
    clear_(subroot->left);
    clear_(subroot->right);
    delete subroot;
}

/** *destruct function. **/
template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   clear_(root);
}

/** * This function find the nearest point according to the giving point. **/
template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    /**
     * 1. First find the matching leaf node
     * 2. back traversal to check for other nodes
     */
    return findNearestNeighbor_(query, root, 0);
}

/** * the helper function of findNearestNeighbor function. **/
template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor_(const Point<Dim> &query, const KDTree<Dim>::KDTreeNode *subroot, int d) const {
    Point<Dim> currentBest;
    if(smallerDimVal(query, subroot->point, d)) {
        // If on d, target is smaller than current node, then should be on left subtree
        if(subroot->left == NULL) {
            // No child on left, we are done
            return subroot->point;
        }
        currentBest = findNearestNeighbor_(query, subroot->left, (d+1)%Dim);
    } else {
        // be on right subtree
        if(subroot->right == NULL) {
            // No right child, we are done
            return subroot->point;
        }
        currentBest = findNearestNeighbor_(query, subroot->right, (d+1)%Dim);
    }
    if(shouldReplace(query, currentBest, subroot->point)) {
        // Current subroot is better
        currentBest = subroot->point;
    }

    // Then work on the subtree of another side plane

    // The condition whether work out another subtree?
    // (val1[d]-val2[d])**2 <= radis

    double radis = calcEucDis(query, currentBest);
    double panelD = (query[d]-subroot->point[d])*(query[d]-subroot->point[d]);

    if(panelD <= radis) {
        // We need do some searchings here
        if(smallerDimVal(query, subroot->point, d)) {
            // We have searched left, now try right
            if(subroot->right != NULL) {
                auto subPoint = findNearestNeighbor_(query, subroot->right, (d+1)%Dim);
                if(shouldReplace(query, currentBest, subPoint)) {
                    currentBest = subPoint;
                }
            }
        } else {
            if(subroot->left != NULL) {
                auto subPoint = findNearestNeighbor_(query, subroot->left, (d+1)%Dim);
                if(shouldReplace(query, currentBest, subPoint)) {
                    currentBest = subPoint;
                }
            }
        }
    }
    return currentBest;
}
