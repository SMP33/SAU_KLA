#include "klamodel.h"

#include <QDebug>
#include <QMetaType>

auto qvector_double_id = qRegisterMetaType<QVector<double>>("QVector<double>");

KLAModel::KLAModel()
{
}

void KLAModel::run()
{
    int n = int(m_params[tmax] / m_params[dt] + 1);
    m_phiTrace.reserve(n);
    m_dphiTrace.reserve(n);

    double t = 0;
    while (t <= m_params[tmax]) {
        m_dphiTrace.push_back(integratorOmega->out());
        m_phiTrace.push_back(integratorPhi->out());

        for (auto* block : m_blocks) {
            block->tick(m_params[dt]);
            t += m_params[dt];
        }
    }

    m_dphiTrace.push_back(integratorOmega->out());
    m_phiTrace.push_back(integratorPhi->out());

    emit simulationFinished(m_phiTrace, m_dphiTrace);
}

bool KLAModel::setParams(QMap<Params, double> params)
{
    m_params = params;

    if (isRunning())
        return false;
    for (auto* block : m_blocks) {
        delete block;
    }
    m_blocks.clear();

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
    summatorM->addSignal(delay, 1 / m_params[J]);

    summatorM->addSignal(&m_params[M_V], 1 / params[J]);

    integratorOmega->setValue(m_params[dphi0]);
    integratorPhi->setValue(m_params[phi0]);
    sensorOmega->setK(m_params[k]);
    sensorOmega->setSpan(m_params[gamma]);
    sensorAngle->setK(0.7);
    relay->setActiveOut(m_params[M_DMS]);
    relay->setBackCoeff(m_params[lambda]);
    relay->setValueActive(m_params[alpha]);
    delay->setDelay(m_params[tau], m_params[dt]);

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
