#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QMainWindow>
#include<QPushButton>
#include "game.h"
#include <QTimer>
#include <QMessageBox>
#include <QSoundEffect>
#include <QUrl>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>

enum class gameStatus
{
    Check1, Check2, Pat1, Mat1, Pat2, Mat2, Game
};

namespace Ui
{
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void timerEvent(QTimerEvent *) override;
public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void current_timer_start();
    void timers_stop();
    void end_game();
    int flag;
    void import_history(int winner);
    void change_rate(const QString nick, bool win);
    void import_data_in_db(const QString nick);
    int get_rate_from_db(const QString nickToCheck);
    void setPlayerNamesAndRatings(const QString& name1, const QString& name2);
    Position *from;
    Position *to;
    void check_status(Game* game);
    void print(const Matrix<ChessPiece*> board, Color currentPlayer, Position king);
    void print1(const Board& board, Position* pos);
    void Game_loop();
    void updateEatenPiecesLabel(QLabel* label, const QVector<QPixmap>& pieces);
public slots:
    void exitGame();
    void setupFullScreen();
    void handleButtonClick();
    void pawnPromotion();
    void playMusic();
    void stopMusic();
    void updateBoard()
    {
        print(gamechess->get_board(), gamechess->get_current_color(),  gamechess->get_b().Current_king_pos(gamechess->get_current_color()));
    }

private:
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlQuery *history;
    QSqlTableModel *model;
    QSqlTableModel *model1;

    int player1Rating;
    int player2Rating;

    QLabel* eatenPiecesLabel1;
    QLabel* eatenPiecesLabel2;
    QVector<QPixmap> eatenPieces1;
    QVector<QPixmap> eatenPieces2;

    QLabel *player1TimerLabel;
    QLabel *player2TimerLabel;
    QTimer *player1Timer;
    QTimer *player2Timer;
    int player1TimeRemaining;
    int player2TimeRemaining;
    QDialog *promotionDialog;
    QPushButton *promotionButtons[4];

    gameStatus game_status;
    QLabel* statusLabel;
    QLabel* player1NameLabel;
    QLabel* player2NameLabel;
    QSoundEffect* soundEffect;
    Position* getClickedPosition(QPushButton* clickedButton);
    QString Name_1;
    QString Name_2;
    Ui::GameWindow *ui;
    QPushButton * buttons[8][8];
    Game* gamechess;
    char change;
    QLabel* player1Label;
    QLabel* player2Label;
    QVBoxLayout* player1Layout;
    QVBoxLayout* player2Layout;
    QScrollArea* eatenPiecesPlayer1;
    QScrollArea* eatenPiecesPlayer2;

    void initializePlayerPanels();
    void addEatenPiece(const QString& imagePath, bool isPlayer1);
    void updateEatenPieces();
};

#endif
