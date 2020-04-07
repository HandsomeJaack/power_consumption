#ifndef PLOT_H
#define PLOT_H

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
    Q_PROPERTY(int lineCount READ lineCount NOTIFY lineCountChanged)
public:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    explicit PlotView(QQuickItem *parent = NULL);
    ~PlotView();
    void setNodeParameters(QSGGeometryNode *node, int pointCount,
                           float lineWidth, const QString &color);
    void plotGraph(QSGGeometryNode *node);
    void plotBackground(QSGGeometryNode *node, int pointCount);
    QList<QString> diff();
    int lineCount();

signals:
    void diffChanged();
    void lineCountChanged();

private:
    QList<QString> m_time;
    QList<int> m_percantage;
    QSGGeometryNode *m_background;
    int m_lineCount;
};

#endif // PLOT_H
