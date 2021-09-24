#include "phasetrajectorychartview.h"

#include <QDebug>
#include <QDesktopWidget>

#include <algorithm>

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

    double x1, x2, y1, y2;
    QLineSeries* series = new QLineSeries;

    x1 = *std::min_element(m_phi.begin(), m_phi.end());
    x2 = *std::max_element(m_phi.begin(), m_phi.end());
    y1 = *std::min_element(m_dphi.begin(), m_dphi.end());
    y2 = *std::max_element(m_dphi.begin(), m_dphi.end());

    for (auto i = 0; i < m_phi.size(); i++) {
        series->append(m_phi[i], m_dphi[i]);
    }

    chart()->addSeries(series);

    QValueAxis* axisX = new QValueAxis;
    axisX->setLabelFormat("%.3f");
    axisX->setTickType(QValueAxis::TicksDynamic);
    axisX->setTickInterval(m_params[phiStep]);

    chart()->legend()->hide();

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%.3f");
    axisY->setTickType(QValueAxis::TicksDynamic);
    axisY->setTickInterval(m_params[dphiStep]);

    QLineSeries* r1 = new QLineSeries;

    int n = 1e2;
    for (int i = 0; i < n; i++) {
        double y = y1 + (y2 - y1) * i / n;

        double x
            = m_params[alpha] * 2
            - m_params[k] * (y - m_params[gamma])
            + y * m_params[tau];

        //        if (m_params[lambda] > 0) {
        //            x /= m_params[lambda];
        //        }

        //        if (x < m_params[alpha])
        {
            r1->append(x, y);
        }
    }

    chart()->addSeries(r1);

    QLineSeries* r3 = new QLineSeries;
    chart()->addSeries(r3);

    for (auto* s : chart()->series()) {
        chart()->setAxisX(axisX, s);
        chart()->setAxisY(axisY, s);
    }
}

void PhaseTrajectoryChartView::clear()
{
    chart()->removeAllSeries();
    for (auto* a : chart()->axes()) {
        chart()->removeAxis(a);
    }
}

void PhaseTrajectoryChartView::setParams(QMap<Params, double> params)
{
    m_params = params;
}

void PhaseTrajectoryChartView::closeEvent(QCloseEvent* event)
{
    event->ignore();
    hide();
}
