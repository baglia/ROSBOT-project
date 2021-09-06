#!/usr/bin/env python
import serial
#import serial.tools.list_ports
import sys
import glob
import rospy
import time
import threading
import queue
from scara_pkg.msg import command
from std_msgs.msg import String
pub = None
import sched
#from apscheduler.scheduler import scheduler
from decimal import *
import RPi.GPIO as GPIO

#================= MACCHINA A STATI ====================
_NOT_CONNECTED = 0
_CONNECTED = 1
_ATTESA_COMANDO = 2
_ESECUZIONE_COMANDO = 3
_ERROR = 4
_EMERGENCY = 5
_ATTESA_RESET = 6
stato = _ATTESA_COMANDO
# ================ SERIALE =============================
prec = 2 #NUMERI DOPO LA VIRGOLA
portName = '/dev/ttyACM0'
#portName = ''
baudRate = 115200
timeout = 1

# ============= MESSAGGI ================================
okMessage = [ord('O'),ord('k'),ord('!')]
ErrMessage = [ord('E'),ord('r'),ord('!')]
UnMessage = [ord('U'),ord('n'),ord('!')]
finishCommandMessagge = [ord('F'),ord('n'),ord('s'),ord('!')]
emrMessage = [ord('|'),ord('E'),ord('m'),ord('g'),ord('!')]
rstMessage = [ord('R'),ord('s'),ord('t'),ord('!')]
checkSerialCommand = "A!"

EMRString = "&EMERGENZA"
CMDString = "&COMANDO"
rstString = "&RESET_ESEGUITO"
errString = "&ERRORE"
cmdExecutedString = "&COMANDO_ESEGUITO"


# ================== BOOLEANE =============================
primoMsg = True
emrSTATE = False
isExecutable = True
isError = False

# =================== GPIO =================================
pinEmg = 12


# """ Lists serial port names

#     :raises EnvironmentError:
#         On unsupported or unknown platforms
#     :returns:
#         A list of the serial ports available on the system
# """
# if sys.platform.startswith('win'):
#     ports = ['COM%s' % (i + 1) for i in range(256)]
# elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
#     # this excludes your current terminal "/dev/tty"
#     ports = glob.glob('/dev/tty[A-Za-z]*')
# elif sys.platform.startswith('darwin'):
#     ports = glob.glob('/dev/tty.*')
# else:
#     raise EnvironmentError('Unsupported platform')

# result = []
# for port in ports:
#     try:
#         s = serial.Serial(port)
#         s.close()
#         result.append(port)
#     except (OSError, serial.SerialException):
#         pass

# #seriale = serial.Serial(port = portName, baudrate = baudRate)
# for p in result:
#     try:
#         seriale = serial.Serial(port = p, baudrate = baudRate)
#         print("Tento la connessione con: " + p)
#         time.sleep(2)
#         seriale.write(checkSerialCommand.encode())
#         risposta = []
#         inp = 'a'
#         t = time.time()
#         while(ord(inp) != ord('!')):
#             if(seriale.in_waiting > 0):
#                 inp = seriale.read()
#                 risposta.append(ord(inp))
#             if(time.time() - t > timeout):
#                 print("[ERR] Timeout")
#                 break
#         print("La risposta è: " + risposta)
#         if(risposta == ord(ErrMessage)):
#             portName = str(p)
#             print("Seriale connessa con il dispositivo: " + portName)
#             break
#     except:
#         print("[ERR] Connessione seriale con" + p + "non riuscita ...")

try:
    seriale = serial.Serial(port = portName, baudrate = baudRate)
    seriale.stopbits = 1
    print("Seriale connessa con il dispositivo: " + portName)
except:
    print("[ERR] Connessione seriale non riuscita ...")

q = queue.Queue(maxsize = -1)
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pinEmg,GPIO.OUT)
GPIO.output(pinEmg, 1)

def main():
    global pub
    global stato

    getcontext().prec = 2   
    rospy.init_node('comSerial_scara') #nome del nodo
    sub = rospy.Subscriber('/scara_commands', command ,comSerial)
    pub = rospy.Publisher('/scara_status', String ,queue_size = 10)
    print("Nodo Inizializzato correttamente!")
    #rospy.spin()
    rate = rospy.Rate(50) #50Hz
    stato = _ATTESA_COMANDO

    while not rospy.is_shutdown():
        #print(stato)
        if stato == _ATTESA_COMANDO:
            attesa_comando()
        elif stato == _ESECUZIONE_COMANDO:
            esecuzione_comando()
        elif stato == _ERROR:
            statoErrore()
        elif stato == _NOT_CONNECTED:
            nonConnesso()
        elif stato == _ATTESA_RESET:
            attesaReset()
        elif stato == _EMERGENCY:
            statoEmergenza()
        
        rate.sleep()

def attesa_comando():
    commandManager()

def esecuzione_comando():
    global stato
    if(seriale.in_waiting > 0):                   
        cmd = readFromSerial()
        if cmd == emrMessage:
            stato = _EMERGENCY
        elif cmd == finishCommandMessagge:
            pub.publish(cmdExecutedString)
            print("Comando eseguito, passo al prossimo...")
            stato = _ATTESA_COMANDO
        else:
            stato = _ERROR

