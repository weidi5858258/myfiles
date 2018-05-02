# -*- coding:utf-8 -*-

import sys

# reload(sys)
# sys.setdefaultencoding('utf-8')

from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QApplication, QWidget


class Icon(QWidget):

    def __init__(self, parent=None):
        QWidget.__init__(self)
        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle("My first window app")
        self.setWindowIcon(QIcon("ic_sync_green_holo.png"))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    icon = Icon()
    icon.show()
    sys.exit(app.exec_())
