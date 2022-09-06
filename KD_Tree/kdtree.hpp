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
      if (first[curDim] < second[curDim]) {
        return true;
      } else if (first[curDim] == second[curDim]) {
        return first < second;
      }
     return false;
 }

 template <int Dim>
 bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                 const Point<Dim>& currentBest,
                                 const Point<Dim>& potential) const
 {
     /**
      * @todo Implement this function!
      */
      double a = 0.0;
      double b = 0.0;
      for (int i = 0; i < Dim; i++) {
        a += (target[i]-currentBest[i])*(target[i]-currentBest[i]);
        b += (target[i]-potential[i])*(target[i]-potential[i]);
      }
      if (b < a) {
        return true;
      } else if (a == b) {
        return potential < currentBest;
      }
      return false;
 }

 template <int Dim>
 KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
 {
     /**
      * @todo Implement this function!
      */
    std::vector<Point<Dim>> points = newPoints;
    size = newPoints.size();
    root = _buildTree(points, 0, 0, points.size() - 1);
 }

 template <int Dim>
 KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
   /**
    * @todo Implement this function!
    */
    size = other.size;
    root = _copy(other.root);
    
 }

 template <int Dim>
 const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
   /**
    * @todo Implement this function!
    */
    if (this == &rhs) {
      return *this;
    }
    _clear();
    size = rhs.size;
    _copy(rhs.root);
   return *this;
 }

 template <int Dim>
 KDTree<Dim>::~KDTree() {
   /**
    * @todo Implement this function!
    */
    _clear(root);
    size = 0;
 }

 template <int Dim>
 Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
 {
     /**
      * @todo Implement this function!
      */
     return _fnn(query, 0, root);
 }


template <int Dim>
int KDTree<Dim>::_partition(std::vector<Point<Dim>>& list, int left, int right, int pivotIndex, int dim) {
  Point<Dim> pivotvalue = list.at(pivotIndex);
  list.at(pivotIndex) = list.at(right);
  list.at(right) = pivotvalue;
  int storeindex = left;
  for (int i = left; i <= right - 1; i++) {
    if (smallerDimVal(list.at(i), pivotvalue, dim)) {
      Point<Dim> inter = list.at(storeindex);
      list.at(storeindex) = list.at(i);
      list.at(i) = inter;
      storeindex++;
    }
  }
  Point<Dim> inter = list.at(storeindex);
  list.at(storeindex) = list.at(right);
  list.at(right) = inter;
  return storeindex;
}


template <int Dim>
Point<Dim>& KDTree<Dim>::_select(std::vector<Point<Dim>>& list, int left, int right, int k, int dim) {
  while (true) {
    if (left == right) {
      return list.at(left);
    }
    int pivotIndex = right;
    pivotIndex = _partition(list, left, right, pivotIndex, dim);
    if (k == pivotIndex) {
      return list.at(k);
    } else if (k < pivotIndex) {
      right = pivotIndex - 1;
    } else {
      left = pivotIndex + 1;
    }
  }
}


template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::_buildTree(std::vector<Point<Dim>>& points, int dim, int left, int right) {
  KDTreeNode* newroot = NULL;
  if (left <= right) {
    int middle = (left + right) / 2;
    newroot = new KDTreeNode(_select(points, left, right, middle, dim));
    newroot->left = _buildTree(points, (dim + 1) % Dim, left, middle - 1);
    newroot->right = _buildTree(points, (dim + 1) % Dim, middle + 1, right);
  }
  return newroot;
}


template <int Dim>
void KDTree<Dim>::_clear(KDTreeNode* root) {
  if (root == NULL) {
    return;
  }
  _clear(root->left);
  _clear(root->right);
  delete root;
  root = NULL;
}


template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::_copy(KDTreeNode *otherroot) {
  if (root == NULL) {
    return;
  }
  KDTreeNode *root = new KDTreeNode(otherroot->point);
  root->left = _copy(otherroot->left);
  root->right = _copy(otherroot->right);
  return root;
}


template <int Dim>
Point<Dim> KDTree<Dim>::_fnn(const Point<Dim>& query, int dim, KDTreeNode * curroot) const {
  Point<Dim> nearest = curroot->point;
  // if been to left
	bool been;
	if (curroot->left == NULL && curroot->right == NULL) {
    return curroot->point;
  }
	if (!smallerDimVal(query, curroot->point, dim)) {
    if (curroot->right == NULL)  {
			nearest = _fnn(query, (dim + 1) % Dim, curroot->left);
    } else  {
			nearest = _fnn(query, (dim + 1) % Dim, curroot->right);
		  been = false;
    }
	} else {
    if (curroot->left == NULL) {
			nearest = _fnn(query, (dim + 1) % Dim, curroot->right);
    } else  {
			nearest = _fnn(query, (dim + 1) % Dim, curroot->left);
		  been = true;
    }
    //
	}
	if (shouldReplace(query, nearest, curroot->point)) {
    nearest = curroot->point;
  }
  // get point distance
	double radius = 0;
	for (int i = 0; i < Dim; i++) {
		radius += (query[i] - nearest[i]) * (query[i] - nearest[i]);
	}
  // get the spliting line distance
	double splitDist = curroot->point[dim] - query[dim];
	splitDist = splitDist * splitDist;

	if (radius >= splitDist) {
		KDTreeNode * tempNearest;
    if (been) {
      tempNearest = curroot->right;
    } else {
      tempNearest = curroot->left;
    }
		if (tempNearest != NULL) {
			Point<Dim> pol = _fnn(query, (dim + 1) % Dim, tempNearest);
			if (shouldReplace(query, nearest, pol)) {
        nearest = pol;
      }
		}
	}
	return nearest;
}