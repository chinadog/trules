#ifndef VARIABLES_H
#define VARIABLES_H

#include <QString>
#include <QPair>
#include <QVector>
#include <TLogger/TLogger>

class Variables
{
public:
    Variables();
    void set(const QString& key, const QString& value);
    QString get(const QString& key) const;
    QString get(int index) const;
    int find(const QString& key) const;
    inline int size() const {return m_pairs.size();}
private:
    QVector<QPair<QString,QString> > m_pairs;
    friend TDebug& operator<<( TDebug& logger, Variables vars)
    {
        logger.nospace() << '(' << '\n' ;
        for(int i=0;i<vars.size();i++)
        {
            logger << vars.m_pairs[i].first << " = " << vars.m_pairs[i].second << '\n';
        }
        logger << ')';
        return logger;
    }
};

#endif // VARIABLES_H
