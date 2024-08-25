#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <string>

using namespace std;

struct position {
    int x;
    int y;
    position() : x(0), y(0) {}
    position(int x, int y) : x(x), y(y) {}
};

struct Cell {
    string value = " ";
    int turn = 0;
    position cur_pos;
    position pre_pos;
    int status = 0;
};

position getRandPos(int n, int m) {
    int x = rand() % n;
    int y = rand() % m;
    return position(x, y);
}

struct Node {
    Cell data;
    Node* next = nullptr;
};

struct List {
    Node* Head = nullptr;
    Node* Tail = nullptr;
};

Node* createNode(Cell data) {
    Node* p = new Node;
    if (!p)
        return NULL;
    p->data = data;
    return p;
}

List* createList(Node* p) {
    List* tmpLst = new List;
    tmpLst->Head = p;
    tmpLst->Tail = p;
    return tmpLst;
}

void addTail(List* Lst, Node* p) {
    if (Lst->Head == nullptr) {
        Lst->Head = p;
        Lst->Tail = p;
    }
    else {
        Lst->Tail->next = p;
        Lst->Tail = p;
    }
}

void removeHead(List* Lst) {
    if (Lst->Head == nullptr)
        return;
    Node* p_temp = Lst->Head;
    Lst->Head = p_temp->next;
    delete p_temp;
}

void Build(List***& Lst, Cell** grid, int ROW_SIZE, int COLUMN_SIZE) {
    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COLUMN_SIZE; j++) {
            Lst[i][j] = createList(createNode(grid[i][j]));

            int temp_i = i - 1;
            while (temp_i >= 0 && grid[temp_i][j].status == 0) {
                Cell tmp_cell = grid[temp_i][j];
                tmp_cell.pre_pos = position(i, j);
                tmp_cell.turn = grid[i][j].turn;
                addTail(Lst[i][j], createNode(tmp_cell));
                temp_i--;
            }
            temp_i = i + 1;
            while (temp_i < ROW_SIZE && grid[temp_i][j].status == 0) {
                Cell tmp_cell = grid[temp_i][j];
                tmp_cell.pre_pos = position(i, j);
                tmp_cell.turn = grid[i][j].turn;
                addTail(Lst[i][j], createNode(tmp_cell));
                temp_i++;
            }

            int temp_j = j - 1;
            while (temp_j >= 0 && grid[i][temp_j].status == 0) {
                Cell tmp_cell = grid[i][temp_j];
                tmp_cell.pre_pos = position(i, j);
                tmp_cell.turn = grid[i][j].turn;
                addTail(Lst[i][j], createNode(tmp_cell));
                temp_j--;
            }
            temp_j = j + 1;
            while (temp_j < COLUMN_SIZE && grid[i][temp_j].status == 0) {
                Cell tmp_cell = grid[i][temp_j];
                tmp_cell.pre_pos = position(i, j);
                tmp_cell.turn = grid[i][j].turn;
                addTail(Lst[i][j], createNode(tmp_cell));
                temp_j++;
            }

            removeHead(Lst[i][j]);
        }
    }
}

int Turn(Cell a, Cell b) {
    if (a.cur_pos.x != b.cur_pos.x && a.cur_pos.y != b.cur_pos.y) {
        return 1;
    }
    return 0;
}

bool Matching_Game_Algo_Processing(Cell** grid, int ROW_SIZE, int COLUMN_SIZE, Cell start_cell, Cell target_cell) {
    // cout << start_cell.value << " " << target_cell.value << endl;
    if (start_cell.cur_pos.x == target_cell.cur_pos.x && start_cell.cur_pos.y == target_cell.cur_pos.y)
        return 0;

    if (start_cell.status == 0 || target_cell.status == 0)
        return 0;

    if (start_cell.value != target_cell.value)
        return 0;

    List*** Lst = new List * *[ROW_SIZE];
    for (int i = 0; i < ROW_SIZE; i++) {
        Lst[i] = new List * [COLUMN_SIZE];
    }

    grid[target_cell.cur_pos.x][target_cell.cur_pos.y].status = 0;

    Build(Lst, grid, ROW_SIZE, COLUMN_SIZE);
    while (Lst[start_cell.cur_pos.x][start_cell.cur_pos.y]->Head != NULL) {

        Cell Head_Cell = Lst[start_cell.cur_pos.x][start_cell.cur_pos.y]->Head->data;
        // cout << Head_Cell.turn << " test Head" << endl;
        int cur_x = Head_Cell.cur_pos.x, cur_y = Head_Cell.cur_pos.y;
        int pre_x = Head_Cell.pre_pos.x, pre_y = Head_Cell.pre_pos.y;
        cout << cur_x << "," << cur_y << "  -  " << endl;
        cout << pre_x << "," << pre_y << "  +  " << endl;

        /*
        Node* Tmp_Node = Lst[cur_x][cur_y]->Head;
        Node* test_N = Tmp_Node;
        // cout << pre_x << "," << pre_y << endl;
        while (test_N != NULL) {
            if (test_N->data.cur_pos.x == target_cell.cur_pos.x || test_N->data.cur_pos.y == target_cell.cur_pos.y)
                cout << test_N->data.cur_pos.x << "," << test_N->data.cur_pos.y << "  *  ";
            test_N = test_N->next;
        }
        cout << endl;
        

        if (cur_x == target_cell.cur_pos.x && cur_y == target_cell.cur_pos.y && grid[cur_x][cur_y].value == target_cell.value) {
            // cout << "final" << endl;
            if (Head_Cell.turn <= 2) {
                return 1;
            }
        }
        */
        
        Node* Tmp_Node = Lst[cur_x][cur_y]->Head;
        while (Tmp_Node != NULL) {
            if (Tmp_Node->data.cur_pos.x == target_cell.cur_pos.x || Tmp_Node->data.cur_pos.y == target_cell.cur_pos.y) {
                // cout << "bla" << endl;
                Cell Tmp_Cell = Tmp_Node->data;
                Tmp_Cell.turn = Head_Cell.turn + Turn(Tmp_Node->data, grid[pre_x][pre_y]);
                // cout << Head_Cell.turn << " " << Tmp_Cell.turn << " v2" << endl;
                if (Tmp_Cell.turn <= 2) {
                    if (Tmp_Node->data.cur_pos.x == target_cell.cur_pos.x && Tmp_Node->data.cur_pos.y == target_cell.cur_pos.y) {
                        cout << "pos1: " << Tmp_Node->data.pre_pos.x << "," << Tmp_Node->data.pre_pos.y << endl;
                        cout << "pos2: " << pre_x << "," << pre_y << endl;
                        return 1;
                    }
                    addTail(Lst[start_cell.cur_pos.x][start_cell.cur_pos.y], createNode(Tmp_Cell));
                }
            }
            Tmp_Node = Tmp_Node->next;
        }
        removeHead(Lst[start_cell.cur_pos.x][start_cell.cur_pos.y]);
    }

    // Giải phóng bộ nhớ của danh sách liên kết
    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COLUMN_SIZE; j++) {
            Node* current = Lst[i][j]->Head;
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
            delete Lst[i][j];
        }
        delete[] Lst[i];
    }
    delete[] Lst;

    target_cell.status = 1;
    return 0;
}

