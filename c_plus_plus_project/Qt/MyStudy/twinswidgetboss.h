#ifndef TWINSWIDGETBOSS_H
#define TWINSWIDGETBOSS_H

#include "twinswidgetmother.h"

class TwinsWidgetBoss : public QWidget {
    Q_OBJECT
    private:
    QPushButton mBossBtn;
    TwinsWidgetMother * mMother;

    public:
    explicit TwinsWidgetBoss(QWidget * parent = nullptr);
    void setMother(TwinsWidgetMother * mMother);
    void goToPenisHome();

    signals:

    public slots:
    void handlePenisSlots();
    void handlePenisSlots2(int, QString);
};

#endif // TWINSWIDGETBOSS_H
