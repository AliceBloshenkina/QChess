#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QLabel>
#include <QPixmap>
#include <QtDebug>
#include <QTimer>
#include <QString>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QPainter>
#include <QCoreApplication>
#include <QDir>
#define lose -5
#define win 15

void GameWindow::timerEvent(QTimerEvent *)
{
    if(flag == 0)
    {
        print(gamechess->get_board(), gamechess->get_current_color(), gamechess->get_b().Current_king_pos(gamechess->get_current_color()));
        flag++;
    }
}

void GameWindow::setupFullScreen()
{
    this->setWindowState(Qt::WindowFullScreen | Qt::WindowMaximized);
    this->setGeometry(QGuiApplication::primaryScreen()->geometry());
}

void setLabelBackground(QLabel* label, const QString& imagePath)
{
    QPixmap background(imagePath);

    if (background.isNull())
    {
        return;
    }

    if (background.hasAlpha())
    {
        label->setPixmap(background);
        label->setScaledContents(true);
    }
}

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    QPixmap background(QCoreApplication::applicationDirPath() + "/img/brick.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);
    game_status = gameStatus::Game;
    ui->setupUi(this);
    gamechess = new Game("Loh", 10, "lol", 15);
    flag = 0;
    from = new Position(-1,-1);
    to = new Position(-1,-1);
    Name_1 = "White";
    Name_2 = "Black";
    const int gridSize = 8;
    const int buttonSize = 80;
    const int xOffset = (width() - gridSize * buttonSize) / 2;
    const int yOffset = (height() - gridSize * buttonSize) / 2;
    statusLabel = new QLabel("Готов", this);
    statusLabel->setGeometry(xOffset, yOffset + gridSize * buttonSize + 50, gridSize * buttonSize, 30);
    statusLabel->setStyleSheet("background-color: lightgray; border: 1px solid gray;");
    statusLabel->setAlignment(Qt::AlignCenter);
    QString labelStyleSheet = "color: white; font-weight: bold;";

    for (int col = 0; col < gridSize; ++col)
    {
        QLabel *topLabel = new QLabel(QString(QChar('A' + col)), this);
        topLabel->setFixedSize(buttonSize, 30);
        topLabel->move(xOffset + col * buttonSize, yOffset - 30);
        topLabel->setAlignment(Qt::AlignCenter);
        topLabel->setStyleSheet(labelStyleSheet);

        QLabel *bottomLabel = new QLabel(QString(QChar('A' + col)), this);
        bottomLabel->setFixedSize(buttonSize, 30);
        bottomLabel->move(xOffset + col * buttonSize, yOffset + gridSize * buttonSize);
        bottomLabel->setAlignment(Qt::AlignCenter);
        bottomLabel->setStyleSheet(labelStyleSheet);
    }

    for (int row = 0; row < gridSize; ++row)
    {
        QLabel *leftLabel = new QLabel(QString::number(8 - row), this);
        leftLabel->setFixedSize(30, buttonSize);
        leftLabel->move(xOffset - 30, yOffset + row * buttonSize);
        leftLabel->setAlignment(Qt::AlignCenter);
        leftLabel->setStyleSheet(labelStyleSheet);

        QLabel *rightLabel = new QLabel(QString::number(8 - row), this);
        rightLabel->setFixedSize(30, buttonSize);
        rightLabel->move(xOffset + gridSize * buttonSize, yOffset + row * buttonSize);
        rightLabel->setAlignment(Qt::AlignCenter);
        rightLabel->setStyleSheet(labelStyleSheet);
    }

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(buttonSize, buttonSize);
            button->move(xOffset + col * buttonSize, yOffset + row * buttonSize);
            if ((row + col) % 2 == 0)
            {
                button->setStyleSheet("background-color: #D3D3D3");
            } else {
                button->setStyleSheet("background-color: #2F4F4F");
            }
            if(gamechess->get_board()[row][col])
            {
            QPixmap pixmap(gamechess->get_board()[row][col]->get_file());
            QIcon icon(pixmap);
            button->setIcon(icon);
            int iconSize = static_cast<int>(buttonSize * 0.8);
            button->setIconSize(QSize(iconSize, iconSize));
            }
            buttons[row][col] = button;
            connect(buttons[row][col], &QPushButton::clicked, this, &GameWindow::handleButtonClick);
        }
    }

