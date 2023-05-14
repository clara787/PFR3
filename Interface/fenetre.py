from PyQt5.QtCore import Qt
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QPushButton, QApplication, QMainWindow
from qtmodern import styles
from PyQt5.QtWidgets import QLabel
from PyQt5.QtGui import QPixmap, QImage, QPainter, QPen, QColor
import sys
import cv2
import requests
import urllib.request
from bs4 import BeautifulSoup
import threading
import time
import numpy as np


"""---------------------------------Var globales-----------------------------------"""
point = []
url = "http://192.168.4.1"
url_robot = "http://192.168.4.1/robot"
url_cam = "http://192.168.4.1/image"
url_auto = "http://192.168.4.1/A"
robot = [(0,0)]
"""---------------------------------Fenêtre principale du logiciel--------------------------------------"""
class MyThread(threading.Thread):
    def __init__(self, my_class_instance):
        super().__init__()
        self.stop_flag = threading.Event()
        self.my_class_instance = my_class_instance
        self.etat = False

    def run(self):
        while not self.stop_flag.is_set():
            self.etat = True
            affichage_pt(self.my_class_instance)
            time.sleep(1)

    def stop(self):
        self.stop_flag.set()
        self.etat = False


class ManuelWindow(QMainWindow) :
    def __init__(self):
        super(ManuelWindow,self).__init__()
        
        self.manuel_button = QPushButton("Manuel",self)
        self.manuel_button.setGeometry(990, 10, 200, 60)
        self.manuel_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30; border :5px solid ;} QPushButton:pressed {background-color: lightblue;}")
        self.manuel_button.setEnabled(False)
        self.manuel_button.clicked.connect(self.goToManuelScreen)
        
        self.auto_button = QPushButton("Automatique",self)
        self.auto_button.setGeometry(990, 80, 200, 60)
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.auto_button.clicked.connect(self.goToAutoScreen)
        
        self.map = QLabel(self)
        self.map.setAlignment(Qt.AlignCenter)
        self.map.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        self.map.setGeometry(270, 10, 700, 500)
        
        #Video display widget
        self.video_display = QLabel(self)
        self.video_display.setAlignment(Qt.AlignCenter)
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        self.video_display.setGeometry(50, 550, 200, 200)
        
        """BOUTONS DIRECTIONS"""
        self.droite = QPushButton("D", self)
        self.droite.setGeometry(1080, 600, 80, 80)
        self.droite.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.droite.setEnabled(False)
        self.droite.clicked.connect(lambda : deplacement(2))
        
        self.gauche = QPushButton("G", self)
        self.gauche.setGeometry(950, 600, 80, 80)
        self.gauche.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.gauche.setEnabled(False)
        self.gauche.clicked.connect(lambda : deplacement(1))

        self.haut = QPushButton("H", self)
        self.haut.setGeometry(1015, 535, 80, 80)
        self.haut.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.haut.setEnabled(False)
        self.haut.clicked.connect(lambda : deplacement(3))

        self.bas = QPushButton("B", self)
        self.bas.setGeometry(1015, 665, 80, 80)
        self.bas.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.bas.setEnabled(False)
        self.bas.clicked.connect(lambda : deplacement(4))
        
        # Start button
        self.start_button = QPushButton('Commencer', self)
        self.start_button.setGeometry(400, 600, 200, 60)
        self.start_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.start_button.clicked.connect(self.point_thread)
        self.start_button.clicked.connect(self.show_camera)
        

        #Stop button
        self.stop_button = QPushButton('Arrêter', self)
        self.stop_button.setGeometry(650, 600, 200, 60)
        self.stop_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.stop_button.clicked.connect(self.stop_btn)
        self.stop_button.setEnabled(False)
        
        #thread point
        self.t1 = MyThread(self)
        
        self.cam = True
        #video capture
        #thread cam
        #self.t2 = threading.Thread(target=self.show_camera, args=())
        

    def goToAutoScreen(self):
        global point
        point = []
        self.map.clear()
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30; border :5px solid } QPushButton:pressed {background-color: lightblue;}")
        self.manuel_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.droite.hide()
        self.gauche.hide()
        self.haut.hide()
        self.bas.hide()
        self.manuel_button.setEnabled(True)
        self.auto_button.setEnabled(False)
        self.start_button.setEnabled(True)
        if (self.t1.etat == True) : self.t1.stop()
    
    def goToManuelScreen(self):
        global point 
        point = []
        self.map.clear()
        self.manuel_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30; border :5px solid } QPushButton:pressed {background-color: lightblue;}")
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.droite.show()
        self.gauche.show()
        self.haut.show()
        self.bas.show()
        self.auto_button.setEnabled(True)
        self.manuel_button.setEnabled(False)
        self.start_button.setEnabled(True)
        if (self.t1.etat == True) : self.t1.stop()
    
        
    def point_thread(self):
        self.t1 = MyThread(self)
        self.map.clear()
        self.stop_button.setEnabled(True)
        self.start_button.setEnabled(False)
        self.t1.start()
        if(not self.gauche.isHidden()):
            self.droite.setEnabled(True)
            self.gauche.setEnabled(True)
            self.haut.setEnabled(True)
            self.bas.setEnabled(True)
        else:
            deplacement(5)
        
    def show_camera(self):
        self.cam = True
        self.stop_button.setEnabled(True)
        while(self.cam):
            self.cap = cv2.VideoCapture(url_cam)  #cam esp
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
        
        
        
    def stop_btn(self):
        self.cam = False
        self.droite.setEnabled(False)
        self.gauche.setEnabled(False)
        self.haut.setEnabled(False)
        self.bas.setEnabled(False)
        self.t1.stop()
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        cv2.destroyAllWindows()
        self.cap.release()
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
    
    def closeEvent(self, event):
        self.cam = False
        self.cap.release()
        self.t1.stop()
        event.accept()
        

            
