#ifndef PLOT_H
#define PLOT_H

#include <QColor>
#include <QQuickItem>
#include <QObject>
#include <QList>
#include <QString>
#include <QSGNode>
#include <QSGGeometryNode>

class PlotView: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> diff READ diff NOTIFY diffChanged)
    Q_PROPERTY(int lineCount READ lineCount WRITE setLineCount NOTIFY lineCountChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    explicit PlotView(QQuickItem *parent = NULL);
    ~PlotView();
    void setNodeParameters(QSGGeometryNode *node, int pointCount,
                           float lineWidth);
    void plotGraph(QSGGeometryNode *node);
    void plotBackground(QSGGeometryNode *node, int pointCount);
    QList<QString> diff();
    int lineCount();
    void setLineCount(int lines);

    QColor color();
    void setColor(QColor theme_color);

signals:
    void diffChanged();
    void lineCountChanged();
    void colorChanged();

private:
    QList<QString> m_time;
    QList<int> m_percantage;
    QSGGeometryNode *m_background;
    int m_lineCount;
    QColor m_color;
};

#endif // PLOT_H