player1NameLabel = new QLabel("Игрок1:\n" + Name_1 + " ", this);
player1NameLabel->setGeometry(xOffset - 200, yOffset - 30, 150, 200);
player1NameLabel->setStyleSheet("color: silver; font-weight: bold; font-size: 16px; text-align: center;");
player1NameLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);

player2NameLabel = new QLabel("Игрок2:\n" + Name_2, this);
player2NameLabel->setGeometry(xOffset + gridSize * buttonSize + 50, yOffset - 30, 150, 200);
player2NameLabel->setStyleSheet("color: silver; font-weight: bold; font-size: 16px; text-align: center;");
player2NameLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);

player1TimerLabel = new QLabel("30:00", this);
player1TimerLabel->setGeometry(xOffset - 200, yOffset + gridSize * buttonSize - 550, 150, 30);
player1TimerLabel->setStyleSheet("color: red; font-weight: bold; text-align: center; font-size: 20px; font-family: 'Courier New';letter-spacing: 4px;");
player1TimerLabel->setAlignment(Qt::AlignCenter);

player1Timer = new QTimer(this);
player1TimeRemaining = 1800;

connect(player1Timer, &QTimer::timeout, [=]() {
    player1TimeRemaining--;

    if (player1TimeRemaining <= 0)
    {
        statusLabel->setText("Игрок " + Name_1 + " проиграл по времени");
        change_rate(Name_1, false);
        change_rate(Name_2, true);
        import_history(2);
        end_game();
    }

    QString elapsedString = QString("%1:%2")
                                .arg(player1TimeRemaining / 60, 2, 10, QChar('0'))
                                .arg(player1TimeRemaining % 60, 2, 10, QChar('0'));

    player1TimerLabel->setText(elapsedString);
});

player2TimerLabel = new QLabel("30:00", this);
player2TimerLabel->setGeometry(xOffset + gridSize * buttonSize + 50, yOffset + gridSize * buttonSize - 550, 150, 30);
player2TimerLabel->setStyleSheet("color: red; font-weight: bold; text-align: center;font-size: 20px; font-family: 'Courier New';letter-spacing: 4px;");
player2TimerLabel->setAlignment(Qt::AlignCenter);

player2Timer = new QTimer(this);
player2TimeRemaining = 1800;

