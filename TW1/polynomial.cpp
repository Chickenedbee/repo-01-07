#include <iostream>
#include <cmath>
#include <unordered_map>
using namespace std;

// 節點結構
struct Node {
    int coef; // 系數
    int exp;  // 指數
    Node* link; // 指向下一個節點的指標
};

// 多項式類別
class Polynomial {
private:
    Node* head; // 頭節點

    // 新增節點到可用空間列表
    void ReturnNode(Node* node) {
        node->link = availableNodes;
        availableNodes = node;
    }

    // 從可用空間列表獲取節點
    static Node* GetNode() {
        if (!availableNodes) {
            return new Node;
        }
        Node* node = availableNodes;
        availableNodes = availableNodes->link;
        return node;
    }

    static Node* availableNodes; // 可用空間列表

public:
    Polynomial() {
        head = new Node;
        head->link = head;
    }

    // 輸入運算子重載
    friend istream& operator>>(istream& is, Polynomial& x);

    // 輸出運算子重載
    friend ostream& operator<<(ostream& os, Polynomial& x);

    // 複製建構子
    Polynomial(const Polynomial& a);

    // 賦值運算子重載
    const Polynomial& operator=(const Polynomial& a);

    // 解構子
    ~Polynomial();

    // 多項式相加
    Polynomial operator+(const Polynomial& b) const;

    // 多項式相減
    Polynomial operator-(const Polynomial& b) const;

    // 多項式相乘
    Polynomial operator*(const Polynomial& b) const;

    // 計算多項式值
    float Evaluate(float x) const;
};

Node* Polynomial::availableNodes = nullptr;

// 輸入運算子實作
istream& operator>>(istream& is, Polynomial& x) {
    int n, c, e;
    Node* temp;
    Node* prev = x.head; // 用於插入新節點的前一節點指標
    is >> n;
    for (int i = 0; i < n; i++) {
        is >> c >> e;
        temp = x.GetNode();
        temp->coef = c;
        temp->exp = e;
        
        // 找到正確的插入位置
        Node* current = x.head->link;
        while (current != x.head && current->exp > e) {
            prev = current;
            current = current->link;
        }
        
        // 插入節點
        temp->link = current;
        prev->link = temp;
        prev = x.head; // 重置 prev 指向頭節點
    }
    return is;
}


// 輸出運算子實作
ostream& operator<<(ostream& os, Polynomial& x) {
    Node* temp = x.head->link;
    while (temp != x.head) {
        os << temp->coef << "x^" << temp->exp << " ";
        temp = temp->link;
    }
    return os;
}

// 複製建構子實作
Polynomial::Polynomial(const Polynomial& a) {
    head = new Node; // 創建新的頭節點
    head->link = head;
    Node* temp = a.head->link; 
    
    while (temp != a.head) { // 遍歷源多項式的每個節點
        Node* newNode = GetNode(); // 從可用空間列表中獲取新的節點
        newNode->coef = temp->coef; // 複製系數,指數
        newNode->exp = temp->exp; 
        newNode->link = head->link; // 將新節點插入到新多項式的鏈表中
        head->link = newNode; // 更新頭節點的鏈接
        temp = temp->link; // 移動到下一個節點
    }
}


// 賦值運算子實作
const Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {
        this->~Polynomial();
        new (this) Polynomial(a);
    }
    return *this;
}

// 解構子實作
Polynomial::~Polynomial() {
    Node* temp = head->link;
    while (temp != head) {
        Node* toDelete = temp;
        temp = temp->link;
        ReturnNode(toDelete);
    }
    delete head;
}

