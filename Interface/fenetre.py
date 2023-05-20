from PyQt5.QtCore import Qt, QObject
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QPushButton, QApplication, QMainWindow, QWidget, QFileDialog
from qtmodern import styles
from PyQt5.QtWidgets import QLabel
from PyQt5.QtGui import QPixmap, QImage, QPainter, QPen, QColor
import sys
import cv2
import requests
from bs4 import BeautifulSoup
import threading
import time


"""---------------------------------Var globales-----------------------------------"""
url = "http://192.168.4.1"
urlG = "http://192.168.4.1/G"
urlD = "http://192.168.4.1/D"
urlH = "http://192.168.4.1/H"
urlB = "http://192.168.4.1/B"
urlA = "http://192.168.4.1/A"
url_robot = "http://192.168.4.1/robot"
url_cam = "http://192.168.4.1/image"
urlS = "http://192.168.4.1/S"
urlM = "http://192.168.4.1/M"

point = []
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
            #time.sleep(1)

    def stop(self):
        self.stop_flag.set()
        self.etat = False

"""class KeyPressFilter(QObject):
    def __init__(self, parent):
        super().__init__(parent)

    def eventFilter(self, obj, event):
        if event.type() == event.KeyPress:
            if event.key() == Qt.Key_Up:
                deplacement(3, self)
                return True
            elif event.key() == Qt.Key_Down:
                deplacement(4, self)
                return True
            elif event.key() == Qt.Key_Left:
                deplacement(1, self)
                return True
            elif event.key() == Qt.Key_Right:
                deplacement(2, self)
                return True
        if event.type() == event.KeyRelease:
            stop_move()
            return True
        return False"""


class ManuelWindow(QWidget) :
    def __init__(self):
        super(ManuelWindow,self).__init__()
        
        self.manuel_button = QPushButton("Manuel",self)
        self.manuel_button.setGeometry(990, 10, 200, 60)
        self.manuel_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.manuel_button.clicked.connect(self.goToManuelScreen)
        #self.manuel_button.installEventFilter(KeyPressFilter(self))
        
        self.auto_button = QPushButton("Automatique",self)
        self.auto_button.setGeometry(990, 80, 200, 60)
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.auto_button.clicked.connect(self.goToAutoScreen)

        self.charger = QPushButton("Charger",self)
        self.charger.setGeometry(25, 10, 200, 60)
        self.charger.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.charger.clicked.connect(self.charge_image)
        
        
        self.save = QPushButton("Enregistrer",self)
        self.save.setGeometry(25, 80, 200, 60)
        self.save.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.save.clicked.connect(self.save_image)
        
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
        self.droite = QPushButton("Droite", self)
        self.droite.setGeometry(655, 600, 80, 80)
        self.droite.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.droite.pressed.connect(lambda : deplacement(2, self))
        self.droite.released.connect(stop_move)
        
        self.gauche = QPushButton("Gauche", self)
        self.gauche.setGeometry(525, 600, 80, 80)
        self.gauche.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.gauche.pressed.connect(lambda : deplacement(1, self))
        self.gauche.released.connect(stop_move)

        self.haut = QPushButton("Avant", self)
        self.haut.setGeometry(590, 535, 80, 80)
        self.haut.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.haut.pressed.connect(lambda : deplacement(3, self))
        self.haut.released.connect(stop_move)

        self.bas = QPushButton("Arrière", self)
        self.bas.setGeometry(590, 665, 80, 80)
        self.bas.setStyleSheet("QPushButton {background-color: '#4B686C'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.bas.pressed.connect(lambda : deplacement(4, self))
        self.bas.released.connect(stop_move)
        
        # Start button
        """self.start_button = QPushButton('Commencer', self)
        self.start_button.setGeometry(400, 600, 200, 60)
        self.start_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.start_button.clicked.connect(lambda : test_affichage(self))"""
        #self.start_button.clicked.connect(self.show_camera)
        

        #Stop button
        self.stop_button = QPushButton('Arrêter', self)
        self.stop_button.setGeometry(770, 610, 200, 60)
        self.stop_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.stop_button.clicked.connect(self.stop_btn)
        
        # Créer une image QPixmap pour le QLabel
        self.pixmap = QPixmap(self.map.width(), self.map.height())
        self.pixmap.fill(Qt.white)

        #thread point
        self.t1 = MyThread(self)
        self.cap = cv2.VideoCapture(url_cam)
        self.cam = False
    
        
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
        if (self.t1.etat == True) : self.t1.stop()
        else : self.point_thread()
        if(self.cam == False) : self.show_camera()
    
    def goToManuelScreen(self):
        global point 
        point = []
        init_manuel()
        self.map.clear()
        self.manuel_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30; border :5px solid } QPushButton:pressed {background-color: lightblue;}")
        self.auto_button.setStyleSheet("QPushButton {background-color: '#00aeef'; color: '#FFFFFF'; font-weight: bold; border-radius: 30} QPushButton:pressed {background-color: lightblue;}")
        self.droite.show()
        self.gauche.show()
        self.haut.show()
        self.bas.show()
        if (self.t1.etat == True) : self.t1.stop()
        else : self.point_thread()
        if(self.cam == False) : self.show_camera()
        
    def point_thread(self):
        self.t1 = MyThread(self)
        self.map.clear()
        self.t1.start()
        if(self.gauche.isHidden()): #cas auto
            deplacement(5, self)
            
        
    def show_camera(self):
        while(self.cap.isOpened()):
            self.cap = cv2.VideoCapture(url_cam)  #cam esp
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
                self.cam = False
                cv2.destroyAllWindows()
                break
    

    def save_image(self):
        fileName = QFileDialog.getSaveFileName(self, 'Save File', '', '*.jpg')
        self.pixmap.save(fileName[0])
    
    def charge_image(self):
        # create a QPixmap object
        dialog = QFileDialog(self)
        dialog.setFileMode(QFileDialog.FileMode.ExistingFiles)
        if dialog.exec_():
            fileName = dialog.selectedFiles()
            qpixmap = QPixmap(fileName[0])
            # creat a QLabel for image
            self.map.setScaledContents(True)
            self.map.setPixmap(qpixmap)

    def stop_btn(self):
        self.t1.stop()
        init_manuel()
        
        self.cam = False
        self.cap.release()
        self.video_display.setText("CAMERA")
        self.video_display.setStyleSheet("background-color: '#FFFFFF';  color: '#00aeef';font-weight: bold;")
        
        cv2.destroyAllWindows()
        
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
            painter.setPen(QPen(QColor('blue'), 5))
            painter.drawPoint(-pt[0], -pt[1])
            
            painter.setPen(QPen(QColor('black'), 9))
            painter.drawPoint(-robot[0][0], -robot[0][1])
        
        painter.end()
        # Afficher l'image dans le QLabel
        myclass.map.setPixmap(pixmap)
        myclass.pixmap = pixmap
    else:
        print("pas de coord")

def deplacement(_dir, myclass):
    if (_dir == 1):
        requests.get(urlG)
        requests.get(urlG)
    if (_dir == 2):
        requests.get(urlD)
        requests.get(urlD)
    if (_dir == 3):
        requests.get(urlH)
        requests.get(urlH)
    if (_dir == 4):
        requests.get(urlB)
        requests.get(urlB)
    if (_dir == 5):
        requests.get(urlA)
        requests.get(urlA)
        
def stop_move() :
    requests.get(urlS)
    time.sleep(0.1)
    requests.get(urlS)
   
def init_manuel():
    requests.get(urlM)
    requests.get(urlM)
   
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