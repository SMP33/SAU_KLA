#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QPushButton>

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
    J
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void addParamLine(
        QString svgPath,
        QString text,
        double defaultValue,
        double valueStep,
        bool positiveOnly,
        QGridLayout* l);
    QPushButton* m_startBtn;
    QMap<Params, QDoubleSpinBox*> m_param;
};
#endif // MAINWINDOW_H
