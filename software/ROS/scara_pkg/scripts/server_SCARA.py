#!/usr/bin/env python

import rospy
import socket

from std_msgs.msg import String
from geometry_msgs.msg import Twist
from scara_pkg.msg import command

conn = None

msg_comando = command()
stato = 0
parametri = []

okMessage = "$OK"
errMessage = "$ERR"
endString = "!"

def ricevi_comandi():   
    global stato
    global msg_comando
    global conn 
    #Istanzio publisher(topic, tipo topic, buffer msg)
    rate = rospy.Rate(50) #50Hz
    msg_comando = command()

    while not rospy.is_shutdown():
        #pub.publish(msg_comando)
        print("_")
        try:
            richiesta = conn.recv(4096)
            data = str(richiesta, "utf-8")
            letturaComando(data)
             
        except socket.timeout:
            print("timeout")
        
        
        rate.sleep()


def letturaComando(data):
    global stato
    global msg_comando
    global parametri
    global conn
    risposta = ""

    if data.startswith("M") or data.startswith("C") or data.startswith("S"):
        if data.endswith(endString):
            data = data.replace(endString,"")
            if "|" in data:
                tipo = data.split("|")
                msg_comando.commandType = tipo[0]
                parametri = tipo[1].split(",")
                print(tipo[0])
                print(parametri)
                composer(data)
                pub.publish(msg_comando)
                risposta = okMessage
                msg_comando = command()
            else:
                msg_comando.commandType = data
                pub.publish(msg_comando)
                risposta = okMessage
                msg_comando = command()
                
        else:
            risposta = errMessage
            print("ERR: Il messaggio deve finire con !")
    else:
        risposta = errMessage
        print("ERR: Il messaggio deve finire con !")

    conn.sendall(risposta.encode())
   

def composer(data):
    global msg_comando
    global parametri

    if msg_comando.commandType.startswith("M"):
        if msg_comando.commandType == "M6":
            msg_comando.zAxisCommand = int(parametri[0])
        if msg_comando.commandType == "M7":
            msg_comando.gripper = int(parametri[0])
        if msg_comando.commandType != "M6" and msg_comando.commandType != "M7" and msg_comando.commandType != "M8":
            msg_comando.nextPoint.x = round(float(parametri[0]),2)
            msg_comando.nextPoint.y = round(float(parametri[1]),2)
            msg_comando.nextPoint.z = round(float(parametri[2]),2)
            msg_comando.speed = round(float(parametri[3]),2)
        #======== MESSAGGI CON C ==================
    if msg_comando.commandType.startswith("C"):
        if msg_comando.commandType == "C1":
            msg_comando.waitTime = int(parametri[0])
        if msg_comando.commandType == "C2":
            msg_comando.inputNumber = int(parametri[0])
    #======== MESSAGGI CON S ===================
    if msg_comando.commandType.startswith("S"):
        if msg_comando.commandType == "S1":
            msg_comando.speed = round(float(parametri[0]),2)
        if msg_comando.commandType == "S4":
            msg_comando.acceleration = round(float(parametri[0]),2)


def subServer(indirizzo, backlog = 1):
    global conn
    try:
        s = socket.socket()
        s.bind(indirizzo)
        s.listen(backlog)
        print("Server inizializzato, ora ascolto...")
    except socket.error as errore:
        print(f"Qualcosa è andato storto, riprovo la connessione... ERR: \n{errore}")
        subServer(indirizzo, backlog = 1)
    conn, indirizzo_client = s.accept()
    
    print(f"Connessione server client stabilita: {indirizzo_client}")
    ricevi_comandi()


def statusSerial(msg):
    global conn

    fixed = str(msg).replace("data:",'')
    fixed = fixed.replace('"','')
    fixed = fixed.replace(' ','')
    fixed = fixed + "%"
    print(fixed)
    conn.sendall(fixed.encode())

if __name__ == '__main__':
    HOST = '10.0.119.69'  # The server's hostname or IP address
    PORT = 12000        # The port used by the server
    rospy.init_node('server_scara') #nome del nodo
    print("init")
    pub = rospy.Publisher('/scara_commands', command ,queue_size = 10)
    sub = rospy.Subscriber('/scara_status', String ,statusSerial)
    subServer((HOST,PORT))

