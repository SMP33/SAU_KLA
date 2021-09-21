#include "mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QHBoxLayout>

#include <QLabel>
#include <QSvgRenderer>
#include <QSvgWidget>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    QFile f(":/res/labels.md");
    f.open(QIODevice::Text | QIODevice::ReadOnly);
    QString txt = f.readAll();
    QStringList labels = txt.split('\n');
    labels.removeAt(0);
    labels.removeAt(0);

    QGridLayout* l = new QGridLayout;
    QGridLayout* r = new QGridLayout;

    for (int i = 0; i < labels.size(); i++) {

        bool isL = i < 13;

        if (labels[i].isEmpty()) {
            continue;
        }

        QStringList v = labels[i].split('|');
        QString svg = QString(":/res/L%1.svg").arg(i + 1);

        addParamLine(
            isL ? svg : "",
            v[0].toInt(),
            v[1].replace("\\n", "\n  "),
            v[2].toDouble(),
            v[3].toDouble(),
            v[4].toInt(),
            isL ? i : i - 13,
            l);
    }

    m_startBtn = new QPushButton("График");
    QFont font = m_startBtn->font();
    font.setPointSize(font.pointSize() * 3);
    m_startBtn->setFont(font);
    QSizePolicy sp = m_startBtn->sizePolicy();
    sp.setVerticalPolicy(QSizePolicy::Expanding);
    m_startBtn->setSizePolicy(sp);
    l->addWidget(m_startBtn, l->rowCount() - 4, 3, -1, -1);

    QWidget* wl = new QWidget;
    QWidget* wr = new QWidget;
    wl->setLayout(l);

    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(wl);
    h->addWidget(wr);

    QWidget* w = new QWidget;
    w->setLayout(h);

    setCentralWidget(w);

    connect(m_startBtn, &QPushButton::clicked,
        this, &MainWindow::onStartButtonClicked);

    m_chart = new PhaseTrajectoryChartView;
}

MainWindow::~MainWindow()
{
}

void MainWindow::onStartButtonClicked()
{

    ModelParams params;

#define ADD_PARAM(x) params.##x = m_param[##x]->value();

    ADD_PARAM(alpha)
    ADD_PARAM(beta)
    ADD_PARAM(gamma)
    ADD_PARAM(lambda)
    ADD_PARAM(phi0)
    ADD_PARAM(dphi0)
    ADD_PARAM(M_DMS)
    ADD_PARAM(M_DGS)
    ADD_PARAM(M_V)
    ADD_PARAM(T)
    ADD_PARAM(tau)
    ADD_PARAM(k)
    ADD_PARAM(J)
    ADD_PARAM(dt)
    ADD_PARAM(tmax)

    ChartViewParams p;
    p.x1 = m_param[marginLeft]->value();
    p.x2 = m_param[marginRight]->value();
    p.y1 = m_param[marginTop]->value();
    p.y2 = m_param[marginBottom]->value();
    p.dx = m_param[phiStep]->value();
    p.dy = m_param[dphiStep]->value();
    p.model = params;

    KLAModel* model = new KLAModel;
    model->setParams(params);

    connect(model, &KLAModel::simulationFinished,
        m_chart, &PhaseTrajectoryChartView::plotTrajectory);
    connect(model, &KLAModel::finished,
        model, &QObject::deleteLater);

    m_chart->clear();
    m_chart->show();
    m_chart->setParams(p);

    model->start();
    m_chart->activateWindow();
}

void MainWindow::addParamLine(
    QString svgPath,
    int id,
    QString text,
    double defaultValue,
    double valueStep,
    bool positiveOnly,
    int row,
    QGridLayout* l)
{

    QDoubleSpinBox* spinBox = new QDoubleSpinBox;
    double vMax = std::numeric_limits<float>::max();
    double vMin = positiveOnly ? 0 : -vMax;
    spinBox->setRange(vMin, vMax);
    spinBox->setValue(defaultValue);
    spinBox->setSingleStep(valueStep);
    spinBox->setMaximumWidth(100);

    int h = spinBox->sizeHint().height();

    if (!svgPath.isEmpty()) {
        QSvgRenderer* svg = new QSvgRenderer(svgPath);
        QImage img(svgPath);
        QLabel* imgLabel = new QLabel;
        imgLabel->setPixmap(QPixmap::fromImage(img));

        l->addWidget(imgLabel, row, 0);
        l->addWidget(new QLabel(text), row, 1);
        l->addWidget(spinBox, row, 2);
    } else {
        l->addWidget(new QLabel(text), row, 3);
        l->addWidget(spinBox, row, 4);
    }

    m_param.insert(static_cast<Params>(id), spinBox);
}