connect(player2Timer, &QTimer::timeout, [=]() {
    player2TimeRemaining--;

    if (player2TimeRemaining <= 0) {
        statusLabel->setText("Игрок " + Name_2 + " проиграл по времени");
        change_rate(Name_2, false);
        change_rate(Name_1, true);
        import_history(1);
        end_game();
    }

    QString elapsedString = QString("%1:%2")
                                .arg(player2TimeRemaining / 60, 2, 10, QChar('0'))
                                .arg(player2TimeRemaining % 60, 2, 10, QChar('0'));

    player2TimerLabel->setText(elapsedString);
});

    int a = startTimer(0);
    soundEffect = new QSoundEffect(this);

    soundEffect->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/music/Another_love.wav"));

    soundEffect->setVolume(0.5);
    playMusic();
    promotionDialog = new QDialog(this);
    promotionDialog->setWindowTitle("Выберите фигуру для превращения");

    QGridLayout *promotionLayout = new QGridLayout(promotionDialog);

    QString figureImages[4] = {"rook.png", "horse.png", "bishop.png", "queen.png"};

    QString figureImages1[4] = {"rook1.png", "horse1.png", "bishop1.png", "queen1.png"};

    for (int i = 0; i < 4; ++i)
    {
        promotionButtons[i] = new QPushButton(promotionDialog);
        promotionButtons[i]->setFixedSize(80, 80);
        if(gamechess->get_current_color() == Color::BLACK)
        {
            QString imagePath = QCoreApplication::applicationDirPath() + "/img/" + figureImages[i];
            QPixmap pixmap(imagePath);
            promotionButtons[i]->setIcon(QIcon(pixmap));

        } else {
            QString imagePath = QCoreApplication::applicationDirPath() + "/img/" + figureImages1[i];
            QPixmap pixmap(imagePath);

            promotionButtons[i]->setIcon(QIcon(pixmap));

        }

        promotionButtons[i]->setIconSize(QSize(80, 80));

        connect(promotionButtons[i], &QPushButton::clicked, this, &GameWindow::pawnPromotion);

        promotionLayout->addWidget(promotionButtons[i], i / 2, i % 2);
    }
    QTimer::singleShot(0, this, &GameWindow::setupFullScreen);
    QMenu *fileMenu = menuBar()->addMenu(tr("Меню"));
    QAction *exitAction = new QAction(tr("Выход"), this);
    connect(exitAction, &QAction::triggered, this, &GameWindow::exitGame);
    fileMenu->addAction(exitAction);
    eatenPiecesLabel1 = new QLabel(this);
    eatenPiecesLabel1->setGeometry(xOffset - 200, yOffset + gridSize * buttonSize - 480, 150, 450);
    eatenPiecesLabel1->setAlignment(Qt::AlignTop);
    eatenPiecesLabel1->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");

    eatenPiecesLabel2 = new QLabel(this);
    eatenPiecesLabel2->setGeometry(xOffset + gridSize * buttonSize + 50, yOffset + gridSize * buttonSize - 480, 150, 450);
    eatenPiecesLabel2->setAlignment(Qt::AlignTop);
    eatenPiecesLabel2->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");

    current_timer_start();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Player.db");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Players(Nick TEXT UNIQUE, Rate INT);");
    model = new QSqlTableModel(this, db);
    model->setTable("Players");
    model->select();
    history = new QSqlQuery(db);
    history->exec("CREATE TABLE History(NICK1 TEXT, NICK2 TEXT, WINNER TEXT);");
    model1 = new QSqlTableModel(this, db);
    model1->setTable("History");
    model1->select();
}

void GameWindow::updateEatenPiecesLabel(QLabel* label, const QVector<QPixmap>& pieces)
{
    int maxRows = 10;
    int maxCols = 2;
    int imageSpacing = 5;
    int imageWidth = 50;
    int imageHeight = 50;

    QPixmap combinedPixmap(label->size());
    combinedPixmap.fill(Qt::transparent);

    QPainter painter(&combinedPixmap);

    int yOffset = 0;
    int col = 0;
    int row = 0;

    for (const QPixmap& piece : pieces)
    {
        painter.drawPixmap(col * (imageWidth + imageSpacing), row * (imageHeight + imageSpacing), piece);
        yOffset += piece.height() + imageSpacing;

        ++col;
        if (col >= maxCols)
        {
            col = 0;
            ++row;
        }

        if (row >= maxRows)
        {
            break;
        }
    }

    label->setPixmap(combinedPixmap);
}

void GameWindow::current_timer_start()
{
    if(gamechess->get_current_color() == Color::WHITE)
    {
        player1Timer->start(1000);
    } else {
        player2Timer->start(1000);
    }
}

void GameWindow::timers_stop()
{
    player1Timer->stop();
    player2Timer->stop();
}

void GameWindow::exitGame()
{
    this->close();
}

void GameWindow::playMusic()
{
    soundEffect->play();
}

void GameWindow::stopMusic()
{
    soundEffect->stop();
}

