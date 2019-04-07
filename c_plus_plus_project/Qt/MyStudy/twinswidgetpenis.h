#ifndef TWINSWIDGETPENIS_H
#define TWINSWIDGETPENIS_H

#include "twinswidgetmother.h"

class TwinsWidgetPenis : public QWidget {
    Q_OBJECT
    private:
    QPushButton mPenisBtn;
    TwinsWidgetMother * mMother;

    public:
    explicit TwinsWidgetPenis(QWidget * parent = nullptr);
    void setMother(TwinsWidgetMother * mMother);
    void goToBossHome();

    signals:

    public slots:
    void handleBossSlots();
};

#endif // TWINSWIDGETPENIS_H
