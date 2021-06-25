#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
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
  void findClosureOfAttr(vector<int>& attrs, vector<FuncDependency>& F,
                           vector<bool>& attrsCover);
  bool isSuperKey(vector<int>& attrs);
  vector<int> primaryKey(void);
  void minCoverFD(void);
  string stringifyAttrs(vector<int> attrs);
  bool checkCover(vector<FuncDependency>& F1, vector<FuncDependency>& F2);
  bool checkEqui(vector<FuncDependency>& F1, vector<FuncDependency>& F2);

 public:
  void fillUpRelation(void);
  void displayDecomposition(void);
  void syn3NFwithDP(void);
};

void Relation::fillUpRelation(void) {
  // all attributes are named 0..numAttrs-1
  int numAttrs = 6; // number of attributes in the relational schema R
  int numFDs = 3; // number of functional dependencies on the gieven relational schema R
  this->numAttrs = numAttrs;
  this->F.resize(numFDs);

  // filling up the functional dependencies
  vector<int> X_0(1), X_1(1), X_2(2), Y_0(1), Y_1(2), Y_2(1);
  X_0[0] = 0, Y_0[0] = 1;                 // 0 -> 1
  X_1[0] = 2, Y_1[0] = 3, Y_1[1] = 4;     // 2 -> 3,4
  X_2[0] = 0, X_2[1] = 2, Y_2[0] = 5;     // 0,2 -> 5
  this->F[0].lhs = X_0, this->F[0].rhs = Y_0;
  this->F[1].lhs = X_1, this->F[1].rhs = Y_1;
  this->F[2].lhs = X_2, this->F[2].rhs = Y_2;
}

void Relation::displayDecomposition(void) {
  cout << "==================================================================\n";
  cout << "3NF Synthesis with DP: \n";
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

void Relation::syn3NFwithDP(void) {
  // Find minCover
  this->minCoverFD();
  unordered_map<string, vector<int>> orgAttrs;
  unordered_map<string, unordered_set<int>> rhs;
  for (FuncDependency fd : this->F) {
    string attrStr = this->stringifyAttrs(fd.lhs);
    if (orgAttrs.find(attrStr) == orgAttrs.end()) {
      orgAttrs.insert(make_pair(attrStr, fd.lhs));
    }
    for (int attr : fd.rhs) {
      rhs[attrStr].insert(attr);
    }
  }

  unordered_set<string> prevDecomps;
  bool hasKey = false;
  for (auto orgAttr : orgAttrs) {
    vector<int> Ri;
    for (int attrID : orgAttr.second) {
      Ri.push_back(attrID);
    }
    for (int attrID : rhs[orgAttr.first]) {
      Ri.push_back(attrID);
    }
    string attrStr = this->stringifyAttrs(Ri);
    if (prevDecomps.find(attrStr) == prevDecomps.end()) {
      this->Decomposition.push_back(Ri);
    }
    prevDecomps.insert(attrStr);
    if (this->isSuperKey(Ri)) {
      hasKey = true;
    }
  }
  if (!hasKey) {
    vector<int> PK = this->primaryKey();
    this->Decomposition.push_back(PK);
  }
  this->displayDecomposition();
}

int main() {
  Relation R;
  R.fillUpRelation();
  R.syn3NFwithDP();
  return 0;
}

void Relation::findClosureOfAttr(vector<int>& X, vector<FuncDependency>& F,
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

// Return true if F1 covers F2
bool Relation::checkCover(vector<FuncDependency>& F1,
                         vector<FuncDependency>& F2) {
  int numAttrs = this->numAttrs;
  for (FuncDependency& fd1 : F1) {
    vector<bool> XF1Cover(numAttrs, false);
    vector<bool> XF2Cover(numAttrs, false);
    this->findClosureOfAttr(fd1.lhs, F1, XF1Cover);
    this->findClosureOfAttr(fd1.lhs, F2, XF2Cover);

    // XF1Cover must include XF2Cover
    for (int i = 0; i < numAttrs; ++i) {
      if (XF2Cover[i] == true && XF1Cover[i] == false) {
        return false;
      }
      if (XF1Cover[i] == true && XF2Cover[i] == false) {
        return false;
      }
    }
  }
  return true;
}

bool Relation::checkEqui(vector<FuncDependency>& F1, vector<FuncDependency>& F2) {
  return (checkCover(F1, F2) && checkCover(F2, F1));
}

string Relation::stringifyAttrs(vector<int> attrs) {
  string str = "";
  for (int attr : attrs) {
    str = str + "_" + to_string(attr);
  }
  return str;
}

vector<int> Relation::primaryKey(void) {
  int numAttrs = this->numAttrs;
  vector<int> PK(numAttrs);

  for (int i = 0; i < numAttrs; ++i) {
    PK[i] = i;
  }
  // try removing one attribute at a time
  int removedAttr;
  for (vector<int>::iterator it = PK.begin(); it != PK.end();) {
    removedAttr = *it;
    PK.erase(it);
    if (!this->isSuperKey(PK)) {
      it = PK.insert(it, removedAttr);
      it++;
    }
  }
  return PK;
}

bool Relation::isSuperKey(vector<int>& attrs) {
  int numAttrs = this->numAttrs;
  vector<bool> AClosure(numAttrs, false);
  this->findClosureOfAttr(attrs, this->F, AClosure);
  for (int i = 0; i < numAttrs; ++i) {
    if (!AClosure[i]) return false;
  }
  return true;
}

// Updates F = minCover(F)
void Relation::minCoverFD(void) {
  // Make a deep copy of F and delete it
  vector<FuncDependency> Forg = this->F;
  this->F.clear();

  // make sure there is only one attr on the rhs of each fd
  FuncDependency newFD;
  for (FuncDependency& fd : Forg) {
    newFD.lhs = fd.lhs;
    for (int attrID : fd.rhs) {
      newFD.rhs = {attrID};
      this->F.push_back(newFD);
    }
  }

  // Removing extraneous attributes
  int removedAttr;
  for (FuncDependency& fd : this->F) {
    for (auto it = fd.lhs.begin(); it != fd.lhs.end();) {
      removedAttr = *it;
      fd.lhs.erase(it);
      if (!this->checkEqui(this->F, Forg)) {
        it = fd.lhs.insert(it, removedAttr);
        it++;
      }
    }
  }

  // Remove one fd and check it the closure changes
  FuncDependency remFD;
  for (auto it = this->F.begin(); it != this->F.end();) {
    remFD = *it;
    F.erase(it);
    if (!this->checkEqui(this->F, Forg)) {
      it = this->F.insert(it, remFD);
      it++;
    }
  }
}
