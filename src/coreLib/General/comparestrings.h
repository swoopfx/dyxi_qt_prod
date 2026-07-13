#ifndef COMPARESTRINGS_H
#define COMPARESTRINGS_H

#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

/**
 * @brief The CompareStrings class
 * @details used to compare two strings such tat to confirm if one is contained in another
 */
class CompareStrings
{
public:
    CompareStrings();

    bool defaultCompareWord(const QString &, const QString &);
    bool regexCompareWord(const QString &, const QString &);
};

#endif // COMPARESTRINGS_H
