#pragma once

 #include <vector>
 #include "util/coloredout.h"
 #include "point.h"

 using std::vector;
 using std::string;
 using std::ostream;
 using std::cout;
 using std::endl;

 /**
  * KDTree class: implemented using Points in Dim dimensional space (given
  * by the template parameter).
  */
 template <int Dim>
 class KDTree
 {
   private:
     /**
      * Internal structure for a node of KDTree.
      * Contains left, right children pointers and a K-dimensional point
      */
     struct KDTreeNode
     {
       Point<Dim> point;
       KDTreeNode *left, *right;

       KDTreeNode() : point(), left(NULL), right(NULL) {}
       KDTreeNode(const Point<Dim> &point) : point(point), left(NULL), right(NULL) {}
     };

   public:
     /**
      * Determines if Point a is smaller than Point b in a given dimension d.
      * If there is a tie, break it with Point::operator<().
      */
     bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second,
                        int curDim) const;

     /**
      * Determines if a Point is closer to the target Point than another
      * reference Point. Takes three points: target, currentBest, and
      * potential, and returns whether or not potential is closer to
      * target than currentBest.
      */
     bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest,
                        const Point<Dim>& potential) const;

     /**
      * Constructs a KDTree from a vector of Points, each having dimension Dim.
      */
     KDTree(const vector<Point<Dim>>& newPoints);


     /**
      * Copy constructor for KDTree.
      *
      * @param other The KDTree to copy.
      */
     KDTree(const KDTree<Dim>& other);

     /**
      * Assignment operator for KDTree.
      *
      * @param rhs The right hand side of the assignment statement.
      * @return A reference for performing chained assignments.
      */
     KDTree const &operator=(const KDTree<Dim>& rhs);

     /**
      * Destructor for KDTree.
      */
     ~KDTree();

     /**
      * Finds the closest point to the parameter point in the KDTree.
      * @return The closest point to a in the KDTree.
      */
     Point<Dim> findNearestNeighbor(const Point<Dim>& query) const;



     void printTree(ostream& out = cout,
                    colored_out::enable_t enable_bold = colored_out::COUT,
                    int modWidth = -1) const;

   private:

     /** Internal representation, root and size **/
     KDTreeNode *root;
     size_t size;

     int getPrintData(KDTreeNode * subroot) const;

     void printTree(KDTreeNode * subroot, std::vector<std::string>& output,
                    int left, int top, int width, int currd) const;

      int _partition(std::vector<Point<Dim>>& list, int left, int right, int pivotIndex, int dim);
      Point<Dim>& _select(std::vector<Point<Dim>>& list, int left, int right, int k, int dim);
      KDTreeNode* _buildTree(std::vector<Point<Dim>>& points, int dim, int left, int right);
      void _clear(KDTreeNode* root);
      KDTreeNode* _copy(KDTreeNode *otherroot);
      Point<Dim> _fnn(const Point<Dim>& query, int dim, KDTreeNode * curroot) const;
 };

 #include "kdtree.hpp"
 #include "kdtree_extras.hpp"
