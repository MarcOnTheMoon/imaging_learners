"""
Abstract base class for cameras to be used with OpenCV.

Concrete sub classes for specific camera types must inherit and implement the
methods with the same parameters and return types.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel
@copyright: 2025
@version: 2025.03.06
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from abc import ABC, abstractmethod 
from enum import IntEnum
import cv2

# =============================================================================

# TODO Methods should return a state indicating the success

class OpState(IntEnum):
    FAILED = -2
    NOT_SUPPORTED = -1
    UNKNOWN = 0
    SUCCESS = 1

# =============================================================================

class Camera(ABC):

    pixel_formats = ['BGR8', 'Mono8']
    switches = ['Off', 'On']
    modes = ['Off', 'Once', 'Continuous']
    
    # ========== Constructor ==================================================

    @abstractmethod
    def __init__(self, camera_id, pixel_format, bin_x, bin_y):
        raise NotImplementedError

    # -------------------------------------------------------------------------

    @abstractmethod
    def _set_binning(self, x, y):
        raise NotImplementedError

    # ========== Destructor ===================================================
    
    @abstractmethod
    def release(self):
        raise NotImplementedError

    # ========== Frame grabbing ===============================================
    
    @abstractmethod
    def get_frame(self):
        raise NotImplementedError

    # ========== General properties ===========================================

    @abstractmethod
    
    def get_name(self):
        raise NotImplementedError

    # -------------------------------------------------------------------------

    @abstractmethod
    def get_resolution(self):
        raise NotImplementedError

    @abstractmethod
    def set_resolution(self, width, height):
        raise NotImplementedError

    # -------------------------------------------------------------------------

    @abstractmethod
    def get_frame_rate(self):
        raise NotImplementedError

    @abstractmethod
    def set_frame_rate(self, fps):
        raise NotImplementedError

    # ========== Acquisition adjustments (image quality) ======================
    
    @abstractmethod
    def set_autofocus(self, switch):
        raise NotImplementedError

    # -------------------------------------------------------------------------

    @abstractmethod
    def get_range_exposure_time_us(self):
        raise NotImplementedError
        
    @abstractmethod
    def set_exposure_time_us(self, time_us):
        raise NotImplementedError
    
    @abstractmethod
    def set_auto_exposure(self, mode):
        raise NotImplementedError

    # -------------------------------------------------------------------------
    
    @abstractmethod
    def set_auto_gain(self, mode):
        raise NotImplementedError

    # -------------------------------------------------------------------------
    
    @abstractmethod
    def set_auto_white_balance(self, mode):
        raise NotImplementedError

    # ========== Show camera stream ===========================================
    
    def show_stream(self):
        """
        Diplay camera stream in an OpenCV window.
        
        The stream and window are closed by pressing any key.

        Returns
        -------
        None.

        """
        windowName = self.get_name() + ' (Press any key to terminate)'
        
        while True:
            frame = self.get_frame()
            if frame is not None:
                cv2.imshow(windowName, frame)
            
            if cv2.waitKey(1) > 0:
                cv2.destroyWindow(windowName)
                break

# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':

    # Open camera
#    from CameraCV import CameraCV
#    camera = CameraCV(camera_id=0)

    from AlliedAlvium import AlliedAlvium
    camera = AlliedAlvium(camera_id=0, bin_x=2, bin_y=2)

#    from DahengVenus import DahengVenus
#    camera = DahengVenus(camera_id=0)
#    camera.set_resolution(name='1080p')
    
    # Show frames
    camera.show_stream()
            
    # Release camera and close window
    camera.release()
    cv2.destroyAllWindows()
           