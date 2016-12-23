#include "cshighlighter.h"

#include <QRegularExpressionMatchIterator>

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"
#include "Misc/chronicler.h"
using Chronicler::shared;


CSHighlighter::CSHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    m_variableFormat.setFontItalic(true);
    m_variableFormat.setForeground(Qt::darkCyan);

    m_actionFormat.setFontWeight(QFont::Bold);
    m_actionFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\*(if|elseif)[^\n]*(and|or)|\\*\\w+|%\\+|%-|\\+|-|=|\\*|/|\\<|\\>|");
    rule.format = m_actionFormat;
    m_highlightingRules.append(rule);

    m_numberFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegularExpression("\\d+");
    rule.format = m_numberFormat;
    m_highlightingRules.append(rule);

    m_commentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\\*comment[^\n]*");
    rule.format = m_commentFormat;
    m_highlightingRules.append(rule);

    m_quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = m_quotationFormat;
    m_highlightingRules.append(rule);
}


void CSHighlighter::highlightBlock(const QString &text)
{
    // everything but variables
    foreach (const HighlightingRule &rule, m_highlightingRules)
    {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // variables
    QRegularExpression action("(\\*\\w+)");
    QRegularExpression dispVar("\\${([[:alpha:]_]\\w*)}");
    QRegularExpression white_space("\\s|\\n");

    if(action.match(text).hasMatch())
    {
        for(const CVariable v : shared().variablesView->model()->variables())
        {
            int start = 0;
            int end = text.indexOf(white_space);
            int diff = end;
            QString token = text.left(end);

            while(end != -1)
            {
                if(!action.match(token).hasMatch() && token.contains(v.name()))
                    setFormat(start, diff, m_variableFormat);

                start = end + 1;
                end = text.indexOf(white_space, start);
                diff = end - start;
                token = text.mid(start, diff);
            }
        }
    }
    else
    {
        QRegularExpressionMatchIterator i = dispVar.globalMatch(text);
        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), m_variableFormat);
        }
    }
}
