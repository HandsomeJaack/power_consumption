#include "cpuview.h"
#include "cpustat/cpustat.h"
#include <QMapIterator>
#include <QSGSimpleRectNode>
#include <QGraphicsItem>
#include <QSGSimpleMaterial>
#include <GLES2/gl2ext.h>

CpuView::CpuView(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(ItemHasContents);
    Usage u;
    u.getStats();
    m_appList = u.totalProgramUsage();
    m_graph = nullptr;
    m_color = Qt::white;
    m_limit = 5;
}

CpuView::~CpuView()
{
}

QColor CpuView::color()
{
    return m_color;
}

void CpuView::setColor(QColor theme_color)
{
    m_color = theme_color;
}

QStringList CpuView::apps()
{
    QStringList apps;
    for(int i = 0; i < m_appList.size(); ++i) {
        apps.append(m_appList[i].second); // + "\n" + QString::number(m_appList[i].first, 'f', 2) + "%");
    }
    return apps;
}

QStringList CpuView::percentage()
{
    QStringList percentage;
    for(int i = 0; i < m_appList.size(); ++i) {
        percentage.append(QString::number(m_appList[i].first, 'f', 2) + "%");
    }
    return percentage;
}

void CpuView::setNodeParameters(QSGGeometryNode *node, int pointCount,
                                 float lineWidth)
{
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), pointCount);
    geometry->setLineWidth(lineWidth);
    geometry->setDrawingMode(GL_LINES);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
    material->setColor(m_color);

    node->setGeometry(geometry);
    node->setMaterial(material);
    node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

void CpuView::plotGraph(QSGGeometryNode *node)
{
    float y = 0.0;
    float  x = 0.0;
    float diff = 5.0;

    int pointStep = 0;
    for(int listItemIndex = 0; listItemIndex < m_appList.size(); listItemIndex++) {
        node->geometry()->vertexDataAsPoint2D()[pointStep].set(0, y);

        x = m_appList[listItemIndex].first;
        if(m_appList[listItemIndex].first < 1.0)
            x = 1;

        node->geometry()->vertexDataAsPoint2D()[pointStep+1].set(static_cast<float>(x/100*width()), y);
        qDebug() << QString::number(x/100*width(), 'f', 2) + ": " + QString::number(y);
        y += diff;
    }
}

QSGNode* CpuView::updatePaintNode(QSGNode *oldNode,
                                   UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData);

    if(!oldNode) {
        m_graph = new QSGGeometryNode();
        setNodeParameters(m_graph, 2*m_appList.size(), 100);
        plotGraph(m_graph);
    }
    return m_graph;
}
