import numpy as np

# Functions
import HSV_filter as hsv
import shape_recognition as shape
import triangulation as tri
import calibration as calib

from threading import Thread
import cv2, time



# Reading the left and right images.
class ThreadedCamera(object):
    def __init__(self, src=0):
        self.capture = cv2.VideoCapture(src)
        self.capture.set(cv2.CAP_PROP_BUFFERSIZE, 1)

        self.FPS = 1 / 100
        self.FPS_MS = int(self.FPS * 1000)
        # First initialisation self.status and self.frame
        (self.status, self.frame) = self.capture.read()

        # Start frame retrieval thread
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()

    def update(self):
        while True:
            if self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)


thread_cam = ThreadedCamera("rtsp://admin:modern123@192.168.1.56:554/Streaming/Channels/101/")
thread_cam2 = ThreadedCamera("rtsp://admin:modern123@192.168.1.61:554/Streaming/Channels/101/")

# Open both cameras
# cap_right = cv2.VideoCapture("rtsp://admin:modern123@192.168.1.69:554/Streaming/Channels/101/")
# cap_left =  cv2.VideoCapture("rtsp://admin:modern123@192.168.1.56:554/Streaming/Channels/101/")

frame_rate = 120    #Camera frame rate (maximum at 120 fps)

B = 7              #Distance between the cameras [cm]
f = 6               #Camera lense's focal length [mm]
alpha = 110        #Camera field of view in the horisontal plane [degrees]


#Initial values
count = 0

begin_point_right = (0,0)
end_point_right = (0,0)
begin_point_left = (0,0)
end_point_left = (0,0)

# make opencv mouse callback function to update points values for left click and reset for right click
def mouse_callback(event, x, y, flags, param):
    global begin_point_right, end_point_right, begin_point_left, end_point_left, count
    print("point loc:",x,y,count)
    if event == cv2.EVENT_LBUTTONUP:
        if count%4 == 0:
            begin_point_right = (x,y)
            print("begin_point_right:",begin_point_right)
        elif count%4 == 1:
            end_point_right = (x,y)
            print("end_point_right:",end_point_right)
        count += 1

    elif event == cv2.EVENT_RBUTTONUP:

        if count%4 == 2:
            begin_point_left = (x,y)
            print("begin_point_left:",begin_point_left)
        elif count%4 == 3:
            end_point_left = (x,y)
            print("end_point_left:",end_point_left)
        print("reset")
    print(begin_point_left,begin_point_right, end_point_right,end_point_right)


while(True):
    count +=  1

    ret_right=True
    frame_right = thread_cam2.frame
    ret_left=True
    frame_left = thread_cam.frame
    # frame_right = cv2.resize(frame_right, (640,360))
    # frame_left = cv2.resize(frame_left, (640,360))
    # frame_right = cv2.flip(frame_right,0)
    # frame_left = cv2.flip(frame_left,0)
    # frame_right = cv2.flip(frame_right,1)
    # frame_left = cv2.flip(frame_left,1)
    #
    # # print(frame_right.shape)
    # # print(frame_left.shape)
################## CALIBRATION #########################################################

    frame_right, frame_left = calib.undistortRectify(frame_right, frame_left)

########################################################################################

    # If cannot catch any frame, break
    if ret_right==False or ret_left==False:                    
        break

    else:
        # Hough Transforms can be used aswell or some neural network to do object detection

        # draw two lines one per each
        cv2.line(frame_right, begin_point_right, end_point_right, (0, 255, 0), 2)
        cv2.line(frame_left, begin_point_left, end_point_left, (0, 255, 0), 2)


        ################## CALCULATING BALL DEPTH #########################################################

        # If no ball can be caught in one camera show text "TRACKING LOST"
        
        # Function to calculate depth of object. Outputs vector of all depths in case of several balls.
        # All formulas used to find depth is in video presentaion
        depth,x,y,length = tri.find_length(begin_point_right,end_point_right,begin_point_left,end_point_left , frame_right, frame_left, B, f, alpha)

        cv2.putText(frame_right, "TRACKING", (75,50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (124,252,0),2)
        cv2.putText(frame_left, "TRACKING", (75,50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (124,252,0),2)
        cv2.putText(frame_right, "Distance: " + str(round(depth,3)), (200,50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (124,252,0),2)
        cv2.putText(frame_left, "Distance: " + str(round(depth,3)), (200,50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (124,252,0),2)
        cv2.putText(frame_right, "Width: " + str(round(length,3)), (450,50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (124,252,0),2)
        cv2.putText(frame_left, "Width: " + str(round(length,3)), (450,50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (124,252,0),2)

        ###################################################################################################
        # Show the frames
        cv2.imshow("frame right", frame_right)
        cv2.imshow("frame left", frame_left)
        # add mouse callback to frame
        cv2.setMouseCallback('frame right', mouse_callback)
        cv2.setMouseCallback('frame left', mouse_callback)
           



        # Hit "q" to close the window
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


cv2.destroyAllWindows()
