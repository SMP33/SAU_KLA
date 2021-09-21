#pragma once

#include <QObject>

#include <QCloseEvent>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class PhaseTrajectoryChartView : public QChartView {
    Q_OBJECT
public:
    explicit PhaseTrajectoryChartView(QWidget* parent = nullptr);

public slots:
    void plotTrajectory(QVector<double> m_phi, QVector<double> m_dphi);
    void clear();

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
