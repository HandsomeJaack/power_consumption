#ifndef CPUVIEW_H
#define CPUVIEW_H

#include <QColor>
#include <QQuickItem>
#include <QObject>
#include <QStringList>
#include <QString>
#include <QPair>
#include <QSGGeometryNode>

class CpuView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QStringList apps READ apps NOTIFY appsChanged)
    Q_PROPERTY(QStringList percentage READ percentage NOTIFY percentageChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
public:
    explicit CpuView(QQuickItem *parent = nullptr);
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    ~CpuView();
    void setNodeParameters(QSGGeometryNode *node, int pointCount,
                           float lineWidth);
    void plotGraph(QSGGeometryNode *node);

    QStringList apps();
    QStringList percentage();
    QColor color();
    void setColor(QColor theme_color);
    float lineWidth();
    void setLineWidth(float width);
signals:
    void appsChanged();
    void percentageChanged();
    void colorChanged();
    void lineWidthChanged();
private:
    QVector<QPair<double, QString>> m_appList;
    QColor m_color;
    QSGGeometryNode *m_graph;
    int m_limit;
    float m_lineWidth;
};

#endif // CPUVIEW_H
