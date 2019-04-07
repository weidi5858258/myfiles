#include "twinswidgetpenis.h"

TwinsWidgetPenis::TwinsWidgetPenis(QWidget * parent) : QWidget(parent) {
    resize(300, 300);
    setWindowTitle(QString("我是老二"));

    mPenisBtn.setText("去老大家");
    mPenisBtn.move(150, 150);
    mPenisBtn.setParent(this);

    connect( & mPenisBtn, & QPushButton::pressed,
             this, & TwinsWidgetPenis::goToBossHome);
}

void TwinsWidgetPenis::setMother(TwinsWidgetMother * mother) {
    mMother = mother;

    connect(mMother, & TwinsWidgetMother::callPenisSignals,
            this, & TwinsWidgetPenis::handleBossSlots);
}

void TwinsWidgetPenis::goToBossHome() {
    this->hide();

    emit mMother->callBossSignals();
    emit mMother->callBossSignals(100, QString("新年好!!!"));
}

void TwinsWidgetPenis::handleBossSlots() {
    this->show();
}
