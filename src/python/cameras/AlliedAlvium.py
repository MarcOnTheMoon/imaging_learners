"""
Allied Vision Alvium 1800 cameras for use with OpenCV.

Requires Allied Vision's Vimba for Python library (vmbpy). Download the
installation file vmbpy-*.whl. If you have installed the Vimba X Viewer
(https://www.alliedvision.com/de/produktportfolio/software/vimba-x-sdk/ ), the
file may already be present on your system. In my case it was located in
following directory:
    
    C:\Program Files\Allied Vision\Vimba X\api\python
    
For installation call pip with <x.y.z> being the Vimba version:

    pip install '<path>/vmbpy-<x.y.z>-py3-none-any.whl

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel
@copyright: 2025
@version: 2025.03.06
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from Camera import Camera
from vmbpy import VmbSystem, PixelFormat, FrameStatus
import numpy as np
import cv2

class AlliedAlvium(Camera):
    """
    Tested cameras:    
        Alvium 1800 U-319 (2064×1544, 1/1.8", 54 fps)
        Alvium 1800 U-500 (2592×1944, 1/2.5", 68 fps)

    """

    # ========== Constructor ==================================================

    def __init__(self, camera_id=0, pixel_format='BGR8', bin_x=1, bin_y=1):
        """
        Initialize the camera.
        
        Parameters
        ----------
        camera_id : int, optional
            Camera id within all detected Vimba cameras. The default is 0.
        pixel_format : string, optional
            Pixel format as declared in Camera.pixel_formats. The default is 'BGR8'.
        bin_x : int, optional
            Number of pixels to bin horizontally (e.g., 2 will half the width). The default is 1.
        bin_y : int, optional
            Number of pixels to bin vertically (e.g., 2 will half the height). The default is 1.

        Returns
        -------
        None.

        """
        # Get Vimba system
        self.__vimba = VmbSystem.get_instance()
        self.__vimba.__enter__()
        
        # Find camera
        self.__camera = self.__get_camera(camera_id)
        self.__camera.__enter__()

        # Set pixel format (color or grayscale)
        assert pixel_format in Camera.pixel_formats
        self.__set_pixel_format('CV_BGR8' if pixel_format == 'BGR8' else 'CV_UINT8')

        # Set image size (binning)
        if bin_x != 1 or bin_y != 1:
            self.__set_binning(x=bin_x, y=bin_y)
        print(f'Sensor size  : {self.__camera.SensorWidth.get()} x {self.__camera.SensorHeight.get()} px')
        print(f'Image size   : {self.__camera.Width.get()} x {self.__camera.Height.get()} px')
        print(f'Frames / sec : {self.get_frame_rate()}')
        
        # Set acquisition parameters
        self.set_auto_exposure('Continuous')
        self.set_auto_white_balance('Continuous')
        self.set_auto_gain('Continuous')
                                        
        # Start acquisition
        print('Start streaming ... ', end="")
        self.__last_frame = None
        self.__handler = Handler()
        self.__camera.start_streaming(handler=self.__handler, buffer_count=10)
        while not self.__camera.is_streaming():
            cv2.waitKey(50)
        cv2.waitKey(500)
        print('done')

    # -------------------------------------------------------------------------

    def __get_camera(self, camera_id):
        """
        Get the Vimba camera.
        
        A camera ID can be specified for the case when more than one Vimba
        cameras are connected to the system. ID 0 corresponds to the first
        camera, ID 1 to the second, and so on.

        Parameters
        ----------
        camera_id : int
            Camera id within all detected Vimba cameras

        Raises
        ------
        Exception
            Raised if no camera was found.

        Returns
        -------
        camera : Camera
            Vimba camera that was found

        """
        cameras = self.__vimba.get_all_cameras()
        if len(cameras) > camera_id:
            camera = cameras[camera_id]
            print('Found camera : {}'.format(camera.get_name()))
        else:
            raise Exception('Alvium camera not found')
                
        return camera

    # -------------------------------------------------------------------------
    
    def __set_pixel_format(self, pixel_format):
        """
        Sets the pixel format of grabbed images.
        
        For use with OpenCV, following formats are supported:
            'CV_UINT8' :  8-bit gray (= color format pixel_format.Mono8)
            'CV_BGR8'  : 24-bit BGR (= color format pixel_format.Bgr8)
        
        Parameters
        ----------
        pixel_format : string
            Pixel format of grabbed frames. The default is color ('CV_BGR8').

        Returns
        -------
        None.

        """
        assert pixel_format in ['CV_UINT8', 'CV_BGR8']
        
        pixel_format = PixelFormat.Bgr8 if (pixel_format == 'CV_BGR8') else PixelFormat.Mono8
        supported_formats = self.__camera.get_pixel_formats()
        
        if pixel_format in supported_formats:
            self.__camera.set_pixel_format(pixel_format)
            print('Pixel format : {} (OpenCV)'.format(pixel_format))
        else:
            raise Exception('Pixel format {} (OpenCV) not supported'.format(pixel_format))

    # -------------------------------------------------------------------------
    
    def __set_binning(self, x=1, y=1):
        """
        Set the pixel binning.
        
        Binning N combines N sensor pixels to a single frame pixel. For
        instance, x = 2 halves the frame width, while y = 2
        halves the frame height.
        
        Note: Decreasing the binning, again, seems to have no effect until the
        camera has been reseted and detected, again. The reset command is
        called in the method release().

        Parameters
        ----------
        x : int, optional
            Number of pixels to bin horizontally. The default is 1.
        y : int, optional
            Number of pixels to bin vertically. The default is 1.

        Returns
        -------
        None.

        """
        self.__camera.BinningHorizontal.set(x)
        self.__camera.BinningVertical.set(y)

    # ========== Destructor ===================================================
    
    def release(self):
        """
        Reset camera.
        
        Reset is required, for instance, for binning. If horizontal or vertical
        binning has been applied to get smaller frames, the frame size will not
        increase, anymore, even when the binning values are set to 1. Calling
        the device reset solves this issue.

        Returns
        -------
        None.

        """
        print('Release camera : {}'.format(self.__camera.get_name()))
        self.__camera.stop_streaming()
        self.__camera.DeviceReset.run()
        self.__camera.__exit__(None, None, None)
        self.__vimba.__exit__(None, None, None)

    # ========== Frame grabbing ===============================================
    
    def get_frame(self):
        """
        Grab the last frame from the camera stream.

        Returns
        -------
        numpy.ndarray
            Frame in OpenCV compatible numpy format.

        """
        frame = self.__handler.frame
        if frame is None:
            print('Warning: No frame grabbed')
            return np.copy(self.__last_frame) if (self.__last_frame is not None) else None

        self.__last_frame = frame
        return np.copy(self.__last_frame)

    # ========== General properties ===========================================

    def get_name(self):
        """
        Get camera's name.

        Returns
        -------
        string
            Camera model name.

        """
        return self.__camera.get_name()

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
        return int(self.__camera.Width.get()), int(self.__camera.Height.get())

    # -------------------------------------------------------------------------

    def set_resolution(self, width=None, height=None):
        """
        Set width and height of grabbed frames.

        Parameters
        ----------
        width : int, optional
            Width. The default is None.
        height : int, optional
            Height. The default is None.

        Returns
        -------
        None.

        """
        if width != None and height != None:
            self.__camera.stop_streaming()
            self.__camera.Width.set(width)
            self.__camera.Height.set(height)
            self.__camera.start_streaming(handler=self.__handler, buffer_count=10)

    # -------------------------------------------------------------------------
    
    def get_frame_rate(self):
        """
        Get the acquisition frame rate.

        Returns
        -------
        float
            Frames per second.

        """
        return self.__camera.AcquisitionFrameRate.get()

    # -------------------------------------------------------------------------

    def set_frame_rate(self, fps):
        """
        Set the acquisition frame rate.

        Parameters
        ----------
        fps : float
            Frames per second.

        Returns
        -------
        bool
            Returns True if the speed has been set, else False.

        """
        self.__camera.AcquisitionFrameRate.set(fps)
        return self.get_frame_rate() == fps


    # ========== Acquisition adjustments (image quality) ======================
    
    # ---------- Focus --------------------------------------------------------
    
    def set_autofocus(self, switch):
        """
        Enable/disable autofocus.

        This feature is not supported.

        """
        print('WARNING: Autofocus not supported')

    # ---------- Exposure time ------------------------------------------------

    def get_range_exposure_time_us(self):
        """
        Get parameter range of exposure time in microseconds.

        Returns
        -------
        float
            Minimum valid exposure time in [us].
        float
            Maximum valid exposure time in [us].

        """
        param_range = self.__camera.ExposureTime.get_range()
        return param_range[0], param_range[1]

    # -------------------------------------------------------------------------
        
    def set_exposure_time_us(self, time_us):
        """
        Set the exposure time in microseconds.

        Parameters
        ----------
        time_us : float
            Exposure time in [us].

        Returns
        -------
        bool
            True on success, else False.

        """
        # Get supported parameter range
        min_us, max_us = self.get_range_exposure_time_us()
        
        # Set parameter
        if min_us <= time_us <= max_us:
            self.set_auto_exposure('Off')
            self.__camera.ExposureTime.set(time_us)
            return self.__camera.ExposureTime.get() == time_us
        else:
            print(f'Warning: Exposure time not in range [{min_us}, {max_us}] us')
            return False

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
        self.__camera.ExposureAuto.set(mode)

    # ---------- Gain ---------------------------------------------------------
    
    def set_auto_gain(self, mode):
        """
        Enable/disable auto gain.

        Parameters
        ----------
        mode : string
            Mode as declared in Camera.modes.

        Returns
        -------
        None.

        """
        assert mode in Camera.modes
        self.__camera.GainAuto.set(mode)

    # ---------- White balance ------------------------------------------------

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
        self.__camera.BalanceWhiteAuto.set(mode)

# ========== Handler ==========================================================

class Handler():
    """
    Class providing callback method for grabbed frames to the camera.
    
    """

    def __init__(self):
        self.frame = None

    # -------------------------------------------------------------------------
    
    def __call__(self, cam, stream, frame):
        if frame.get_status() == FrameStatus.Complete:
            self.frame = frame.as_opencv_image()      # TODO Clone image?
        cam.queue_frame(frame)

# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    camera = AlliedAlvium(bin_x=2, bin_y=2)
    camera.show_stream()
    camera.release()
