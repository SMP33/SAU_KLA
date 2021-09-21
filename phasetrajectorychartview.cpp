#include "phasetrajectorychartview.h"

#include <QDebug>
#include <QDesktopWidget>

PhaseTrajectoryChartView::PhaseTrajectoryChartView(QWidget* parent)
    : QChartView(new QChart, parent)
{
    setRenderHint(QPainter::Antialiasing);
    QDesktopWidget* w = new QDesktopWidget;
    QRect geometry = w->screenGeometry();

    geometry.setSize(geometry.size() / 2);

    setGeometry(geometry);
    move(geometry.center());
}

void PhaseTrajectoryChartView::plotTrajectory(QVector<double> m_phi, QVector<double> m_dphi)
{

    QLineSeries* series = new QLineSeries;

    for (auto i = 0; i < m_phi.size(); i++) {
        series->append(m_phi[i], m_dphi[i]);
    }

    chart()->addSeries(series);

    QValueAxis* axisX = new QValueAxis;
    axisX->setLabelFormat("%.3f");
    axisX->setTickType(QValueAxis::TicksDynamic);
    axisX->setTickInterval(0.4);
    chart()->setAxisX(axisX, series);
    chart()->legend()->hide();

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%.3f");
    axisY->setTickType(QValueAxis::TicksDynamic);
    axisY->setTickInterval(0.1);
    chart()->setAxisY(axisY, series);
}

void PhaseTrajectoryChartView::clear()
{
    chart()->removeAllSeries();
    for (auto* a : chart()->axes()) {
        chart()->removeAxis(a);
    }
}

void PhaseTrajectoryChartView::closeEvent(QCloseEvent* event)
{
    event->ignore();
    hide();
}
