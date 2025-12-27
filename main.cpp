#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

const int BOARD_SIZE = 10;
const int CELL_SIZE = 60;

enum class Piece { None, White, Black, Arrow };
enum class State { SelectPiece, SelectMove, SelectArrow };

class AmazonsBoard : public QWidget {
public:
    AmazonsBoard(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
        initBoard();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                // Draw Square
                painter.setBrush((r + c) % 2 == 0 ? QColor(240, 217, 181) : QColor(181, 136, 99));
                painter.drawRect(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE);

                // Highlight Selection
                if (state != State::SelectPiece && r == selR && c == selC) {
                    painter.setBrush(QColor(255, 255, 0, 120));
                    painter.drawRect(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                }

                // Draw Contents
                Piece p = board[r][c];
                if (p == Piece::White || p == Piece::Black) {
                    painter.setBrush(p == Piece::White ? Qt::white : Qt::black);
                    painter.setPen(Qt::darkGray);
                    painter.drawEllipse(c * CELL_SIZE + 8, r * CELL_SIZE + 8, CELL_SIZE - 16, CELL_SIZE - 16);
                } else if (p == Piece::Arrow) {
                    painter.setBrush(Qt::red);
                    painter.setPen(Qt::black);
                    painter.drawRect(c * CELL_SIZE + 20, r * CELL_SIZE + 20, CELL_SIZE - 40, CELL_SIZE - 40);
                }
            }
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        int c = event->position().x() / CELL_SIZE;
        int r = event->position().y() / CELL_SIZE;

        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return;

        if (state == State::SelectPiece) {
            if (board[r][c] == currentTurn) {
                selR = r; selC = c;
                state = State::SelectMove;
            }
        } else if (state == State::SelectMove) {
            if (isValidMove(selR, selC, r, c)) {
                board[r][c] = currentTurn;
                board[selR][selC] = Piece::None;
                selR = r; selC = c;
                state = State::SelectArrow;
            } else if (board[r][c] == currentTurn) {
                selR = r; selC = c; // Change selected piece
            }
        } else if (state == State::SelectArrow) {
            if (isValidMove(selR, selC, r, c)) {
                board[r][c] = Piece::Arrow;
                currentTurn = (currentTurn == Piece::White) ? Piece::Black : Piece::White;
                state = State::SelectPiece;
                checkGameOver();
            }
        }
        update();
    }

private:
    Piece board[BOARD_SIZE][BOARD_SIZE];
    Piece currentTurn = Piece::White;
    State state = State::SelectPiece;
    int selR = -1, selC = -1;

    void initBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j) board[i][j] = Piece::None;

        // Starting positions
        board[0][3] = board[0][6] = board[3][0] = board[3][9] = Piece::Black;
        board[6][0] = board[6][9] = board[9][3] = board[9][6] = Piece::White;
    }

    bool isValidMove(int r1, int c1, int r2, int c2) {
        if (r1 == r2 && c1 == c2) return false;
        int dr = (r2 > r1) ? 1 : (r2 < r1 ? -1 : 0);
        int dc = (c2 > c1) ? 1 : (c2 < c1 ? -1 : 0);
        if (abs(r1 - r2) != abs(c1 - c2) && r1 != r2 && c1 != c2) return false;

        int cr = r1 + dr, cc = c1 + dc;
        while (true) {
            if (board[cr][cc] != Piece::None) return false;
            if (cr == r2 && cc == c2) break;
            cr += dr; cc += dc;
        }
        return true;
    }

    void checkGameOver() {
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                if (board[r][c] == currentTurn) {
                    if (canMove(r, c)) return;
                }
            }
        }
        QString winner = (currentTurn == Piece::White) ? "Black" : "White";
        QMessageBox::information(this, "Game Over", winner + " wins!");
        initBoard();
    }

    bool canMove(int r, int c) {
        int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == Piece::None)
                return true;
        }
        return false;
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AmazonsBoard w;
    w.setWindowTitle("Game of the Amazons");
    w.show();
    return a.exec();
}