// 多項式相加實作
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result;
    Node* p1 = head->link;
    Node* p2 = b.head->link;
    Node* p3 = result.head;
    while (p1 != head && p2 != b.head) {
        Node* temp = GetNode();
        if (p1->exp == p2->exp) {
            temp->coef = p1->coef + p2->coef;
            temp->exp = p1->exp;
            p1 = p1->link;
            p2 = p2->link;
        } else if (p1->exp > p2->exp) {
            temp->coef = p1->coef;
            temp->exp = p1->exp;
            p1 = p1->link;
        } else {
            temp->coef = p2->coef;
            temp->exp = p2->exp;
            p2 = p2->link;
        }
        temp->link = result.head;
        p3->link = temp;
        p3 = temp;
    }
    while (p1 != head) {
        Node* temp = GetNode();
        temp->coef = p1->coef;
        temp->exp = p1->exp;
        temp->link = result.head;
        p3->link = temp;
        p3 = temp;
        p1 = p1->link;
    }
    while (p2 != b.head) {
        Node* temp = GetNode();
        temp->coef = p2->coef;
        temp->exp = p2->exp;
        temp->link = result.head;
        p3->link = temp;
        p3 = temp;
        p2 = p2->link;
    }
    return result;
}

// 多項式相減實作
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial result;
    Node* p1 = head->link;
    Node* p2 = b.head->link;
    Node* p3 = result.head;
    while (p1 != head && p2 != b.head) {
        Node* temp = GetNode();
        if (p1->exp == p2->exp) {
            temp->coef = p1->coef - p2->coef;
            temp->exp = p1->exp;
            p1 = p1->link;
            p2 = p2->link;
        } else if (p1->exp > p2->exp) {
            temp->coef = p1->coef;
            temp->exp = p1->exp;
            p1 = p1->link;
        } else {
            temp->coef = -p2->coef;
            temp->exp = p2->exp;
            p2 = p2->link;
        }
        temp->link = result.head;
        p3->link = temp;
        p3 = temp;
    }
    while (p1 != head) {
        Node* temp = GetNode();
        temp->coef = p1->coef;
        temp->exp = p1->exp;
        temp->link = result.head;
        p3->link = temp;
        p3 = temp;
        p1 = p1->link;
    }
    while (p2 != b.head) {
        Node* temp = GetNode();
        temp->coef = -p2->coef;
        temp->exp = p2->exp;
        temp->link = result.head;
        p3->link = temp;
        p3 = temp;
        p2 = p2->link;
    }
    return result;
}

Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial result;
    Node* p1 = head->link;
    Node* p2 = nullptr;
    
    // 使用哈希表來儲存相同次方項的系數和避免重複計算
    unordered_map<int, int> terms; // 次方 -> 系數

    while (p1 != head) {
        p2 = b.head->link;
        while (p2 != b.head) {
            int expSum = p1->exp + p2->exp;
            int coefProd = p1->coef * p2->coef;
            terms[expSum] += coefProd;
            p2 = p2->link;
        }
        p1 = p1->link;
    }

    // 將哈希表中的項目轉換回多項式鏈表
    for (auto& term : terms) {
        Node* newNode = GetNode();
        newNode->coef = term.second;
        newNode->exp = term.first;

        // 插入到結果多項式中按次方由大到小排序
        Node* prev = result.head;
        Node* current = result.head->link;
        while (current != result.head && current->exp < newNode->exp) {
            prev = current;
            current = current->link;
        }
        newNode->link = current;
        prev->link = newNode;
        prev = result.head;
    }
    

    return result;
}


// 計算多項式值實作
float Polynomial::Evaluate(float x) const {
    float result = 0;
    Node* temp = head->link;
    while (temp != head) {
        result += temp->coef * pow(x, temp->exp);
        temp = temp->link;
    }
    return result;
}

int main() { 
    Polynomial p1, p2, p3; 

    cout << "輸入多項式1 : "; 
    cin >> p1; 
    cout << "多項式1: " << p1 << endl; 
    cout << "輸入多項式2 : "; 
    cin >> p2; 
    cout << "多項式2: " << p2 << endl;

    p3 = p1 + p2; cout << "多項式1 + 多項式2: " << p3 << endl; 
    p3 = p1 - p2; cout << "多項式1 - 多項式2: " << p3 << endl; 
    p3 = p1 * p2; cout << "多項式1 * 多項式2: " << p3 << endl; 

    float x = 2.0; 
    cout << "多項式1 在 x = " << x << " 的值: " << p1.Evaluate(x) << endl; 
    
    return 0;
}