#include <iostream>
using namespace std;

// Node structure for linked list
struct Node {
    int row, col, value;
    Node* next;
};

// Sparse Matrix class using linked list
class SparseMatrix {
private:
    int rows, cols;
    Node* head;

public:
    // Constructor
    SparseMatrix(int r, int c) : rows(r), cols(c), head(nullptr) {}

    // Destructor to free memory
    ~SparseMatrix() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Insert a non-zero value at (row, col)
    void insert(int r, int c, int val) {
        if (val == 0) return;

        Node* newNode = new Node{r, c, val, nullptr};

        if (!head || r < head->row || (r == head->row && c < head->col)) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* current = head;
        while (current->next && (current->next->row < r || 
               (current->next->row == r && current->next->col < c))) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }

    // Read non-zero elements from user
    void read(int numNonZero) {
        cout << "Enter non-zero elements (row col value):\n";
        for (int i = 0; i < numNonZero; ++i) {
            int r, c, val;
            cin >> r >> c >> val;
            insert(r, c, val);
        }
    }

    // Display full matrix
    void display() const {
        Node* current = head;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (current && current->row == i && current->col == j) {
                    cout << current->value << " ";
                    current = current->next;
                } else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }

    // Add two sparse matrices
    SparseMatrix add(const SparseMatrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Matrix dimensions must match for addition.");
        }

        SparseMatrix result(rows, cols);
        Node* p = head;
        Node* q = other.head;

        while (p && q) {
            if (p->row < q->row || (p->row == q->row && p->col < q->col)) {
                result.insert(p->row, p->col, p->value);
                p = p->next;
            } else if (q->row < p->row || (p->row == q->row && q->col < p->col)) {
                result.insert(q->row, q->col, q->value);
                q = q->next;
            } else {
                int sum = p->value + q->value;
                if (sum != 0) {
                    result.insert(p->row, p->col, sum);
                }
                p = p->next;
                q = q->next;
            }
        }

        while (p) {
            result.insert(p->row, p->col, p->value);
            p = p->next;
        }

        while (q) {
            result.insert(q->row, q->col, q->value);
            q = q->next;
        }

        return result;
    }
};

// Main function to test the SparseMatrix class
int main() {
    int rows = 4, cols = 5;
    int num1 = 4, num2 = 3;

    SparseMatrix mat1(rows, cols);
    SparseMatrix mat2(rows, cols);

    cout << "Matrix 1:\n";
    mat1.read(num1);

    cout << "Matrix 2:\n";
    mat2.read(num2);

    cout << "\nMatrix 1:\n";
    mat1.display();

    cout << "\nMatrix 2:\n";
    mat2.display();

    SparseMatrix sum = mat1.add(mat2);
    cout << "\nSum Matrix:\n";
    sum.display();

    return 0;
}
