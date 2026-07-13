#include "abstracterrorprocessing.h"

AbstractErrorProcessing::AbstractErrorProcessing(QObject *parent)
    : QObject{parent}
{
   }

bool AbstractErrorProcessing::isLoadingData() const
{
    return m_isLoadingData;
}

bool AbstractErrorProcessing::isLoadedData() const
{
    return m_isLoadedData;
}

void AbstractErrorProcessing::setIsLoadedData(bool newIsLoadedData)
{
    if (m_isLoadedData == newIsLoadedData)
        return;
    m_isLoadedData = newIsLoadedData;
    emit isLoadedDataChanged();
}

void AbstractErrorProcessing::setIsLoadingData(bool newIsLoadingData)
{
    if (m_isLoadingData == newIsLoadingData)
        return;
    m_isLoadingData = newIsLoadingData;
    emit isLoadingDataChanged();
}
