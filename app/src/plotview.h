#ifndef PLOT_H
#define PLOT_H

#include <QColor>
#include <QQuickItem>
#include <QObject>
#include <QList>
#include <QStringList>
#include <QSGNode>
#include <QSGGeometryNode>

class PlotView: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QStringList measurementTime READ measurementTime NOTIFY measurementChanged)
    Q_PROPERTY(int lineCount READ lineCount WRITE setLineCount NOTIFY lineCountChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    explicit PlotView(QQuickItem *parent = NULL);
    ~PlotView();
    void setNodeParameters(QSGGeometryNode *node, int pointCount,
                           float lineWidth, bool isBlur);
    void plotGraph(QSGGeometryNode *node);
    void plotBackground(QSGGeometryNode *node, int pointCount);
    void plotBlur(QSGGeometryNode *node);
    QStringList measurementTime();
    int lineCount();
    void setLineCount(int lines);

    QColor color();
    void setColor(QColor theme_color);

signals:
    void measurementChanged();
    void lineCountChanged();
    void colorChanged();

private:
    QStringList m_time;
    QList<int> m_percantage;
    QSGGeometryNode *m_background;
    int m_lineCount;
    QColor m_color;
};

#endif // PLOT_H