def recup_coord(pt) : #retour liste couple de coord
    point_tmp = []
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
                X = float(X)
                Y = float(Y)
                point_tmp.append((int(X),int(Y)))
                X = ""
                Y = ""
        
        
    return point_tmp

def affichage_pt(myclass) :
    global point
    global robot
    """recupération des coordonnées du point"""
    requete = requests.get(url)
    page = requete.content
    soup = BeautifulSoup(page, features="html.parser")
    coord_str = soup.find("h1").text
    
    """recupération des coordonnées du robot"""
    requete_robot = requests.get(url_robot)
    page_robot = requete_robot.content
    soup_robot = BeautifulSoup(page_robot, features="html.parser")
    coord_str_robot = soup_robot.find("h1").text

    if (not coord_str == "") :
        pt_tmp = recup_coord(coord_str)
        if (not coord_str_robot == "") :
            robot = recup_coord(coord_str_robot)
        
        for pt in pt_tmp:
            if (pt is not None):
                point.append(pt)
        
        X0 = myclass.map.width()
        Y0 = myclass.map.height()
        
        # Créer une image QPixmap pour le QLabel
        pixmap = QPixmap(myclass.map.width(), myclass.map.height())
        pixmap.fill(Qt.white)
        
        # Dessiner des points sur l'image
        painter = QPainter(pixmap)
        painter.translate(X0, Y0)
        
        for pt in point:
            #if (pt is not None) :
            painter.setPen(QPen(QColor('blue'), 5))
            painter.drawPoint(-pt[0], -pt[1])
            
            painter.setPen(QPen(QColor('black'), 9))
            painter.drawPoint(-robot[0][0], -robot[0][1])
        
        painter.end()
        # Afficher l'image dans le QLabel
        myclass.map.setPixmap(pixmap)
    else:
        print("pas de coord")

def deplacement(_dir):
    if (_dir == 1):
        urlG = "http://192.168.4.1/G"
        requests.get(urlG)
    if (_dir == 2):
        urlD = "http://192.168.4.1/D"
        requests.get(urlD)
    if (_dir == 3):
        urlH = "http://192.168.4.1/H"
        requests.get(urlH)
    if (_dir == 4):
        urlB = "http://192.168.4.1/B"
        requests.get(urlB)
    if (_dir == 5):
        urlA = "http://192.168.4.1/A"
        requests.get(urlA)
        
if __name__ == "__main__": 
    app = QApplication(sys.argv)
    styles.dark(app)
    widget = QtWidgets.QStackedWidget()

    """exection de la fenêtre principale"""
    window = ManuelWindow()
    widget.addWidget(window)
    widget.resize(1200,800)
    widget.setWindowTitle("Robot")
    widget.show()
    
    
    app.exec_()
    app.exit()
    
#At the end, free the webcam and the video capture
#cap.release()
cv2.destroyAllWindows()