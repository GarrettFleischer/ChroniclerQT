#ifndef CSHIGHLIGHTER_H
#define CSHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class CSHighlighter : public QSyntaxHighlighter
{
public:
    CSHighlighter(QTextDocument *parent = Q_NULLPTR);

protected:
    virtual void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;

    QTextCharFormat m_actionFormat;
    QTextCharFormat m_variableFormat;
    QTextCharFormat m_numberFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_quotationFormat;
};

#endif // CSHIGHLIGHTER_H