void GameWindow::handleButtonClick()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            if (clickedButton == buttons[row][col])
            {
                if(flag == 1)
                {
                    from->setRow(row);
                    from->setCol(col);
                    print1(gamechess->get_b(), from);
                    flag++;
                } else if(flag == 2) {
                    to->setRow(row);
                    to->setCol(col);
                    if(from->getRow() == to->getRow() && from->getCol() == to->getCol())
                    {
                        print(gamechess->get_board(), gamechess->get_current_color(), gamechess->get_b().Current_king_pos(gamechess->get_current_color()));
                        flag--;
                    } else {
                    Position _from(from->getRow(), from->getCol());
                    Position _to(to->getRow(), to->getCol());
                    if(gamechess->get_board()[to->getRow()][to->getCol()])
                    {
                        QString imagePath = gamechess->get_board()[to->getRow()][to->getCol()]->get_file() + ".png";
                        if (gamechess->get_current_color() == Color::WHITE)
                        {
                            QPixmap pixmap(imagePath);
                            eatenPieces1.append(pixmap);
                            updateEatenPiecesLabel(eatenPiecesLabel1, eatenPieces1);
                        } else {
                            QPixmap pixmap(imagePath);
                            eatenPieces2.append(pixmap);
                            updateEatenPiecesLabel(eatenPiecesLabel2, eatenPieces2);
                        }
                    }
                    gamechess->get_b().set_piece(_from, _to);
                    if(gamechess->get_board()[to->getRow()][to->getCol()] &&
                        gamechess->get_board()[to->getRow()][to->getCol()]->get_name() == 'P'
                        && (to->getRow() == 0 || to->getRow() == 7))
                    {
                        promotionDialog->setWindowFlags(promotionDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
                        promotionDialog->setAttribute(Qt::WA_DeleteOnClose, false);
                        promotionDialog->show();
                        promotionDialog->exec();
                        if(gamechess->get_board()[to->getRow()][to->getCol()])
                        {

                            gamechess->get_b().del_from_vector(_to,Color_opposite::opposite(gamechess->get_current_color()));
                            delete gamechess->get_b().getBoard()[to->getRow()][to->getCol()];
                            gamechess->get_b().getBoard()[to->getRow()][to->getCol()] = nullptr;
                        }


                        gamechess->get_b().getBoard()[to->getRow()][to->getCol()] = gamechess->get_b().createChessPiece(change, gamechess->get_current_color(), to->getRow(), to->getCol());

                    }


                    timers_stop();

                    gamechess->set_current_color();

                    current_timer_start();
                    check_status(gamechess);
                    print(gamechess->get_board(), gamechess->get_current_color(), gamechess->get_b().Current_king_pos(gamechess->get_current_color()));

                    flag--;
                    }

                }
                return;
            }
        }
    }
}

void GameWindow::pawnPromotion()
{
    qDebug() << "Change!!!!!!!!!!!!!!!!!!!!!!!";
    char names[4] = {'R', 'H', 'B', 'Q'};
    for (int i = 0; i < 4; ++i) {
        if (sender() == promotionButtons[i]) {
            // promotionDialog->hide();
            change = names[i];
            promotionDialog->hide();
            return;
        }
    }
}

void GameWindow::import_history(int winner){
    qDebug() << "import_history called. Winner:" << winner;
    QString draw = "Draw";
    int row = model1->rowCount();
    model1->insertRow(row);

    model1->setData(model1->index(row, 0), Name_1);
    model1->setData(model1->index(row, 1), Name_2);
    if(winner == 1){
        model1->setData(model1->index(row, 2), Name_1);
    } else if(winner == 2){
        model1->setData(model1->index(row, 2), Name_2);
    } else if(winner == 3){
        model1->setData(model1->index(row, 2), draw);
    }
    // Применяем изменения
    model1->submitAll();

    // Обновляем модель
    model1->select();
}

void GameWindow::import_data_in_db(const QString nick){
    // Создаем новую строку
    int row = model->rowCount();
    model->insertRow(row);

    // Задаем значения для столбцов
    model->setData(model->index(row, 0), nick);  // Замените "Lesha" на вашу переменную QString
    model->setData(model->index(row, 1), 0);       // Замените 14 на вашу переменную int

    // Применяем изменения
    model->submitAll();

    // Обновляем модель
    model->select();

    qDebug() << "(((LLLLLL";
}

int GameWindow::get_rate_from_db(const QString nickToCheck){
    // Выполняем SQL-запрос для получения рейтинга по указанному нику
    QSqlQuery ratingQuery;
    int playerRating = 0;
    ratingQuery.prepare("SELECT Rate FROM Players WHERE Nick = :nick");
    ratingQuery.bindValue(":nick", nickToCheck);

    if (ratingQuery.exec() && ratingQuery.next()) {
        // Извлекаем рейтинг из результата запроса
        playerRating = ratingQuery.value(0).toInt();

        // Используем playerRating по вашему усмотрению
        qDebug() << "Ник: " << nickToCheck << ", Рейтинг: " << playerRating;

        // Пример: вставляем рейтинг в переменную типа int (замените "playerRatingVariable" на вашу переменную)
    } else {
        // Запись не найдена, делаем что-то еще (например, выводим сообщение об ошибке)
        qDebug() << "Запись с ником" << nickToCheck << "не найдена.";
        import_data_in_db(nickToCheck);
    }
    return playerRating;
}


