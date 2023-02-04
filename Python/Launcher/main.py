import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QFileDialog, QLineEdit, QLabel, QSlider
from PyQt5.QtCore import Qt
from functools import partial
import ImageConversation


class MyApp(QWidget):
    def __init__(self):
        super().__init__()

        # Image block vars
        self.PATH_BLOCK_Y = 0.05
        self.PATH_BLOCK_BASE_DISTANCE = 10
        self.pathBlockSize = None
        self.lePath = None
        self.LE_PATH_BASE_WIDTH = 300
        self.btnChooseImage = None
        self.BTN_CHOOSE_IMAGE_BASE_WIDTH = 100
        self.PATH_BLOCK_WIDTH = self.LE_PATH_BASE_WIDTH +\
                                self.BTN_CHOOSE_IMAGE_BASE_WIDTH +\
                                self.PATH_BLOCK_BASE_DISTANCE
        # Binary block
        self.leOffset = None
        self.sOffset = None
        self.lOffset = None

        # Window vars
        self.minWindowSize = [self.PATH_BLOCK_WIDTH + 20, 160]

        # Buttons
        self.btnOriginal = None
        self.btnNegative = None
        self.btnGray = None
        self.btnBinary = None
        self.btnUpScale = None
        self.btnDownScale = None
        self.btnReset = None

        self.initUI()

    def initUI(self):
        self.setWindowTitle('Image redactor')
        self.setGeometry(int(self.screen().size().width() / 2 - self.minWindowSize[0] / 2),
                         int((self.screen().size().height() / 2 - self.minWindowSize[1] / 2) * 0.8),
                         self.minWindowSize[0],
                         self.minWindowSize[1])
        self.setFixedSize(self.minWindowSize[0], self.minWindowSize[1])

        # Image block
        self.btnChooseImage = QPushButton("Browse image", self)
        self.lePath = QLineEdit(self)
        self.lePath.setGeometry(int(self.minWindowSize[0] / 2 - self.PATH_BLOCK_WIDTH / 2),
                                int(self.minWindowSize[1] * self.PATH_BLOCK_Y),
                                self.LE_PATH_BASE_WIDTH,
                                25)
        self.btnChooseImage.clicked.connect(partial(self.onClicked, 0))

        # -----------------
        # Conversion block
        # -----------------

        # Original
        self.btnOriginal = QPushButton("Show original", self)
        self.btnOriginal.clicked.connect(partial(self.onClicked, 1))
        self.btnOriginal.move(170, 50)

        # Negative
        self.btnNegative = QPushButton("Convert to negative", self)
        self.btnNegative.clicked.connect(partial(self.onClicked, 2))
        self.btnNegative.move(20, 100)

        # Gray
        self.btnGray = QPushButton("Convert to gray", self)
        self.btnGray.clicked.connect(partial(self.onClicked, 3))
        self.btnGray.move(130, 100)

        # -----------------
        # Binary block
        # -----------------
        self.btnBinary = QPushButton("Convert to binary", self)
        self.btnBinary.clicked.connect(partial(self.onClicked, 4))
        self.btnBinary.move(220, 100)

        self.btnReset = QPushButton("Reset", self)
        self.btnReset.clicked.connect(self.reset)
        self.btnReset.move(int(self.btnBinary.x() + 8),
                           self.btnBinary.y() + self.btnBinary.height() - 5)

        self.sOffset = QSlider(Qt.Horizontal, self)
        self.sOffset.setRange(-128, 128)
        self.sOffset.valueChanged.connect(lambda value: self.leOffset.setText(str(value)))
        self.sOffset.move(self.btnBinary.x() + self.btnBinary.width() + 5, self.btnBinary.y())

        self.leOffset = QLineEdit('0', self)
        self.leOffset.returnPressed.connect(lambda: self.sOffset.setValue(self.tryConvert()))
        self.leOffset.resize(30, 20)
        self.leOffset.move(int(self.sOffset.x() + self.sOffset.width() / 2 - 5), self.sOffset.y() + 22)

        self.lOffset = QLabel("Offset", self)
        self.lOffset.move(self.leOffset.x() - 35, self.leOffset.y() + 3)
        # -----------------
        # -----------------

    def onClicked(self, index):
        imagePath = self.lePath.text()
        offset = self.tryConvert()

        btnFinder = {
            0: self.onBrowseClick,
            1: ImageConversation.showOriginal,
            2: ImageConversation.toNegative,
            3: ImageConversation.toGray,
            4: ImageConversation.toBinary,
            5: self.reset
        }

        if index == 0 or index == 5:
            func = btnFinder.get(index)
            func()
        elif imagePath:
            if index == 4:
                func = btnFinder.get(index)
                func(imagePath, offset)
            else:
                func = btnFinder.get(index)
                func(imagePath)
        else:
            pass

    def onBrowseClick(self):
        options = QFileDialog.Options()
        imagePath, _ = QFileDialog.getOpenFileName(self, "Browse image", "",
                                                   "Image files (*.png *.jpeg *.jpg)",
                                                   options=options)
        if imagePath:
            self.lePath.setText(f"{imagePath}")

    def tryConvert(self):
        try:
            value = int(self.leOffset.text())
            if value < -128 or value > 128:
                value = 0
                self.leOffset.setText('0')
                self.sOffset.setValue(0)
            return value
        except:
            self.leOffset.setText('0')
            self.sOffset.setValue(0)
            return 0

    def reset(self):
        self.leOffset.setText('0')
        self.sOffset.setValue(0)

    def resizeEvent(self, event):
        new_width = event.size().width()
        new_height = event.size().height()

        if new_width < self.minWindowSize[0]:
            self.resize(self.minWindowSize[0], new_height)
            new_width = self.minWindowSize[0]
        if new_height < self.minWindowSize[1]:
            self.resize(new_width, self.minWindowSize[1])
            new_height = self.minWindowSize[1]

        new_pathBlockX = int(new_width / 2 - self.PATH_BLOCK_WIDTH / 2)
        new_pathBlockY = int(new_height * self.PATH_BLOCK_Y)
        self.lePath.move(new_pathBlockX, new_pathBlockY)
        self.btnChooseImage.move(int(new_pathBlockX + self.lePath.width() + self.PATH_BLOCK_BASE_DISTANCE),
                                 new_pathBlockY)

        QWidget.resizeEvent(self, event)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MyApp()
    window.show()
    sys.exit(app.exec_())
