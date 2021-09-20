#include "mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QHBoxLayout>

#include <QLabel>
#include <QSvgWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    QFile f(":/res/labels.txt");
    f.open(QIODevice::Text | QIODevice::ReadOnly);
    QString txt = f.readAll();
    QStringList labels = txt.split('\n');

    QGridLayout* l = new QGridLayout;
    for (int i = 0; i < 13; i++) {
        QStringList v = labels[i].split(';');

        addParamLine(
            QString(":/res/L%1.svg").arg(i + 1),
            v[0].replace("\\n", "\n  "),
            v[1].toDouble(),
            v[2].toDouble(),
            v[3].toInt(),
            l);
    }

    QGridLayout* r = new QGridLayout;
    for (int i = 13; i < 13 + 8; i++) {
        QStringList v = labels[i].split(';');

        addParamLine(
            "",
            v[0].replace("\\n", "\n  "),
            v[1].toDouble(),
            v[2].toDouble(),
            v[3].toInt(),
            r);
    }

    m_startBtn = new QPushButton("График");
    m_startBtn->setFixedHeight(m_startBtn->sizeHint().height() * 3);
    QFont font = m_startBtn->font();
    font.setPointSize(font.pointSize() * 3);
    m_startBtn->setFont(font);
    r->addWidget(m_startBtn, r->rowCount(), 0, -1, -1);

    QWidget* wl = new QWidget;
    QWidget* wr = new QWidget;
    wl->setLayout(l);
    wr->setLayout(r);
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(wl);
    h->addWidget(wr);

    QWidget* w = new QWidget;
    w->setLayout(h);

    setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addParamLine(
    QString svgPath,
    QString text,
    double defaultValue,
    double valueStep,
    bool positiveOnly,
    QGridLayout* l)
{
    int row = l->rowCount();

    QDoubleSpinBox* spinBox = new QDoubleSpinBox;
    double vMax = std::numeric_limits<float>::max();
    double vMin = positiveOnly ? 0 : -vMax;
    spinBox->setRange(vMin, vMax);
    spinBox->setValue(defaultValue);
    spinBox->setSingleStep(valueStep);
    spinBox->setMaximumWidth(100);

    if (!svgPath.isEmpty()) {
        QSvgWidget* svg = new QSvgWidget;
        svg->load(svgPath);
        svg->setFixedSize(svg->sizeHint());
        l->addWidget(svg, row, 0);
        l->addWidget(new QLabel(text), row, 1);
        l->addWidget(spinBox, row, 2);
    } else {
        l->addWidget(new QLabel(text), row, 0);
        l->addWidget(spinBox, row, 1);
    }
}
