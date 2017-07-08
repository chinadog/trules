/* ---------------------------------------------------------------------------
 * Copyright © 2017 The trifecta Company Ltd.
 * Contact: http://www.trifecta.ru/licensing/
 * Author: BM
 *
 * This file is part of the Trifecta.
 *
 * Trifecta is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Trifecta is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------------
 */

#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <QString>
#include <QVector>
#include <TLogger/TLogger>
#include "variables.h"
#include <TDB/structs.h>



/**
 * @brief Класс Rules
 * @details Предназначен для считывания правил из файла и хранения их
 * структуры, для последующей обработки ими входных данных. Формат файла rule.
 */
class Rules
{
public:
    // Конструкторы
    Rules();
    Rules(const QString& filePath);
    int exec(const Event &event, QString* debug = 0);
    int exec(const QString& msg, long long date);
    void setRules(const QString& rules);
private:
    //Поля
    struct Action
    {
        QString cmd;
        QVector<QString> tokens;
    };
    struct Record
    {
        QVector<Action> actions;
    };
    QVector<Record> m_records;

    // Commands -------------------------------------------------------------------------------------
    bool concat(const QString& str1, const QString& str2, const QString& varName, Variables* vars);
    bool eventlog(const QString& logMsg);
    bool find(const QString& whereFind, const QString& whatFind, Variables* vars );
    bool regexp(const QString& whereFind, const QString& regexpStr, int dep, int count,
                const QString& varName, Variables* vars);
    bool set(const QString& varName, const QString& value, Variables* vars);

    // Приватные методы -----------------------------------------------------------------------------
    Record parseToToken(const QString& line);
    QString getValue(const QString& str, Variables* vars);
    QString removeQuotes(const QString& str);

    // Операторы вывода -----------------------------------------------------------------------------
    friend TDebug& operator<<( TDebug& logger, Record record)
    {
        for(int i=0;i<record.actions.size();i++)
        {
            Action action = record.actions[i];
            logger << "Action " << i << " ------------\n";
            logger << "  CMD: " << action.cmd << "\n";
            logger << "  [";
            for(int j=0;j<action.tokens.size();j++)
            {
                logger << action.tokens[j];
                if(j!=action.tokens.size()-1)
                {
                    logger << ',';
                }
            }
            logger << "]";
            if(i!=record.actions.size()-1)
            {
                logger << "\n";
            }
        }
        return logger;
    }
    friend TDebug& operator<<( TDebug& logger, Rules rules)
    {
        for(int i=0;i<rules.m_records.size();i++)
        {
            logger.nospace() << "\nRECORD " << i << " =======================\n"
                             << rules.m_records[i];
        }
        return logger;
    }
    friend Record& operator<<( Record& obj1, const Record &obj2)
    {
        for(int i=0;i<obj2.actions.size();i++)
        {
            obj1.actions << obj2.actions[i];
        }
        return obj1;
    }
};




#endif // RULEPARSER_H
