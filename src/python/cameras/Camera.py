"""
Abstract base class for cameras to be used with OpenCV.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.02.18
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from abc import ABC, abstractmethod 

class Camera(ABC):

    """
    Pixel formats of grabbed frames:
        'default' - camera default (typically 24-bit BGR color)
        'mono8'   - 8-bit grayscale    
    """
    pixel_formats = ['default', 'mono8']

    resolutions = {
        '720p'   : { 'width': 1280, 'height': 720 },    # HD
        '1080p'  : { 'width': 1920, 'height': 1080 }    # Full HD
        }
    
    switches = ['Off', 'On']
    modes = ['Off', 'Once', 'Continuous']
    
    # ========== Constructor ==================================================

    @abstractmethod
    def __init__(self, camera_id, pixel_format):
        pass

    # ========== Destructor ===================================================
    
    @abstractmethod
    def release(self):
        pass

    # ========== Frame grabbing ===============================================
    
    @abstractmethod
    def get_frame(self):
        pass

    # ========== General properties ===========================================

    @abstractmethod
    
    def get_name(self):
        pass

    # ========== Image format =================================================

    @abstractmethod
    def get_resolution(self):
        pass

    # -------------------------------------------------------------------------

    @abstractmethod
    def set_resolution(self, name, width, height):
        pass

    # -------------------------------------------------------------------------

    @abstractmethod
    def set_binning(self, x, y):
        pass

    # ========== Frame rate ===================================================

    @abstractmethod
    def get_fps(self):
        pass

    # -------------------------------------------------------------------------

    @abstractmethod
    def set_fps(self, fps):
        pass

    # ========== Auto acquisition adjustments =================================
    
    @abstractmethod
    def set_autofocus(self, switch):
        pass

    # -------------------------------------------------------------------------

    # TODO Set exposure time

    @abstractmethod
    def set_auto_exposure(self, mode):
        pass

    # -------------------------------------------------------------------------
    
    @abstractmethod
    def set_auto_gain(self, mode):
        pass

    # -------------------------------------------------------------------------
    
    @abstractmethod
    def set_auto_white_balance(self, mode):
        pass

# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    from CameraCV import CameraCV
    from AlliedAlvium import AlliedAlvium
    from DahengVenus import DahengVenus
    import cv2
    
    # Open camera
#    camera = CameraCV(camera_id=0)
#    camera = AlliedAlvium(camera_id=0, bin_x=2, bin_y=2)
    camera = DahengVenus(camera_id=0)
#    camera.set_resolution(name='1080p')
    
    # Print properties
    name = camera.get_name()
    width, height = camera.get_resolution()
    fps = camera.get_fps()
    print(f'Name : {name}')
    print(f'Size : {width}x{height}')
    print(f'fps  : {fps}')
    
    # Set properties
    camera.set_autofocus('On')
    camera.set_auto_gain('Continuous')
    camera.set_auto_exposure('Off')
    camera.set_auto_white_balance('Once')

    # Show frames
    window_name = name + ' (Press any key to terminate)'
    frame = camera.get_frame()
    cv2.imshow(window_name, frame)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_TOPMOST, 1)

    wait_time_ms = int(1000.0 / fps)
    while True:
        frame = camera.get_frame()
#        frame = cv2.resize(frame, None, fx=0.25, fy=0.25)

        cv2.imshow(window_name, frame)
        if cv2.waitKey(wait_time_ms) > 0:
             break
            
    # Release camera and close window
    print('Release camera')
    camera.release()
    cv2.destroyAllWindows()
