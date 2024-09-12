import zmq
import numpy as np
import cv2
import json
import time

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://192.168.43.148:5555")

socket.setsockopt_string(zmq.SUBSCRIBE, '')

CX = 0
CY = 0
centerSet = False
W = 0
H = 0

def setCenter(w, h):
    global CX, CY, W, H, centerSet
    CX = w // 2
    CY = h // 2
    W = w
    H = h
    print("cx,cy ", CX, CY) 
    centerSet = True

def superLine(frame, p1, p1prev, color):
    if p1prev is not None:
        p1dx = p1[0] - p1prev[0]
        p1dy = p1[1] - p1prev[1]
        p2dx = p2[0] - p2prev[0]
        p2dy = p2[1] - p2prev[1]
        kp1 = 65535
        bp1 = 0
        if not p1dx == 0:
            kp1 = p1dy / p1dx
            bp1 = p1[1] - p1[0] * kp1
            #print("bp1 = ", bp1)
            #print("y2 = ", int(W*kp1+bp1))
            x0 = 0 
            y0 = int(bp1) 
            if y0 < 0: 
                y0 = 0 
                x0 = int(-bp1 / kp1) 
            if y0 > H: 
                y0 = H 
                x0 = int((H - bp1) / kp1) 
            x1 = W 
            y1 = int(W*kp1+bp1)
            if y1 < 0: 
                y1 = 0 
                x1 = int(-bp1 / kp1) 
            if y1 > H: 
                y1 = H 
                x1 = int((H - bp1) / kp1) 
            # print("x0 = ", x0) 
            # print("y0 = ", y0) 
            # print("x1 = ", x1) 
            # print("y1 = ", y1) 
            color = (255,0,0)
            if p1dx < 0: color = (0,0,255)
            cv2.line(frame, (x0, y0), (x1, y1), color, 1)
            cv2.line(frame, p1, p1prev, color, 3)
        else:
            cv2.line(frame, (p1[0], 0), (p1[0], H), color, 1)
        return kp1, bp1

def drawBBHistory(frame, BBHistory):
    if len(BBHistory) > 0:
        qc = 200 // len(BBHistory)
        i = 0
        for bb in BBHistory:
            cx = int(bb[0] + bb[2] / 2)
            cy = int(bb[1] + bb[3] / 2)
            vc = qc * i
            cv2.circle(frame, (cx, cy), 5, (vc,vc,vc), 2)
            cv2.circle(frame, (bb[0], bb[1]), 5, (vc,vc,vc), 2)
            cv2.circle(frame, (bb[0]+bb[2], bb[1]+bb[3]), 5, (vc,vc,vc), 2)

            i += 1

vvr = cv2.VideoWriter(time.strftime("%Y%m%d-%H%M%S.avi"),  
                         cv2.VideoWriter_fourcc(*'MJPG'), 
                         10, (720,576))

p1prev = None
p2prev = None

while True:
    message = socket.recv()
    telemetry = socket.recv_string()
    #print("tel: ", telemetry)
    np_array = np.frombuffer(message, dtype=np.uint8)
    
    frame = cv2.imdecode(np_array, 1)
    #str = '{ "x": 10 }'
    J = json.loads(telemetry)
    #json_object = json.loads(str)
    p1 = (J["rect"][0], J["rect"][1])
    p2 = (J["rect"][0]+J["rect"][2], J["rect"][1]+J["rect"][3])
    pc = (J["rect"][0]+J["rect"][2] // 2, J["rect"][1]+J["rect"][3] // 2)
    mode = J["cMode"]
    #bbHistory = J["bbHist"]
    bbHistory = []
    if "bbHist" in J: bbHistory = J["bbHist"]
    ##rollPID = J["rollPid"]
    if frame is not None:
        if not (centerSet):
            setCenter(frame.shape[1], frame.shape[0])

        color = (255, 0, 0)
        cv2.line(frame, (0, CY), (W, CY), color, 2)
        cv2.line(frame, (0, CY // 2), (W, CY // 2), color, 2)
        cv2.line(frame, (0, 3 * CY // 2), (W, 3 * CY // 2), color, 2)
        cv2.line(frame, (CX, 0), (CX, H), color, 2)
        cv2.line(frame, (CX // 2, 0), (CX // 2, H), color, 2)
        cv2.line(frame, (3 * CX // 2, 0), (3 * CX // 2, H), color, 2)
        dd = 40
        sc = 0.8

        cv2.putText(frame, "Est: "+str(J["EstAlt"])+", LAlt: "+str(J["lastAlt"])+", G0: ", 
                          (50, 25), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)

        cv2.putText(frame, "rRol: "+str(J["RC"][0]), (50, 50), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "rPit: "+str(J["RC"][1]), (50, 50+dd), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "rYaw: "+str(J["RC"][2]), (50, 50+dd*2), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "rThr: "+str(J["RC"][3]), (50, 50+dd*3), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)

        dx = CX - (p1[0] + p2[0]) // 2
        dy = CY - (p1[1] + p2[1]) // 2
         
        cv2.putText(frame, "dx: "+str(dx), (50, 50+dd*4), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "dy: "+str(dy), (50, 50+dd*5), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)


        cv2.putText(frame, "oRol: "+str(J["RCOver"][0]), (50, 300), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "oPit: "+str(J["RCOver"][1]), (50, 300+dd), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "oYaw: "+str(J["RCOver"][2]), (50, 300+dd*2), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)
        cv2.putText(frame, "oThr: "+str(J["RCOver"][3]), (50, 300+dd*3), cv2.FONT_HERSHEY_SIMPLEX, sc, (255,255,255), 2, cv2.LINE_AA)

        cv2.putText(frame, "FPS "+str(J["tFPS"]), (550, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 2, cv2.LINE_AA)

        color = (0,255, 0)
        if mode == 1: 
            color = (0,255,255)
        if mode == 2: 
            color = (0,0,255)
            cv2.line(frame, pc, (CX, CY), color, 2)
        
        cv2.rectangle(frame, p1, p2, color, 2)

        drawBBHistory(frame, bbHistory)
        vvr.write(frame)
        cv2.imshow("Received Frame", frame)
        #print("Received frame w", frame.shape)
        #print("Received telem:", telemetry)
        #print("x:", J['x'])
        if cv2.waitKey(10) >= 0:
            break
    
    p1prev = p1
    p2prev = p2
vvr.release()
cv2.destroyAllWindows()
