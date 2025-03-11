"""
Basler Ace U USB cameras for use with OpenCV.

Requires Basler's pylon library:
    
    1. Download and install pylon (https://www.baslerweb.com/de-de/software/pylon/ )
    2. Install pylon for Python by "pip3 install pypylon" (https://github.com/basler/pypylon )

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel
@copyright: 2025
@version: 2025.03.10
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from Camera import Camera
from pypylon import pylon
import numpy as np

class BaslerAce(Camera):
    """
    Tested cameras:    
        Basler ace U acA1920-40uc (1920×1200, 1/1.2", 41 fps)

    """
    
    # ========== Constructor ==================================================

    def __init__(self, camera_id=0, pixel_format='BGR8', bin_x=1, bin_y=1):
        """
        Initialize the camera.

        Parameters
        ----------
        camera_id : int, optional
            Camera ID for all detected cameras of the given model. The default is 0.
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
        # Find and open camera
        print('Connecting to camera {}'.format(camera_id))
        devices = pylon.TlFactory.GetInstance().EnumerateDevices()
        
        if camera_id < len(devices):
            device = devices[camera_id]
            self.__camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateDevice(device))
            self.__camera.Open()
        else:
            print('Warning: Camera not found.')
            return

        # Set model name
        self.__name = 'Basler ' + device.GetModelName()
        print('Found camera : {}'.format(self.get_name()))

        # Set pixel format (color or grayscale)
        assert pixel_format in Camera.pixel_formats
        self.__camera.PixelFormat.SetValue(pixel_format)

        # Init converter to OpenCV format
        pixel_type = pylon.PixelType_BGR8packed if (pixel_format == 'BGR8') else pylon.PixelType_Mono8
        self.__converter = pylon.ImageFormatConverter()
        self.__converter.OutputPixelFormat = pixel_type
        self.__converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned

        # Set image size (binning)
        if bin_x != 1 or bin_y != 1:
            print('Warning: Binning not supported')
        print(f'Sensor size  : {self.__camera.SensorWidth.Value} x {self.__camera.SensorHeight.Value} px')
        print(f'Image size   : {self.__camera.Width.Value} x {self.__camera.Height.Value} px')
        print(f'Frame rate   : {self.get_frame_rate()} fps')
                
        # Set acquisition parameters
        self.set_auto_exposure('Continuous')
        self.set_auto_gain('Continuous')
        self.set_auto_white_balance('Once')

        # Start acquisition
        self.__last_frame = None
        self.__camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly) 

    # ========== Destructor ===================================================
    
    def release(self):
        """
        Reset camera and release resources.

        Returns
        -------
        None.

        """
        print('Release camera : {}'.format(self.__name))
        self.__camera.StopGrabbing()
        self.__camera.UserSetSelector.SetValue('Default')
        self.__camera.UserSetLoad.Execute()
        self.__camera.Close()

    # ========== Frame grabbing ===============================================
    
    def get_frame(self):
        """
        Grab next frame from camera stream.

        Returns
        -------
        numpy.ndarray
            Camera frame in OpenCV format (i.e., numpy array).

        """
        # Grab frame and convert to OpenCV / numpy
        numpy_image = None
        if self.__camera.IsGrabbing():
            grab_result = self.__camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)
            if grab_result.GrabSucceeded():
                pylon_image = self.__converter.Convert(grab_result)
                numpy_image = np.copy(pylon_image.GetArray())
            grab_result.Release()

        # Return frame
        if numpy_image is None:
            print('Warning: No frame grabbed')
            return np.copy(self.__last_frame) if (self.__last_frame is not None) else None
        self.__last_frame = numpy_image
        return np.copy(self.__last_frame)

    # ========== General properties ===========================================

    def get_name(self):
        """
        Get camera model name.

        Returns
        -------
        string
            Camera manufacturer and model name.

        """
        return self.__name

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
        return int(self.__camera.Width.Value), int(self.__camera.Height.Value)

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
            self.__camera.StopGrabbing()
            self.__camera.Width.SetValue(width)
            self.__camera.Height.SetValue(height)
            self.__camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly) 

    # -------------------------------------------------------------------------

    def get_frame_rate(self):
        """
        Get the acquisition frame rate.

        Returns
        -------
        float
            Frames per second.

        """
        return self.__camera.AcquisitionFrameRate.Value

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
        self.__camera.AcquisitionFrameRate.SetValue(fps)
        return self.get_frame_rate() == fps

    # ========== Acquisition adjustments (image quality) ======================
    
    # ---------- Focus --------------------------------------------------------
    
    def set_autofocus(self, switch):
        """
        Enable/disable autofocus.

        This feature is not supported.

        """
        print('Warning: Autofocus not supported')

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
        return self.__camera.ExposureTime.GetMin(), self.__camera.ExposureTime.GetMax()

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
            self.__camera.ExposureTime.SetValue(time_us)
            return self.__camera.ExposureTime.Value == time_us
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
        self.__camera.ExposureAuto.SetValue(mode)

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
        self.__camera.GainAuto.SetValue(mode)

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
        self.__camera.BalanceWhiteAuto.SetValue(mode)
            
# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    camera = BaslerAce()
    camera.show_stream()
    camera.release()
 