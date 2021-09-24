#pragma once

#include <QMap>
#include <QObject>
#include <QThread>
#include <QVector>

#include "modelblocks.hpp"

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

class KLAModel : public QThread {
    Q_OBJECT
public:
    KLAModel();
    bool setParams(QMap<Params, double> params);

signals:
    void simulationFinished(QVector<double> integratorPhi, QVector<double> integratorOmega);

private:
    void run() override;

    QMap<Params, double> m_params;

    QVector<SOBlock*> m_blocks;
    QVector<double> m_phiTrace;
    QVector<double> m_dphiTrace;

    IntegratorBlock* integratorPhi;
    IntegratorBlock* integratorOmega;
};
