#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QPushButton>

#include "klamodel.h"
#include "phasetrajectorychartview.h"

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
