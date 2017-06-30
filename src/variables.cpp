#include "variables.h"

Variables::Variables()
{

}

void Variables::set(const QString &key, const QString &value)
{
    int index = find(key);
    if(index < 0)
    {
        m_pairs.push_back(QPair<QString,QString>(key,value));
    }
    else
    {
        m_pairs[index].second = value;
    }
}

QString Variables::get(const QString &key) const
{
    int index = find(key);
    if(index == -1)
    {
        return key;
    }
    return get(index);
}

QString Variables::get(int index) const
{
    if(index >= 0)
    {
        return m_pairs[index].second;
    }
    TDEBUG() << "Не найдено элемент под индексом" << index;
    return QString();
}

int Variables::find(const QString &key) const
{
    for(int i=0;i<m_pairs.size();i++)
    {
        if(m_pairs[i].first == key)
        {
            return i;
        }
    }
    return -1;
}

