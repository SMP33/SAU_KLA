#pragma once

#include <QObject>
#include <QThread>
#include <QVector>

#include "modelblocks.hpp"

struct ModelParams {
    double alpha;
    double beta;
    double gamma;
    double lambda;
    double phi0;
    double dphi0;
    double M_DMS;
    double M_DGS;
    double M_V;
    double T;
    double tau;
    double k;
    double J;
    double dt;
    double tmax;
};

class KLAModel : public QThread {
    Q_OBJECT
public:
    KLAModel();
    bool setParams(const ModelParams& params);

signals:
    void simulationFinished(QVector<double> integratorPhi, QVector<double> integratorOmega);

private:
    void run() override;

    ModelParams m_params;
    double t = 0;

    QVector<SOBlock*> m_blocks;
    QVector<double> m_phiTrace;
    QVector<double> m_dphiTrace;

    IntegratorBlock* integratorPhi;
    IntegratorBlock* integratorOmega;
};
