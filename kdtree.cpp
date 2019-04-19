/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    return first.vals[curDim]<second.vals[curDim];
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    // First work out the Euclidean distance
    double pEuc = 0, curEuc;
    for(int i=0; i<Dim; i++) {
        pEuc += (potential.vals[i]-target.vals[i])*(potential.vals[i]-target.vals[i]);
        curEuc += (currentBest.vals[i]-target.vals[i])*(currentBest.vals[i]-target.vals[i]);
    }

    return pEuc < curEuc;
}

template <int Dim>
int partition(const vector<Point<Dim>>& pList, int start, int end, int d) {
    if(start==end)
        return start;
    Point<Dim> pivot = pList[start];
    while(start < end) {
        while(start < end && pList[end].vals[d] > pivot.vals[d])
            end--;
        pList[start] = pList[end];
        while(start < end && pList[start].vals[d] < pivot.vals[d])
            start++;
        pList[end] = pList[start];
    }
    pList[start] = pivot;
    return start;
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
Point<Dim> quickSelect(const vector<Point<Dim>>& pList, int start, int end, int k, int d) {
    while(start<=end) {
        int parIdx = partition(pList, start, end, d);
        if(start+k == parIdx)
            return pList[parIdx];
        else if (start+k < parIdx)
            end = parIdx-1;
        else
            start = parIdx+1;
    }
    return Point<Dim>();
}

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
    KDTreeBuild(root, newPoints, 0, newPoints.size()-1, 0);
}

template <int Dim>
void KDTree<Dim>::KDTreeBuild(KDTreeNode*& subroot, const vector<Point<Dim>> &newPoints, int a, int b, int d) {
    if(a>b)
        return;
    Point<Dim> median = quickSelect(newPoints, a, b, (b-a)/2, d);
    if(subroot == NULL) {
        subroot = new KDTreeNode(median);
    } else {
        subroot->point = median;
    }
    int m = (a+b)/2;
    KDTreeBuild(subroot->left, newPoints, a, m-1, (d+1)%Dim);
    KDTreeBuild(subroot->right, newPoints, m+1, b, (d+1)%Dim);
}


template <int Dim>
KDTree<Dim>::KDTree(const KDTree& other) {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs) {
  /**
   * @todo Implement this function!
   */

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

    return Point<Dim>();
}

