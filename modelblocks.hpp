#pragma once

#include <queue>
#include <vector>

//! Интерфейс блока с одним выходом
class SOBlock {
public:
    virtual ~SOBlock() = default;
    virtual void tick(double dt)
    {
    }

    //! Возвращает указатель на сигнал выхода
    double* signalOut()
    {
        return &m_out;
    }

    //! Возвращает значение выходного сигнала
    double out() const
    {
        return m_out;
    }

protected:
    //! Устанавливает значение выходного сигнала
    void setOut(double out)
    {
        m_out = out;
    }

    //! Значение выходного сигнала
    double m_out = 0;
};

//! Интерфейс блока с одним входом и одним выходом
class SISOBlock : public SOBlock {
public:
    //! Устанавливает входной сигнал
    void setSignalIn(SOBlock* prevBlock)
    {
        m_in = prevBlock->signalOut();
    }

    //! Возвращает входное значение
    double in()
    {
        if (m_in) {
            return *m_in;
        }

        return 0;
    }

private:
    //! Указатель на входное значение
    double* m_in = nullptr;
};

class SumBlock : public SOBlock {
public:
    //! Добавляет входной сигнал и коффициент перед ним
    void addSignal(double* signal, double coeff = 1)
    {
        m_inputs.push_back(signal);
        m_coeffs.push_back(coeff);
    }
    //! Добавляет входной сигнал и коффициент перед ним
    void addSignal(SOBlock* prevBlock, double coeff = 1)
    {
        m_inputs.push_back(prevBlock->signalOut());
        m_coeffs.push_back(coeff);
    }
    virtual void tick(double dt) override
    {
        double s = 0;
        for (auto i = 0; i < m_inputs.size(); i++) {
            s += (*m_inputs[i]) * m_coeffs[i];
        }

        setOut(s);
    }

private:
    std::vector<double*> m_inputs;
    std::vector<double> m_coeffs;
};

class IntegratorBlock : public SISOBlock {

public:
    virtual void tick(double dt) override
    {
        m_out += in() * dt;
    }

    //! Устанавливает значение на выходе интегратора
    void setValue(double value)
    {
        m_out = value;
    }
};

//! Модель ДУ
class PhiSensorBlock : public SISOBlock {
public:
    virtual void tick(double dt) override
    {
        double phi = in();

        // Saturation
        if (phi > phiMax) {
            phi = phiMax;
        } else if (phi < -phiMax) {
            phi = -phiMax;
        }

        setOut(k * phi);
    }

    //! Устанавливает диапазон линейности ДУ
    void PhiSensorBlock::setPhiMax(double value)
    {
        phiMax = value;
    }

    //! Устанавливает коэффициент передачи ДУ
    void PhiSensorBlock::setK(double value)
    {
        k = value;
    }

private:
    //! Коэффициент передачи ДУ
    double k = 1;
    //! Диапазон линейности ДУ
    double phiMax = 1;
};

//! Модель ДУСа
class OmegaSensorBlock : public SISOBlock {
public:
    virtual void tick(double dt) override
    {
        double omega = in();

        // Зона нечувствительности
        if (abs(omega) <= span) {
            setOut(0);
            return;
        }

        //Saturation
        if (omega > omegaMax) {
            omega = omegaMax;
        } else if (omega < -omegaMax) {
            omega = -omegaMax;
        }

        setOut(k * (omega + (omega > 0 ? -1 : 1) * span));
    }

    //! Устанавливает крутизну характеристики
    void setK(double value)
    {
        k = value;
    }

    //! Устанавливает диапазон линейности
    void setOmegaMax(double value)
    {
        omegaMax = value;
    }

    //! Устанавливает ширину зону нечувствительности
    void setSpan(double value)
    {
        span = value / 2;
    }

private:
    //! Коэффициент передачи ДУСа
    double k = 1;
    //! Диапазон линейности ДУСа
    double omegaMax = 0.5;
    //! Зона нечувствительности
    double span = 0;
};

//! Трехпозиционное реле
class RelayBlock : public SISOBlock {

public:
    virtual void tick(double dt) override
    {
        double i = in();

        // dI/dt>0
        if (i - iLast > 0) {
            if (i > valueActive) {
                setOut(+activeOut);
            } else if (i < -backCoeff * valueActive) {
                setOut(-activeOut);
            } else {
                setOut(0);
            }
        } /*dI/dt<0*/ else {
            if (i < -valueActive) {
                setOut(-activeOut);
            } else if (i > backCoeff * valueActive) {
                setOut(activeOut);
            } else {
                setOut(0);
            }
        }

        iLast = i;
    }

    //! Устанавливает коэффициент возврата
    void setBackCoeff(double value)
    {
        backCoeff = value;
    }
    //! Устанавливает значение срабатывания
    void setValueActive(double value)
    {
        valueActive = value;
    }
    //! Устанавливает значение на выходе при срабатывании
    void setActiveOut(double value)
    {
        activeOut = value;
    }

private:
    //! Коэффициент возврата
    double backCoeff = 0.5;
    //! Значение срабатывания
    double valueActive = 1;
    //! Значение на выходе при срабатывании
    double activeOut = 1;

    double iLast = 0;
};

class DelayBlock : public SISOBlock {

public:
    virtual void tick(double dt) override
    {
        trace.push(in());
        setOut(trace.front());
        trace.pop();
    }

    //! Устанавливает задержку
    void setDelay(double delay, double dt)
    {
        int n = int(delay / dt) + 1;
        while (!trace.empty()) {
            trace.pop();
        }

        for (int i = 0; i < n; i++) {
            trace.push(0);
        }
    }

private:
    std::queue<double> trace;
};