bool checkGridValid(Cell** grid, int ROW_SIZE, int COLUMN_SIZE) {
    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COLUMN_SIZE; j++) {
            if (grid[i][j].status == 1)
                return 1;
        }
    }
    return 0;
}

int main() {
    srand(time(0));

    int n, m;
    cout << "Enter number of rows: "; cin >> n;
    cout << "Enter number of columns: "; cin >> m;

    int ROW_SIZE = n + 2, COLUMN_SIZE = m + 2;
    int COUNT = n * m; // Chỉ đếm số ô bên trong biên

    // Khởi tạo lưới
    Cell** grid = new Cell * [ROW_SIZE];
    for (int i = 0; i < ROW_SIZE; i++) {
        grid[i] = new Cell[COLUMN_SIZE];
    }

    // Đặt trạng thái ban đầu cho các ô trong lưới
    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COLUMN_SIZE; j++) {
            grid[i][j].status = 0;
            position p(i, j);
            grid[i][j].cur_pos = p;
            grid[i][j].value = " ";
        }
    }

    // Sinh ngẫu nhiên các cặp ký tự
    while (COUNT > 0) {
        char _data = 'A' + rand() % 26;
        string data(1, _data); // Chuyển đổi char thành string với độ dài 1

        int x1, y1, x2, y2;
        do {
            position p = getRandPos(n, m);
            x1 = p.x + 1; y1 = p.y + 1; // Điều chỉnh để nằm trong biên
        } while (grid[x1][y1].status == 1);

        grid[x1][y1].status = 1;
        grid[x1][y1].value = data;

        do {
            position p2 = getRandPos(n, m);
            x2 = p2.x + 1; y2 = p2.y + 1; // Điều chỉnh để nằm trong biên
        } while (grid[x2][y2].status == 1 || (x2 == x1 && y2 == y1));

        grid[x2][y2].status = 1;
        grid[x2][y2].value = data;

        COUNT -= 2;
    }

    // In lưới ra màn hình
    cout << endl;
    for (int i = 1; i <= n; i++) {
        cout << "   ";
        for (int j = 1; j <= m; j++) {
            cout << grid[i][j].value << "  ";
        }
        cout << endl;
    }

    // Nhập dữ liệu cho start_cell và target_cell
    while (checkGridValid(grid, ROW_SIZE, COLUMN_SIZE)) {
        cout << endl;
        int sx, sy, tx, ty;
        cout << "Nhap toa do cho start_cell: ";
        cin >> sx >> sy;
        cout << "Nhap toa do cho target_cell: ";
        cin >> tx >> ty;
        if (Matching_Game_Algo_Processing(grid, ROW_SIZE, COLUMN_SIZE, grid[sx][sy], grid[tx][ty])) {
            grid[sx][sy].value = " ";
            grid[tx][ty].value = " ";
            grid[sx][sy].status = 0;
            grid[tx][ty].status = 0;
            cout << "Matching";
        }
        else
            cout << "Not matching";

        cout << endl;
        for (int i = 1; i <= n; i++) {
            cout << "   ";
            for (int j = 1; j <= m; j++) {
                cout << grid[i][j].value << "  ";
            }
            cout << endl;
        }
    }

    // Giải phóng bộ nhớ
    for (int i = 0; i < ROW_SIZE; i++) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}
