#include <QDebug>
#include <QFile>
#include <TLogger/TLogger>
#include "ruleparser.h"
#include <QRegExp>

//------------------------------------------------------------------------------
//                               Конструкторы
//------------------------------------------------------------------------------

Rules::Rules()
{

}

Rules::Rules(const QString &filePath)
{
    QFile file( filePath );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        Record record;
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(line.size()>1 && line[0] != '/' && line[1] != '/')
            {
                if(line[0] != '\t' && record.actions.size()>0)
                {
                    m_records.push_back(record);
                    record.actions.clear();
                }

                record << parseToToken(line);
            }
        }
        m_records.push_back(record);
        record.actions.clear();
    }
    else
    {
       TWARNING() << "Не удалось открыть файл правил" << filePath;
    }
}

//--------------------------------------------------------------------------------------
//                                     Методы
//--------------------------------------------------------------------------------------

// -- Methods --------------------------------------------------------------------------

QString Rules::getValue(const QString &str, Variables* vars)
{
    QString ret;
    if(vars->find(str)!=-1)
    {
        ret = vars->get(str);
    }
    else
    {
        ret = removeQuotes( str );
    }
    return ret;
}

Rules::Record Rules::parseToToken(const QString &line)
{
    Record record;
    QString buf;
    bool quotes = false;
    for(int i = 0;i<line.size();i++)
    {
        // Если символ двоеточия и кавчки не открыты, то значит это команда
        if(line[i] == ':' && quotes == false)
        {
            Action action;
            action.cmd = buf;
            record.actions.push_back( action );
            buf.clear();
        }
        // Если разделительный символ, и кавычки не открыты, то
        else if((line[i] == ',' || line[i] == '|') && quotes == false)
        {
            int size = record.actions.size();
            record.actions[size-1].tokens.push_back( buf );
            buf.clear();
        }
        else if(line[i] == '"')
        {
            quotes = !quotes;
            buf+=line[i];
        }
        else if(line[i] != '\t')
        {
            buf+=line[i];
        }
    }
    return record;
}

QString Rules::removeQuotes(const QString &str)
{
    int size = str.size()-1;
    QString ret = str;
    if(str.size() == 0)
    {
        return str;
    }
    if(str[0]=='"' && str[size]=='"')
    {
        ret = ret.remove(size,1).remove(0,1);
    }
    return ret;
}

void Rules::setRules(const QString &rules)
{
    QStringList list = rules.split("\n");
    Record record;
    for (int i=0;i<list.size();i++)
    {
         QString line = list[i];
         if(line.size()>1 && line[0] != '/' && line[1] != '/')
         {
             if(line[0] != '\t' && record.actions.size()>0)
             {
                 m_records.push_back(record);
                 record.actions.clear();
             }
             record << parseToToken(line);
         }
         if(i==list.size()-1)
         {
             m_records.push_back(record);
         }
    }
}

int Rules::exec(const QString &msg, long long date)
{
    int result = -1;
    Variables vars;
    vars.set("msg",msg);
    vars.set("date",QString::number(date));

    for(int i=0;i<m_records.size();i++) // Пробегаем по всем правилам
    {
        int ruleResult = -1;
        for(int j=0;j<m_records[i].actions.size();j++) // Пробегаем по всем лексемам в правиле
        {
            Action action = m_records[i].actions[j];
            QString cmd = action.cmd;
//            int recordTokensSize = action.tokens.size();
            if(cmd == "find")
            {
                bool ret = find( action.tokens[0],
                                 action.tokens[1], &vars );
                if(ret == 0)
                {
                    ruleResult = -1;
                    break;
                }
                ruleResult = i;
            }
            else if(cmd == "regexp")
            {
                bool ret = regexp( action.tokens[0],
                                   action.tokens[1],
                                   action.tokens[2].toInt(),
                                   action.tokens[3].toInt(),
                                   action.tokens[4], &vars );
                if(ret == 0)
                {
                    ruleResult = -1;
                    break;
                }
                ruleResult = i;
            }
            else if(action.cmd == "set")
            {
                set( action.tokens[0],
                     action.tokens[1], &vars );
                ruleResult = i;
            }
            else if(action.cmd == "concat")
            {
                QString varName = action.tokens[0];
                QString str1 = getValue(action.tokens[1], &vars);
                for(int k=2;k<action.tokens.size();k++)
                {
                    QString str2 = getValue(action.tokens[k], &vars);
                    concat( varName, str1, str2, &vars);
                    str1 = str1+str2;
                }
            }
            else if(action.cmd == "eventlog")
            {
                TWARNING() << "eventlog: " << vars.get( action.tokens[0] );
                eventlog( action.tokens[0] );
                ruleResult = i;
//                j=j+1;
            }
        }
        if(ruleResult > 0)
        {
            result = ruleResult;
            break;
        }
    }
    return result;
}

// -- Commands -------------------------------------------------------------------------

bool Rules::concat(const QString &varName, const QString &str1, const QString &str2, Variables *vars)
{
    QString res = str1 + str2;
    set(varName, res, vars);
    return true;
}

bool Rules::eventlog(const QString &logMsg)
{
    TDEBUG() << logMsg;
    return true;
}

bool Rules::find(const QString &whereFind, const QString &whatFind, Variables* vars)
{
    QString whereFindValue = vars->get(whereFind);
    QString whatFindValue = whatFind;
    if(whatFind[0]=='"')
    {
        whatFindValue = whatFindValue.remove(whatFindValue.size()-1,1).remove(0,1);
    }
    else
    {
        whatFindValue = vars->get(whatFind);
    }
    return whereFindValue.contains(whatFindValue);
}

bool Rules::regexp(const QString &whereFind, const QString &regexpStr, int dep, int count,
                   const QString &varName, Variables *vars)
{
    TDEBUG() << "regexp start exec";
    TDEBUG() << " where = " << removeQuotes( vars->get(whereFind) );
    TDEBUG() << " regex = " << removeQuotes( regexpStr );
    QString whereFindValue = vars->get(whereFind);
    QString regexpStrValue = removeQuotes( regexpStr );
    QRegExp re( regexpStrValue );
    bool ret = false;
    int lastPos = 0;
    int i = 0;
    while( ( lastPos = re.indexIn( whereFindValue, lastPos ) ) != -1 )
    {
        lastPos += re.matchedLength();
        if(i == count)
        {
            TDEBUG() << "regexp: found - " << re.cap( dep );
            ret = true;
            set(varName,re.cap( dep ),vars);
        }
    }
    return ret;
}

bool Rules::set(const QString &varName, const QString &value, Variables* vars)
{
    vars->set(varName, value);
    return true;
}





