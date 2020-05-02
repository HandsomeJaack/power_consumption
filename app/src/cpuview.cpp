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
        apps.append(m_appList[i].second);
    }
    return apps;
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
    float diff = 0;

    for(int i = 0; i < 2*m_appList.size() - 1; ++i) {
        node->geometry()->vertexDataAsPoint2D()[0].set(0, diff);
        node->geometry()->vertexDataAsPoint2D()[1].set(static_cast<float>(m_appList[0].first*100), diff);
        diff += height()/m_appList.size();
    }
}

QSGNode* CpuView::updatePaintNode(QSGNode *oldNode,
                                   UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData);

    if(!oldNode) {
        m_graph = new QSGGeometryNode();
        setNodeParameters(m_graph, m_appList.size(), 100);
        plotGraph(m_graph);
    }
    return m_graph;
}
