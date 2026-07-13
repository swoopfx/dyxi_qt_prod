#include "comparestrings.h"

CompareStrings::CompareStrings() {}

bool CompareStrings::defaultCompareWord(const QString &word, const QString &sentence)
{
    if (sentence.contains(word, Qt::CaseInsensitive)) {
        return true;
    }else{
        return false;
    }
}

bool CompareStrings::regexCompareWord(const QString &word, const QString &sentence)
{
    // \\b ensures we match the whole word only
    QRegularExpression rx("\\b" + word + "\\b", QRegularExpression::CaseInsensitiveOption);
    if (sentence.contains(rx)) {
        return true ;
    }else{
        return false;
    }

}