def statoErrore():
    global stato
    pub.publish(errString)
    stato = _ATTESA_RESET
    attesaReset()

def nonConnesso():
    global stato
    global seriale

    try:
        seriale.connect()
        print("Seriale connessa")
        stato = _ATTESA_RESET
    except:
        print("Collega la seriale ...")

def attesaReset():
    print("ATTENDO IL RESET...")

def clearSerial():
    global seriale
    a = ''
    while(seriale.in_waiting > 0):
        a = seriale.read()

def isFromSerial(msg):
    risposta = []
    inp = 'a'
    t = time.time()   
    while(ord(inp) != ord('!')):
        if(seriale.in_waiting > 0):
            inp = seriale.read()
            risposta.append(ord(inp))
        if(time.time() - t > timeout):
            print("[ERR] Timeout")
            break
    if(msg == risposta):
        return True
    else:
        return False

def readFromSerial():
    risposta = []
    inp = 'a'
    t = time.time()
    while(ord(inp) != ord('!')):
        if(seriale.in_waiting > 0):
            inp = seriale.read()
            risposta.append(ord(inp))
        if(time.time() - t > timeout):
            print("[ERR] Timeout")
            break
    print("Risposta vale:")
    print(risposta)
    return risposta

def sendSerial(messaggio):
    seriale.write(messaggio.encode())    
    verifica = 0 
    risposta = [] 
    risposta = readFromSerial()
    return risposta

def statoEmergenza():
    global stato
    emrProcedure()
    stato = _ATTESA_RESET


def commandManager():
    global stato
    global seriale

    if( seriale.in_waiting > 0):
        if(isFromSerial(emrMessage)):
            stato = _EMERGENCY
        else:
            stato =_ERROR

    if q.empty() == False:
        msg = q.get()
        print(msg)
        risposta = sendSerial(msg)

        if(risposta == okMessage):
            print("Risposta: Ok!")
            stato = _ESECUZIONE_COMANDO
            pub.publish(CMDString + ": " + msg)
            print("Rimuovo il messaggio " + msg + " dalla coda" )
        
        elif(risposta == ErrMessage):
            print("Risposta: Er!")
            stato = _ERROR
            
        elif(risposta == UnMessage):
            print("[ERR] Il punto non è raggiungibile ...")
            stato = _ERROR

        elif(risposta == finishCommandMessagge):
            print("Comando eseguito (FINISH)")
            stato = _ERROR

        elif(risposta == emrMessage):
            stato = _EMERGENCY
            print("Stato di emergenza")

        else:
            print("[ERR] Nessuna risposta: chiudo la porta seriale ...")
            stato = _NOT_CONNECTED

def comSerial(_command): 
    global isExecutable
    global emrSTATE
    global seriale
    global stato

    type = _command.commandType
    zCommand = _command.zAxisCommand
    targetPoint = _command.nextPoint
    speed = _command.speed
    acceleration = _command.acceleration
    targetX = Decimal(_command.nextPoint.x)
    targetY = Decimal(_command.nextPoint.y)
    targetZ = Decimal(_command.nextPoint.z)
    speed = Decimal(_command.speed)
    acceleration = Decimal(_command.acceleration)
    targetX = round(targetX,prec)
    targetY = round(targetY,prec)
    targetZ = round(targetZ,prec)
    speed = round(speed,prec) 
    acceleration = round(acceleration,prec)  
    gripper = _command.gripper
    delay = _command.waitTime
    messaggio = ""

    if type == "M1" or type == "M2" or type == "M3" or type == "M4":
        parametri = "X"+str(targetX) + "Y"+str(targetY) + "V"+ str(speed)
        messaggio = type + parametri
    if type == "M5":
        parametri = "P"+str(targetX) + "S"+str(targetY) + "V"+ str(speed)
        messaggio = type + parametri
    if type == "M6":
        messaggio = type + "Z"+ str(zCommand)
    if type == "M7":
        messaggio = type + "S"+ str(gripper)
    if type == "M8" or type == "S2" or type == "S3":
        messaggio = type
    if type == "C1":
        messaggio = type + "T" + str(delay)
    if type == "S4":
        messaggio = type + "A" + str(acceleration)
    messaggio = messaggio + "!"

    if(messaggio == "S3!"):
        emrProcedure()

    elif(messaggio == "S2!"):
            clearSerial()
            risposta = sendSerial(messaggio)
            if risposta == okMessage:
                if(isFromSerial(finishCommandMessagge)):
                    clearSerial()
                    q.queue.clear()
                    stato = _ATTESA_COMANDO
                    pub.publish(rstString)
                    print("RESET effettuato")
    else:
        q.put(messaggio)
        print("Aggiungo " + messaggio + " alla coda")
    #sendSerial(messaggio)


def emrProcedure():
    global stato
    GPIO.output(pinEmg, 0)
    time.sleep(0.01)
    GPIO.output(pinEmg, 1)
    clearSerial()
    stato = _EMERGENCY
    pub.publish(EMRString)


if __name__ == '__main__':
    main()
    


