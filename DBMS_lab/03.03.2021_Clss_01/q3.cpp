#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

typedef struct {
  // lhs -> rhs
  vector<int> lhs;
  vector<int> rhs;
} FuncDependency;

class Relation {
  int numAttrs;
  vector<int> pk;  
  vector<vector<int>> Decomposition;
  vector<FuncDependency> F;
  string stringifyAttrs(vector<int> attrs);
  void computeAttrsClosure(vector<int>& attrs, vector<FuncDependency>& F,
                           vector<bool>& attrsClosure);

 public:
  void fillUpRelation(void);
  void displayDecomposition(void);
  bool checkBCNF(unordered_set<int>& attrs, unordered_set<int>& X,
                 vector<bool>& XClosure);
  void BCNFDecompose(void);
};

int main() {
  Relation R;
  R.fillUpRelation();
  R.BCNFDecompose();
  R.displayDecomposition();
  return 0;
}

void Relation::fillUpRelation(void) {
  // all attributes are named 0..numAttrs-1
  int numAttrs = 4; // number of attributes in the relational schema R
  int numFDs = 2; // number of functional dependencies on the gieven relational schema R
  this->numAttrs = numAttrs;
  this->F.resize(numFDs);

  // filling up the functional dependencies
  vector<int> X_0(1), X_1(1), Y_0(1), Y_1(1);
  X_0[0] = 0, Y_0[0] = 1;                 // 0 -> 1
  X_1[0] = 2, Y_1[0] = 3;                 // 2 -> 3
  this->F[0].lhs = X_0, this->F[0].rhs = Y_0;
  this->F[1].lhs = X_1, this->F[1].rhs = Y_1;
}

void Relation::BCNFDecompose(void) {
  // Iterate
  queue<vector<int>> decompQueue;
  int iter_num = 0;
  vector<int> decomp;
  for (int i = 0; i < this->numAttrs; ++i) {
    decomp.push_back(i);
  }
  decompQueue.push(decomp);
  while (!decompQueue.empty()) {
    decomp = decompQueue.front();
    iter_num++;
    decompQueue.pop();
    unordered_set<int> decompSet(decomp.begin(), decomp.end());
    // find a problematic FD
    // check all subsets of the attributes
    int decompSize = decomp.size();

    bool violateBCNF = false;
    for (long long i = 0; i <= (long long)pow(2, decompSize); ++i) {
      unordered_set<int> X;
      for (long long j = 0; j < decompSize; ++j) {
        if ((i & (1 << j)) != 0) {
          X.insert(i);
        }
      }
      vector<bool> XClosure(this->numAttrs);
      if (!this->checkBCNF(decompSet, X, XClosure)) {
        vector<int> XUYVec, YVec, QMinYVec;
        for (int id = 0; id < XClosure.size(); ++id) {
          if(decompSet.find(id) == decompSet.end()) continue;
          if (XClosure[id] && X.find(id) == X.end()) {
            XUYVec.push_back(id);
          } else if (X.find(id) != X.end()) {
            XUYVec.push_back(id);
            QMinYVec.push_back(id);
          } else {
            QMinYVec.push_back(id);
          }
        }
        if (XUYVec.size() > 0) decompQueue.push(XUYVec);
        if (QMinYVec.size() > 0) decompQueue.push(QMinYVec);
        violateBCNF = true;
        break;
      }
    }
    if (!violateBCNF) {
      this->Decomposition.push_back(decomp);
    }
  }
}

bool Relation::checkBCNF(unordered_set<int>& attrs, unordered_set<int>& X,
                         vector<bool>& XClosure) {
  int numAttrs = this->numAttrs;
  vector<int> XVec;
  for (int Xattr : X) {
    XVec.push_back(Xattr);
  }
  this->computeAttrsClosure(XVec, this->F, XClosure);
  bool isTrivial = true;
  for (int attrID : attrs) {
    if (X.find(attrID) == X.end() && XClosure[attrID]) {
      isTrivial = false;
      break;
    }
  }

  bool isSuperKey = true;
  for (int attrID : attrs) {
    if (!XClosure[attrID]) {
      isSuperKey = false;
      break;
    }
  }
  return isTrivial || isSuperKey;
}

void Relation::computeAttrsClosure(vector<int>& X, vector<FuncDependency>& F,
                                   vector<bool>& XCover) {
  unordered_set<int> X_plus;
  for (int attr : X) {
    X_plus.insert(attr);
  }
  bool noChange;
  bool isSubset;
  do {
    noChange = true;
    for (FuncDependency& fd : F) {
      // check if fd.lhs is a subset of X_plus
      isSubset = true;
      for (int lhsAttr : fd.lhs) {
        if (X_plus.find(lhsAttr) == X_plus.end()) {
          isSubset = false;
          break;
        }
      }
      if (!isSubset) {
        continue;
      }
      // if lhs is a subset of X_plus
      // X_plus = X_plus union rhs of fd
      for (int rhsAttr : fd.rhs) {
        if (X_plus.find(rhsAttr) == X_plus.end()) {
          noChange = false;
          X_plus.insert(rhsAttr);
        }
      }
    }
  } while (!noChange);
  for (int X_plus_elem : X_plus) {
    XCover[X_plus_elem] = true;
  }
}

void Relation::displayDecomposition(void) {
  cout << "==================================================================\n";
  cout << "BCNF Synthesis with LJ: \n";
  cout << "==================================================================\n";
  for (int i = 0; i < this->Decomposition.size(); ++i) {
    cout << "R" << i << ": (";
    for (int attrID : this->Decomposition[i]) {
      cout << attrID << ",";
    }
    cout << "\b)\n";
  }
  cout << "==================================================================\n";
}
