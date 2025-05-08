#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <utility>
#include "C:/Users/User/Documents/untitled1/other/color.h"

/**
 * @brief Класс, представляющий игрока в шахматах
 */
class Player {
public:
    /**
     * @brief Конструктор с параметрами
     * @param color Цвет игрока
     * @param name_ Имя игрока
     * @param rate Рейтинг игрока
     */
    Player(Color color, std::string name_, int rate) : color(color), name(std::move(name_)), rating(rate) {}

    /**
     * @brief Деструктор по умолчанию
     */
    ~Player() = default;

    /**
     * @brief Устанавливает имя игрока
     * @param name_ Новое имя игрока
     */
    void set_name(const std::string &name_);

    /**
     * @brief Устанавливает рейтинг игрока
     * @param rating_ Новый рейтинг игрока
     */
    void set_rating(int rating_);

    /**
     * @brief Получает рейтинг игрока
     * @return Рейтинг игрока
     */
    [[nodiscard]] int get_rating() const;

    /**
     * @brief Получает имя игрока
     * @return Имя игрока
     */
    [[nodiscard]] std::string get_name() const;

    /**
     * @brief Получает цвет игрока
     * @return Цвет игрока
     */
    [[nodiscard]] Color get_color() const;

    /**
     * @brief Получает время игрока
     * @return Время игрока
     */
    [[nodiscard]] double get_time() const;

    /**
     * @brief Получает количество убитых фигур игрока
     * @return Количество убитых фигур игрока
     */
    [[nodiscard]] int get_kills_pieces() const;

private:
    int kills_piece; /**< Количество убитых фигур игрока */
    double time; /**< Время игрока */
    Color color; /**< Цвет игрока */
    std::string name; /**< Имя игрока */
    int rating; /**< Рейтинг игрока */
};
#endif // PLAYER_H
