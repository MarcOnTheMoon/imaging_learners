"""
Standard OpenCV camera based on class VideoCapture.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel
@copyright: 2025
@version: 2025.02.21
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from Camera import Camera
import cv2

class CameraCV(Camera):

    # ========== Constructor ==================================================

    def __init__(self, camera_id=0, pixel_format='default', bin_x=1, bin_y=1):
        """
        Initialize the camera.
        
        Parameters
        ----------
        camera_id : int, optional
            Camera ID for all detected cameras of the given model. The default is 0.
        pixel_format : string, optional
            Pixel format as declared in Camera.pixel_formats. The default is 'default'.

        Returns
        -------
        None.

        """
        # Connect to camera
        print('Connecting to camera {}'.format(camera_id))
        self.__camera = cv2.VideoCapture(camera_id)

        # Is camera ready?        
        if not self.__camera.isOpened():
            print('Warning: Could not open camera.')
            return
        print('Found camera : {}'.format(self.get_name()))
                             
        # Set pixel format (color or grayscale)
        assert pixel_format in Camera.pixel_formats
        self.__is_mono8 = (pixel_format == 'mono8')
        
        # Set image size (binning)
        if bin_x != 1 or bin_y != 1:
            self.__camera._set_binning(x=bin_x, y=bin_y)
        width, height = self.get_resolution()
        print(f'Image size   : {width} x {height} px')
        print(f'Frames / sec : {self.get_fps()}')

    # -------------------------------------------------------------------------

    def _set_binning(self, x, y):
        """
        Set horizontal and vertical binning.

        This feature is not supported.

        """
        print('Warning: Binning not supported')

    # ========== Destructor ===================================================
    
    def release(self):
        """
        Release camera resources.

        Returns
        -------
        None.

        """
        print('Release camera : {}'.format(self.get_name()))
        self.__camera.release()

    # ========== Frame grabbing ===============================================
    
    def get_frame(self):
        """
        Grab next frame from camera stream.

        Returns
        -------
        numpy.ndarray
            Camera frame in OpenCV format (i.e., numpy array).

        """
        # Is camera ready?
        if self.__camera.isOpened() is False:
            print('Warning: Camera not ready.')
            return None
    
        # Read frame from camera
        is_success, frame = self.__camera.read()
        if is_success is False:
            print('Warning: No frame grabbed')
            return None
        
        # Return frame in correct pixel format
        if self.__is_mono8:
            return cv2.cvtColor(frame.copy(), cv2.COLOR_BGR2GRAY)
        else:
            return frame.copy()

    # ========== General properties ===========================================

    def get_name(self):
        """
        Get camera model name.

        Returns
        -------
        string
            Fixed string 'OpenCV video capture'.

        """
        return 'OpenCV video capture'

    # -------------------------------------------------------------------------

    def get_resolution(self):
        """
        Get width and height of grabbed frames.

        Returns
        -------
        int
            Width.
        int
            Height.

        """
        return int(self.__camera.get(cv2.CAP_PROP_FRAME_WIDTH)), int(self.__camera.get(cv2.CAP_PROP_FRAME_HEIGHT))

    # -------------------------------------------------------------------------

    def set_resolution(self, name=None, width=None, height=None):
        """
        Set width and height of grabbed frames.
        
        A valid name (e.g., '1080p') or width and height must be passed to the
        method.

        Parameters
        ----------
        name : string, optional
            Resolution type as declared in Camera.resolutions. The default is None.
        width : int, optional
            Width. The default is None.
        height : int, optional
            Height. The default is None.

        Returns
        -------
        None.

        """
        # Look-up standard resolution (e.g., '1080p')
        if name != None:
            width = Camera.resolutions[name]['width']
            height = Camera.resolutions[name]['height']
            
        # Set resolution
        if width != None and height != None:
            self.__camera.set(cv2.CAP_PROP_FRAME_WIDTH, width)
            self.__camera.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    # -------------------------------------------------------------------------

    def get_fps(self):
        """
        Get the speed as frames per second.

        Returns
        -------
        float
            Frames per second.

        """
        return self.__camera.get(cv2.CAP_PROP_FPS)

    # -------------------------------------------------------------------------

    def set_fps(self, fps):
        """
        Set the speed as frames per second.

        Parameters
        ----------
        fps : float
            Frames per second.

        Returns
        -------
        bool
            Returns True if the speed has been set, else False.

        """
        self.__camera.set(cv2.CAP_PROP_FPS, fps)
        return self.get_fps() == fps

    # ========== Auto acquisition adjustments =================================
    
    def set_autofocus(self, switch):
        """
        Enable/disable autofocus.

        Parameters
        ----------
        switch : string
            Switch as declared in Camera.switches.

        Returns
        -------
        None.

        """
        assert switch in Camera.switches
        if switch == 'Off':
            self.__camera.set(cv2.CAP_PROP_AUTOFOCUS, 0.0)
        elif switch == 'On':
            self.__camera.set(cv2.CAP_PROP_AUTOFOCUS, 1.0)
        else:
            print('Warning: Unknown switch: {switch}')
            
    # -------------------------------------------------------------------------

    def set_auto_exposure(self, mode):
        """
        Enable/disable auto exposure.

        Parameters
        ----------
        mode : string
            Mode as declared in Camera.modes.

        Returns
        -------
        None.

        """
        assert mode in Camera.modes
        if mode == 'Off':
            self.__camera.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.0)
        elif mode == 'Once':
            print('Warning: Auto exposure once not implemented')
        elif mode == 'Continuous':
            self.__camera.set(cv2.CAP_PROP_AUTO_EXPOSURE, 1.0)
        else:
            print('Warning: Unknown mode: {mode}')

    # -------------------------------------------------------------------------
    
    def set_auto_gain(self, mode):
        """
        Enable/disable auto gain.

        This feature is not supported.

        """
        print('Warning: Auto gain not supported')

    # -------------------------------------------------------------------------
    
    def set_auto_white_balance(self, mode):
        """
        Enable/disable auto white balance.

        Parameters
        ----------
        mode : string
            Mode as declared in Camera.modes.

        Returns
        -------
        None.

        """
        assert mode in Camera.modes
        if mode == 'Off':
            self.__camera.set(cv2.CAP_PROP_AUTO_WB, 0.0)
        elif mode == 'Once':
            print('Warning: Auto white balance once not implemented')
        elif mode == 'Continuous':
            self.__camera.set(cv2.CAP_PROP_AUTO_WB, 1.0)
        else:
            print('Warning: Unknown mode: {mode}')

# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    camera = CameraCV(camera_id=0)
    camera.show_stream()
    camera.release()
 