void GameWindow::setPlayerNamesAndRatings(const QString& name1, const QString& name2) {
    Name_1 = name1;
    Name_2 = name2;


    // Загрузка данных из базы данных для игрока 1
    player1Rating = get_rate_from_db(name1);
    // Загрузка данных из базы данных для игрока 2
    player2Rating = get_rate_from_db(name2);

    // Обновление интерфейса с новыми данными
    player1NameLabel->setText("Игрок1:\n" + Name_1 + " " + QString::number(player1Rating));
    player2NameLabel->setText("Игрок2:\n" + Name_2 + " " + QString::number(player2Rating));
}



Position *GameWindow::getClickedPosition(QPushButton* clickedButton) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (clickedButton == &*buttons[row][col]) {
                // Найдена кнопка, вернем соответствующую ей позицию
                return new Position(row, col);
            }
        }
    }
    // Вернем позицию (-1, -1), чтобы обозначить ошибку, если кнопка не найдена
    return new Position(1, 1);
}

// Position * GameWindow::handleButtonClick() {
//     QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

//     Position * clickedPosition = getClickedPosition(clickedButton);
//     return clickedPosition;
// }


GameWindow::~GameWindow()
{
    delete query;
    delete history;
    delete model;
    delete model1;
    delete eatenPiecesLabel1;
    delete eatenPiecesLabel2;
    delete player1TimerLabel;
    delete player2TimerLabel;
    delete player1Timer;
    delete player2Timer;
    for(int i = 0; i < 8; i++){
        delete[] buttons[i];
    }

    delete[] buttons;

    delete gamechess;
    char change;
    delete player1Label;
    delete player2Label;
    delete player1Layout;
    delete player2Layout;
    delete eatenPiecesPlayer1;
    delete eatenPiecesPlayer2;
    delete ui;

}

// void GameWindow::set_name_1(const QString name)
// {
//     Name_1 = name;
//     player1NameLabel->setText("Игрок1:\n" + Name_1);
// }

// void GameWindow::set_name_2(const QString name)
// {
//     Name_2 = name;
//     player2NameLabel->setText("Игрок2:\n" + Name_2);
// }

void GameWindow::print(const Matrix<ChessPiece*> board, Color currentPlayer, Position king){

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                if(board[i][j]->GetColor() == currentPlayer)
                {
                    buttons[i][j]->setEnabled(true);
                } else {
                    buttons[i][j]->setDisabled(true);
                }
                QPixmap pixmap(board[i][j]->get_file());

                QIcon icon(pixmap);

                buttons[i][j]->setIcon(icon);
                int iconSize = static_cast<int>(80 * 0.8);
                buttons[i][j]->setIconSize(QSize(iconSize, iconSize));
            } else {
                buttons[i][j]->setIcon(QIcon());
                buttons[i][j]->setDisabled(true);
            }
            if ((i + j) % 2 == 0) {
                buttons[i][j]->setStyleSheet("background-color: #D3D3D3");
            } else {
                buttons[i][j]->setStyleSheet("background-color: #2F4F4F");
            }
        }
    }
    if(currentPlayer == Color::WHITE){
        if (game_status == gameStatus::Check1){
            buttons[king.getRow()][king.getCol()]->setStyleSheet("background-color: blue");
        } else if(game_status == gameStatus::Game){
            statusLabel->setText("Ход игрока " + Name_1);
        } else {
            end_game();
        }
    } else{

       if (game_status == gameStatus::Check2){
            buttons[king.getRow()][king.getCol()]->setStyleSheet("background-color: blue");
        } else if(game_status == gameStatus::Game){
           statusLabel->setText("Ход игрока " + Name_2);
        } else {
            end_game();
        }
    }
}
static bool is_pos(const std::vector<Position>& val_m, int row, int col) {
    return std::any_of(val_m.begin(), val_m.end(), [&](const Position& pos) {
        return pos.getRow() == row && pos.getCol() == col;
    });
}
void GameWindow::print1(const Board& board, Position* pos){
    //qDebug() << "Loh";
    const std::vector<Position>& val_m = board.GetValidMoves(*pos);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (is_pos(val_m, i, j)) {
                    buttons[i][j]->setEnabled(true);
                buttons[i][j]->setStyleSheet("background-color: red");
            } else if(pos->getRow() == i && pos->getCol() == j){
                buttons[i][j]->setEnabled(true);
                buttons[i][j]->setStyleSheet("background-color: yellow");
            }else {
                buttons[i][j]->setDisabled(true);
            }
        }
    }
}

