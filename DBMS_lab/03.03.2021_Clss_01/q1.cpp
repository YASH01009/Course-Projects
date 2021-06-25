#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
  // lhs -> rhs
  vector<int> lhs;
  vector<int> rhs;
} FuncDependency;

class Relation {
  int numAttrs;
  vector<FuncDependency> F;
  vector<vector<int>> Decomposition;

  bool checkForAs(vector<vector<string>>& S);
  void processAllFDs(vector<vector<string>>& S);
  void displayS(vector<vector<string>>& S);

 public:
  void fillUpRelation(void);
  void fillUpDecomposition(void);
  bool validateLJProp(void);
};

bool Relation::checkForAs(vector<vector<string>>& S) {
  bool isLJDecomposition;
  for (vector<string> Si : S) {
    isLJDecomposition = true;
    for (string Sij : Si) {
      if (Sij != "a") {
        isLJDecomposition = false;
        break;
      }
    }
    if (isLJDecomposition) return true;
  }
  return false;
}

void Relation::processAllFDs(vector<vector<string>>& S) {
  // iterate over FDs
  bool noChange;
  int numAttrs = this->numAttrs;
  // Repeat until there is no change
  do {
    noChange = true;
    for (FuncDependency& FD : this->F) {
      // FD: lhs -> rhs
      // get all the rows having same values for lhs
      vector<int> sameX(numAttrs, true);
      for (int lhsAttr : FD.lhs)
        for (int i = 0; i < S.size(); ++i)
          if (S[i][lhsAttr] != "a")
            sameX[i] = false;
    
      bool isA;
      for (int rhsAttr : FD.rhs) {
        isA = false;
        for (int i = 0; i < S.size(); ++i) {
          if (!sameX[i]) continue;
          if (S[i][rhsAttr] == "a") {
            isA = true;
            break;
          }
        }
        if (!isA) continue;
        for (int i = 0; i < S.size(); ++i) {
          if (!sameX[i]) continue;
          if (S[i][rhsAttr] != "a") {
            noChange = true;
            S[i][rhsAttr] = "a";
          }
        }
      }
    }
    cout << "After processing for a single time: \n";
    displayS(S);
  } while (!noChange);
}

void Relation::displayS(vector<vector<string>>& S) {
  for (int i = 0; i < S.size(); ++i) {
    for (int j = 0; j < S[i].size(); ++j) {
      cout << setw(2) << S[i][j] << " ";
    }
    cout << "\n";
  }
}

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

void Relation::fillUpDecomposition(void) {
  int numR = 3; // number of relations in the given decomposition
  this->Decomposition.resize(numR);
  vector<int> R0(2), R1(3), R2(3);

  // filling up all the decompositions
  R0[0] = 0, R0[1] = 1;
  R1[0] = 2, R1[1] = 3, R1[2] = 4;
  R2[0] = 0, R2[1] = 2, R2[2] = 5;
  Decomposition[0] = R0;
  Decomposition[1] = R1;
  Decomposition[2] = R2;
}

bool Relation::validateLJProp(void) {
  vector<vector<string>> S(this->Decomposition.size(),
                           vector<string>(this->numAttrs, "b"));
  // making S ready: filling all a's
  for (int i = 0; i < S.size(); ++i)
    for (int attrID : this->Decomposition[i])
      S[i][attrID] = 'a';

  cout << "The matrix S after the initialization: \n"; 
  displayS(S);
  this->processAllFDs(S);
  cout << "The matrix S after processing all the functional dependencies: \n";
  displayS(S);
  return checkForAs(S);
}

int main() {
  Relation R;
  R.fillUpRelation();
  R.fillUpDecomposition();
  cout << "==================================================================\n";
  if (R.validateLJProp()) {
    cout << "==================================================================\n";
    cout << "The given decomposition holds lossless join property.\n";
  } else {
    cout << "==================================================================\n";
    cout << "The given decomposition doesn\'t hold lossless join property.\n";
  }
  cout << "==================================================================\n";
  return 0;
}
