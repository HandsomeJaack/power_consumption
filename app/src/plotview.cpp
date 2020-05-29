#include "batstat.h"
#include "plotview.h"
#include <QtMath>
#include <QDate>
#include <QSGSimpleRectNode>
#include <QGraphicsItem>
#include <QSGSimpleMaterial>
#include <GLES2/gl2ext.h>

PlotView::PlotView(QQuickItem *parent): QQuickItem(parent)
{
    setFlag(ItemHasContents);
    BatteryStatistics stats;
    m_time = stats.getTime();
    m_percantage = stats.getPercentage();
    m_background = nullptr;
    m_lineCount = 5;
    m_color = Qt::white;
}

PlotView::~PlotView()
{
}

QStringList PlotView::measurementTime()
{
    QStringList diff;
    int lastMeasurement = m_time.size()-1;

    QString firstMeasurementDay = m_time[0];
    QString midMeasurementDay = m_time[lastMeasurement/2];
    QString lastMeasurementDay = m_time[lastMeasurement];

    if(firstMeasurementDay.left(6) == lastMeasurementDay.left(6)) {
        firstMeasurementDay = firstMeasurementDay.remove(0,6);
        midMeasurementDay = midMeasurementDay.remove(0,6);
        lastMeasurementDay = lastMeasurementDay.remove(0,6);
    } else {
        firstMeasurementDay =  firstMeasurementDay.insert(7, '\n');
        midMeasurementDay = midMeasurementDay.insert(7, '\n');
        lastMeasurementDay = lastMeasurementDay.insert(7, '\n');
    }
    diff.append(firstMeasurementDay);
    diff.append(midMeasurementDay);
    diff.append(lastMeasurementDay);

    return diff;
}

int PlotView::lineCount()
{
    return m_lineCount;
}

void PlotView::setLineCount(int lines)
{
    m_lineCount = lines;
}

QColor PlotView::color()
{
    return m_color;
}

void PlotView::setColor(QColor theme_color)
{
    m_color = theme_color;
}

void PlotView::setNodeParameters(QSGGeometryNode *node, int pointCount,
                                 float lineWidth, bool isBlur)
{
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), pointCount);
    geometry->setLineWidth(lineWidth);
    geometry->setDrawingMode(GL_LINES);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
    if(isBlur) {
        int r,g,b;
        m_color.getRgb(&r,&g,&b);
        material->setColor(QColor(r,g,b, 64));
    } else {
        material->setColor(m_color);
    }

    node->setGeometry(geometry);
    node->setMaterial(material);
    node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

void PlotView::plotBackground(QSGGeometryNode *node, int lineCount)
{
    float step = 0;
    float diff = height()/lineCount;

    for(int i = 0; i < 2*lineCount - 1; i += 2){
        node->geometry()->vertexDataAsPoint2D()[i].set(0, step);
        node->geometry()->vertexDataAsPoint2D()[i+1].set(width(), step);
        step += diff;
    }
}

void PlotView::plotGraph(QSGGeometryNode *node)
{
    float step = 0;
    float diff = width()/(m_percantage.size() - 1);

    for(int i = 0;  i < m_percantage.size(); ++i) {
        node->geometry()->vertexDataAsPoint2D()[i].set(step, height() - height()*m_percantage[i]/100);
        step += diff;
        node->geometry()->vertexDataAsPoint2D()[i+1].set(step, height() - height()*m_percantage[i+1]/100);
    }
}

void PlotView::plotBlur(QSGGeometryNode *node)
{
    node->geometry()->setDrawingMode(GL_TRIANGLES);

    float step = 0;
    float diff = width()/(m_percantage.size() - 1);

    int shadowStep = 0;
    for(int i = 0;  i < m_percantage.size() - 1; ++i) {
        node->geometry()->vertexDataAsPoint2D()[shadowStep].set(step, height() - height()*m_percantage[i]/100);
        node->geometry()->vertexDataAsPoint2D()[shadowStep+1].set(step, height()*0.8);
        step += diff;
        node->geometry()->vertexDataAsPoint2D()[shadowStep+2].set(step, height() - height()*m_percantage[i+1]/100);

        node->geometry()->vertexDataAsPoint2D()[shadowStep+3].set(step, height() - height()*m_percantage[i+1]/100);
        node->geometry()->vertexDataAsPoint2D()[shadowStep+4].set(step, height()*0.8);
        node->geometry()->vertexDataAsPoint2D()[shadowStep+5].set(step - diff, height()*0.8);
        shadowStep += 6;
    }
}

QSGNode* PlotView::updatePaintNode(QSGNode *oldNode,
                                   UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData);

    if(!oldNode) {
        m_background = new QSGGeometryNode();
        setNodeParameters(m_background, m_lineCount*2, 1, false);
        m_background->setInheritedOpacity(0.3);
        plotBackground(m_background, m_lineCount);

        auto graph = new QSGGeometryNode();
        setNodeParameters(graph, m_percantage.size(), 3, false);
        graph->geometry()->setDrawingMode(GL_LINE_STRIP);
        plotGraph(graph);

        auto blur = new QSGGeometryNode();

        int pointCount = m_percantage.size()*6;
        setNodeParameters(blur, pointCount, 3, true);
        plotBlur(blur);

        m_background->appendChildNode(graph);
        m_background->appendChildNode(blur);
    }
    m_background->markDirty(QSGNode::DirtyGeometry);
    return m_background;
}