void GameWindow::end_game(){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            buttons[i][j]->setDisabled(true);
            if ((i + j) % 2 == 0) {
                buttons[i][j]->setStyleSheet("background-color: #D3D3D3");
            } else {
                buttons[i][j]->setStyleSheet("background-color: #2F4F4F");
            }
        }
    }
    timers_stop();
}
int norm(int rate, bool win1){
    if(win1 == true){
        return rate + win;
    }
    if((rate + lose) >= 0){
        return rate + lose;
    }
    return 0;
}

void GameWindow::change_rate(const QString nickToCheck,  bool win1){
    // Выполняем SQL-запрос для изменения рейтинга по указанному нику
    QSqlQuery updateQuery;
    int rate = norm(get_rate_from_db(nickToCheck), win1);
    updateQuery.prepare("UPDATE Players SET Rate = :rate WHERE Nick = :nick");
    updateQuery.bindValue(":nick", nickToCheck);
    updateQuery.bindValue(":rate", rate);


    if (updateQuery.exec()) {
        // Изменения применены успешно
        model->select();  // Обновляем модель, чтобы отобразить изменения
        qDebug() << "Рейтинг для ника " << nickToCheck << " изменен на 22.";
    } else {
        // Что-то пошло не так при выполнении запроса
        qDebug() << "Ошибка при изменении рейтинга для ника " << nickToCheck;
    }
}

void GameWindow::check_status(Game* game){
    if(game->get_b().IsKingInCheck(Color::WHITE))
    {

        if(!game->get_b().is_mate(Color::WHITE)){
           game_status = gameStatus::Check1;
            statusLabel->setText("Шах на игрока " + Name_1);
        } else {
            game_status = gameStatus::Mat1;
            statusLabel->setText("Мат игрока " + Name_1);
            change_rate(Name_1, false);
            change_rate(Name_2, true);
            import_history(2);
        }
    } else if (game->get_b().IsKingInCheck(Color::BLACK)) {
        if(!game->get_b().is_mate(Color::BLACK)){
            game_status = gameStatus::Check2;
            statusLabel->setText("Шах на игрока " + Name_2);
        } else {
            game_status = gameStatus::Mat2;
            statusLabel->setText("Мат игрока " + Name_2);
            change_rate(Name_2, false);
            change_rate(Name_1, true);
            import_history(1);
        }
    } else {
             qDebug() << "@lllllllllllllllllllllllllll";
        if(game->get_b().is_mate(Color::WHITE)){
                      qDebug() << "@lllllllllllllllllllllllllll";
            game_status = gameStatus::Pat1;
            statusLabel->setText("Пат игрока " + Name_1);
            import_history(3);

        } else if(game->get_b().is_mate(Color::BLACK)){
                 qDebug() << "@lllllllllllllllllllllllllll";
            game_status = gameStatus::Pat2;
            statusLabel->setText("Пат игрока " + Name_2);
            import_history(3);

        } else {
                 qDebug() << "+++++++++";
            game_status = gameStatus::Game;
        }
    }
         qDebug() << "@kkkkkkkkkkkkkkkkll";
}

void GameWindow::Game_loop() {
        print(gamechess->get_board(), gamechess->get_current_color(), gamechess->get_b().Current_king_pos(gamechess->get_current_color()));
        //Position* from = handleButtonClick();
        //buttons[from->getRow()][from->getCol()].setStyleSheet("background-color: blue");
}
