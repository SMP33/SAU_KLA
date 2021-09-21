#pragma once

#include <QObject>

#include <QCloseEvent>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "klamodel.h"

QT_CHARTS_USE_NAMESPACE

struct ChartViewParams {
    double x1;
    double x2;
    double y1;
    double y2;
    double dx;
    double dy;

    ModelParams model;
};

class PhaseTrajectoryChartView : public QChartView {
    Q_OBJECT
public:
    explicit PhaseTrajectoryChartView(QWidget* parent = nullptr);

    void setParams(const ChartViewParams& value);

public slots:
    void plotTrajectory(QVector<double> m_phi, QVector<double> m_dphi);
    void clear();

private:
    ChartViewParams m_params;

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
