#include "klamodel.h"

#include <QDebug>
#include <QMetaType>

auto qvector_double_id = qRegisterMetaType<QVector<double>>("QVector<double>");

KLAModel::KLAModel()
{
}

void KLAModel::run()
{
    int n = int(m_params.tmax / m_params.dt + 1);
    m_phiTrace.reserve(n);
    m_dphiTrace.reserve(n);

    while (t <= m_params.tmax) {
        m_dphiTrace.push_back(integratorOmega->out());
        m_phiTrace.push_back(integratorPhi->out());

        for (auto* block : m_blocks) {
            block->tick(m_params.dt);
            t += m_params.dt;
        }
    }

    m_phiTrace.reserve(n);
    m_dphiTrace.reserve(n);

    emit simulationFinished(m_phiTrace, m_dphiTrace);
}

bool KLAModel::setParams(const ModelParams& params)
{
    if (isRunning())
        return false;
    for (auto* block : m_blocks) {
        delete block;
    }
    m_blocks.clear();

    m_params = params;

    integratorOmega = new IntegratorBlock;
    integratorPhi = new IntegratorBlock;
    PhiSensorBlock* sensorAngle = new PhiSensorBlock;
    OmegaSensorBlock* sensorOmega = new OmegaSensorBlock;
    SumBlock* summatorU = new SumBlock;
    RelayBlock* relay = new RelayBlock;
    DelayBlock* delay = new DelayBlock;
    SumBlock* summatorM = new SumBlock;

    integratorOmega->setSignalIn(summatorM);
    integratorPhi->setSignalIn(integratorOmega);
    sensorOmega->setSignalIn(integratorOmega);
    sensorAngle->setSignalIn(integratorPhi);
    summatorU->addSignal(sensorAngle, -1);
    summatorU->addSignal(sensorOmega, -1);
    relay->setSignalIn(summatorU);
    delay->setSignalIn(relay);
    summatorM->addSignal(delay, 1 / params.J);
    summatorM->addSignal(&m_params.M_V, 1 / params.J);

    integratorOmega->setValue(params.dphi0);
    integratorPhi->setValue(params.phi0);
    sensorOmega->setK(params.k);
    sensorOmega->setSpan(params.gamma);
    sensorAngle->setK(0.7);
    relay->setActiveOut(params.M_DMS);
    relay->setBackCoeff(params.lambda);
    relay->setValueActive(params.alpha);
    delay->setDelay(params.tau, params.dt);

    m_blocks.push_back(integratorOmega);
    m_blocks.push_back(integratorPhi);
    m_blocks.push_back(sensorOmega);
    m_blocks.push_back(sensorAngle);
    m_blocks.push_back(summatorU);
    m_blocks.push_back(relay);
    m_blocks.push_back(delay);
    m_blocks.push_back(summatorM);

    return true;
}
