from PyQt5.QtCore import Qt, QTimer
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QPushButton
from PyQt5.QtWidgets import QApplication, QMainWindow, QDialog
from qtmodern import styles
from PyQt5.QtWidgets import QLabel
from PyQt5.QtGui import QPixmap, QImage, QPainter, QPen, QBrush, QColor
import qtmodern.windows
from PyQt5 import QtGui, QtCore
import sys
import cv2
import csv
import requests
from bs4 import BeautifulSoup
import time


"""---------------------------------Var globales-----------------------------------"""
point = []

"""---------------------------------Fenêtre principale du logiciel--------------------------------------"""


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.manuel_button = QPushButton("Manuel",self)
        self.manuel_button.setGeometry(300, 650, 200, 60)
        self.manuel_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.manuel_button.clicked.connect(self.goToManuelScreen)
        
        self.auto_button = QPushButton("Automatique",self)
        self.auto_button.setGeometry(600, 650, 200, 60)
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.auto_button.clicked.connect(self.goToAutoScreen)
        

    def goToManuelScreen(self):
        manuel = ManuelWindow()
        widget.addWidget(manuel)
        widget.setCurrentIndex(widget.currentIndex()+1)
    
    def goToAutoScreen(self):
        auto = AutoWindow()
        widget.addWidget(auto)
        widget.setCurrentIndex(widget.currentIndex()+1)
        
class ManuelWindow(QMainWindow) :
    def __init__(self):
        super(ManuelWindow,self).__init__()
        self.main_button = QPushButton("Retour",self)
        self.main_button.setGeometry(10, 10, 200, 60)
        self.main_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.main_button.clicked.connect(self.goToMainScreen)
        
        self.map = QLabel(self)
        self.map.setAlignment(Qt.AlignCenter)
        self.map.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        self.map.setGeometry(290, 10, 700, 500)
        
        """BOUTONS DIRECTIONS"""
        self.droite = QPushButton("D", self)
        self.droite.setGeometry(665, 600, 80, 80)
        self.droite.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")

        self.gauche = QPushButton("G", self)
        self.gauche.setGeometry(535, 600, 80, 80)
        self.gauche.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")

        self.haut = QPushButton("H", self)
        self.haut.setGeometry(600, 535, 80, 80)
        self.haut.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")

        self.bas = QPushButton("B", self)
        self.bas.setGeometry(600, 665, 80, 80)
        self.bas.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")

        
        # Start button
        self.start_button = QPushButton('Commencer', self)
        self.start_button.setGeometry(800, 535, 200, 60)
        self.start_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.start_button.clicked.connect(self.affichage_pt)
        #self.start_button.clicked.connect(self.show_camera)
        

        #Stop button
        self.stop_button = QPushButton('Arrêter', self)
        self.stop_button.setGeometry(800, 665, 200, 60)
        self.stop_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.stop_button.clicked.connect(self.close_video)

        #Video display widget
        self.video_display = QLabel(self)
        self.video_display.setAlignment(Qt.AlignCenter)
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        self.video_display.setGeometry(50, 550, 200, 200)
        
        #video capture
        self.cap = cv2.VideoCapture(0)  #cam esp

    def goToMainScreen(self):
        main = MainWindow()
        widget.addWidget(main)
        widget.setCurrentIndex(widget.currentIndex()+1)
    
        
    
    def show_camera(self):
        self.cap = cv2.VideoCapture("http://192.168.4.1/stream")  #cam esp
        while(self.cap.isOpened()):
            ret, image = self.cap.read()
            if ret == True:            
                # Convert the image to the RGB format that QImage expects
                rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                # Create a QImage from the RGB image
                qImg = QImage(rgb_image.data, rgb_image.shape[1], rgb_image.shape[0], QImage.Format_RGB888)
                pixmap = QPixmap.fromImage(qImg)
                if self.video_display is not None:
                    self.video_display.setPixmap(pixmap.scaled(self.video_display.size()))
                if  cv2.waitKey(1) == ord('q') :
                    break
            else:
                self.cap.release()
                cv2.destroyAllWindows()
                break
       
       
    def affichage_pt(self) :
        global point
        coord = ""
        url = "http://192.168.4.1"
        requete = requests.get(url)
        page = requete.content
        soup = BeautifulSoup(page, features="lxml")
        coord_str = soup.find("h1").text
        
        if (coord_str) :
            point.append(recup_coord(coord_str))
            
            print(point)
            X0 = self.map.width()
            Y0 = self.map.height()
    
            # Créer une image QPixmap pour le QLabel
            pixmap = QPixmap(self.map.width(), self.map.height())
            pixmap.fill(Qt.white)
            
            # Dessiner des points sur l'image
            painter = QPainter(pixmap)
            painter.translate(X0, Y0)
            for pt in point:
                if (point.index(pt) != len(point)-1) :
                    painter.setPen(QPen(QColor('blue'), 5))
                    painter.drawPoint(-pt[0], -pt[1])
                    
                    painter.setPen(QPen(QColor('black'), 7))
                    painter.drawPoint(0, 0)
                else:
                    break
            
            painter.end()
            # Afficher l'image dans le QLabel
            self.map.setPixmap(pixmap)
        else:
            print("pas de coord")
        
            
        
    def close_video(self):
        self.cap.release()
        cv2.destroyAllWindows()
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
    
    def closeEvent(self, event):
        self.cap.release()
        event.accept()
        
