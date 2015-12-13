#ifndef SCHEME_H
#define SCHEME_H
#include "agent.h"
#include <vector>
#include <glm/glm.hpp>
#include <logger.h>
#include <map>
#include <rapidjson/document.h>

typedef std::string SchemeType;
typedef char Letter;
typedef std::vector<std::vector<Letter>> LetterMatrix;

struct Scheme
{
    enum TRANSFORM {
        TRANSFORM_MX,
        TRANSFORM_MY,
        TRANSFORM_CW,
        TRANSFORM_CCW,
        TRANSFORM_T,
        TRANSFORM_AFTER_LAST
    };

    LetterMatrix data;
    std::map<Letter, std::string> dict;

    /*!
     * \brief Отражение схемы по горизонтали
     */
    void MirrorX();

    /*!
     * \brief Отражение схемы по вертикали
     */
    void MirrorY();

    /*!
     * \brief Поворот схемы по чесовой стрелке
     */
    void RotateCCW();
    void RotateCW();

    /*!
     * \brief Транспонирование схемы
     */
    void Transpose();

    void Resize(glm::vec2 __size);
    void LogData() const;
    glm::vec2 size;
    SchemeType type;

    void Set(std::vector<std::vector<Letter>> __data);

    /*!
     * \brief Производит изменение № = остаток от деления __num на число возможных трансформаций
     * \param __num
     *
     * \code for(int i = 0; i < 5; ++i) NumericTransform(rnd()); //4 случайных трансформации
     * \code NumericTransform(Scheme::TRANSFORM_CCW);
     */
    void NumericTransform(int __num);

    /*!
    *\brief 123
    *
    *   Example:
    *   {
    *       "type":"scheme",
    *       "building":<string building type list name>,
    *       "size":[<int x, int y>],
    *       "dictionary":[
    *           [<char>,<id>],
    *           [<T>,<table>]
    *       ],
    *       "data":[
    *           <string scheme row>,
    *           ".................",
    *           "..TTTTTTTT.......",
    *           "..T......TTTTT...",
    *           "..T..........T...",
    *           "..T..........T...",
    *           "..TTTTTTTTTTTT...",
    *           "................."
    *       ]
    *   }
    */
    bool Deserialize(rapidjson::Value &__val);
};

#endif // SCHEME_H
