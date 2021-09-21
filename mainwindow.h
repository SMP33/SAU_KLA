#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QPushButton>

#include "klamodel.h"
#include "phasetrajectorychartview.h"

enum Params {
    alpha,
    beta,
    gamma,
    lambda,
    phi0,
    dphi0,
    M_DMS,
    M_DGS,
    M_V,
    T,
    tau,
    k,
    J,
    marginLeft,
    marginRight,
    marginTop,
    marginBottom,
    phiStep,
    dphiStep,
    dt,
    tmax
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartButtonClicked();

private:
    void addParamLine(
        QString svgPath,
        int id,
        QString text,
        double defaultValue,
        double valueStep,
        bool positiveOnly,
        int row,
        QGridLayout* l);
    QPushButton* m_startBtn;
    QMap<Params, QDoubleSpinBox*> m_param;

    PhaseTrajectoryChartView* m_chart;
};
#endif // MAINWINDOW_H