class AutoWindow(QMainWindow) :
    def __init__(self):
        super(AutoWindow,self).__init__()
        self.auto_button = QPushButton("Retour",self)
        self.auto_button.setGeometry(10, 10, 200, 60)
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.auto_button.clicked.connect(self.goToMainScreen)
        
        self.map = QLabel(self)
        self.map.setAlignment(Qt.AlignCenter)
        self.map.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        self.map.setGeometry(250, 10, 800, 600)

        #Video display widget
        self.video_display = QLabel(self)
        self.video_display.setAlignment(Qt.AlignCenter)
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        self.video_display.setGeometry(10, 550, 200, 200)
        
        # Start button
        self.start_button = QPushButton('Commencer', self)
        self.start_button.setGeometry(400, 665, 200, 60)
        self.start_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.start_button.clicked.connect(self.show_camera)
        

        #Stop button
        self.stop_button = QPushButton('Arrêter', self)
        self.stop_button.setGeometry(650, 665, 200, 60)
        self.stop_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.stop_button.clicked.connect(self.close_video)
        
        #video capture
        self.cap = cv2.VideoCapture(0)  #cam esp

    def goToMainScreen(self):
        main = MainWindow()
        widget.addWidget(main)
        widget.setCurrentIndex(widget.currentIndex()+1)
    
    def show_camera(self):
        self.cap = cv2.VideoCapture(0)  #cam esp
        while(self.cap.isOpened()):
            ret, image = self.cap.read()
            if ret == True:            
                # Convert the image to the RGB format that QImage expects
                rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                # Create a QImage from the RGB image
                qImg = QImage(rgb_image.data, rgb_image.shape[1], rgb_image.shape[0], QImage.Format_RGB888)
                pixmap = QPixmap.fromImage(qImg)
                if self.video_display is not None:
                    self.video_display.setPixmap(pixmap.scaled(self.video_display.size()))
                if  cv2.waitKey(1) == ord('q') :
                    break
            else:
                self.cap.release()
                cv2.destroyAllWindows()
                break
    
    def close_video(self):
        self.cap.release()
        cv2.destroyAllWindows()
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
    
    def closeEvent(self, event):
        self.cap.release()
        event.accept()

            
def recup_coord(pt) : #retour liste couple de coord
    global point   
    X = ""
    Y = ""
    
    for i in range (0,len(pt)) :
        if pt[i] == "X" :
            i+=1
            while pt[i] != "Y":
                X += pt[i]
                i+=1
            i+=1
            while pt[i] != "F" :
                Y += pt[i]
                i+=1
            if pt[i] == "F" :
                point.append((int(X),int(Y)))
                X = ""
                Y = ""
        
        
    #return point
        
if __name__ == "__main__": 
    app = QApplication(sys.argv)
    styles.dark(app)
    widget = QtWidgets.QStackedWidget()

    """exection de la fenêtre principale"""
    window = MainWindow()
    widget.addWidget(window)
    widget.setFixedSize(1200,800)
    widget.show()

    app.exec_()
    app.exit()
    
#At the end, free the webcam and the video capture
#cap.release()
cv2.destroyAllWindows()