"""
Daheng Imaging Venus USB cameras for use with OpenCV.

Requires Daheng Imaging's Galaxy SDK and gxipy library:
    
    1. Download and install Galaxy SDK (https://va-imaging.com/pages/customerdownloads )
    2. Copy "<program files>\Daheng Imaging\GalaxySDK\Development\Samples\Python\gxipy" to be a sub folder of your Python script.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel
@copyright: 2025
@version: 2025.04.18
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from Camera import Camera
import gxipy as gx
import numpy as np
import cv2

class DahengVenus(Camera):
    """
    Tested cameras:    
        VEN-161-61U3C-M01 (1440x1080, 1/2.9", 61 fps)
        VEN-505-36U3C-M01 (2592x1944, 1/2.8", 36.9 fps)

    """
    
    modes = {
        'Off'           : gx.GxAutoEntry.OFF,
        'Once'          : gx.GxAutoEntry.ONCE,
        'Continuous'    : gx.GxAutoEntry.CONTINUOUS}
    
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
        # Init device manager (must keep reference to keep API initialized)
        self.__device_manager = gx.DeviceManager()
        
        # Find and open camera
        print('Connecting to camera {}'.format(camera_id))
        device_count, info_list = self.__device_manager.update_device_list()
        
        if camera_id < device_count:
            self.__camera = self.__device_manager.open_device_by_index(camera_id + 1)    # Indexing starts at 1!
        else:
            print('Warning: Camera not found.')
            return

        # Set model name
        vendor = info_list[camera_id].get('vendor_name')
        model = info_list[camera_id].get('model_name')
        self.__name = vendor + ' ' + model
        print('Found camera : {}'.format(self.get_name()))

        # Set pixel format (color or grayscale)
        assert pixel_format in Camera.pixel_formats
        self.__is_mono8 = (pixel_format == 'Mono8')

        # Reset camera to maximum resolution        
        sensor_width = self.__camera.SensorWidth.get()
        sensor_height = self.__camera.SensorHeight.get()
        self.set_resolution(width=sensor_width, height=sensor_height)

        # Set image size (binning)
        if bin_x != 1 or bin_y != 1:
            self.__set_binning(x=bin_x, y=bin_y)
        print(f'Sensor size  : {self.__camera.SensorWidth.get()} x {self.__camera.SensorHeight.get()} px')
        print(f'Image size   : {self.__camera.Width.get()} x {self.__camera.Height.get()} px')
        print(f'Frame rate   : {self.get_frame_rate()}')
                
        # Set acquisition parameters
        self.set_auto_exposure('Continuous')
        self.set_auto_white_balance('Once')
        self.set_auto_gain('Continuous')

        # Start acquisition
        self.__camera.stream_on()

    # -------------------------------------------------------------------------

    def __set_binning(self, x, y):
        """
        Set horizontal and vertical binning.

        Parameters
        ----------
        x : int
            Binning in x-direction.
        y : int
            Binning in y-direction.

        Returns
        -------
        None.

        """
        is_supported = self.__camera.BinningHorizontal.is_writable() and self.__camera.BinningVertical.is_writable()
        
        if is_supported:
            self.__camera.BinningHorizontal.set(x)
            self.__camera.BinningVertical.set(y)
        else:
            print('Warning: Binning not supported')

    # ========== Destructor ===================================================
    
    def release(self):
        """
        Reset camera and release resources.

        Returns
        -------
        None.

        """
        print('Release camera : {}'.format(self.__name))
        self.__camera.stream_off()
        self.__camera.DeviceReset.send_command()
        self.__camera.close_device()

    # ========== Frame grabbing ===============================================
    
    def get_frame(self):
        """
        Grab next frame from camera stream.

        Returns
        -------
        numpy.ndarray
            Camera frame in OpenCV format (i.e., numpy array).

        """
        # Grab frame and correct defective pixels
        raw_image = self.__camera.data_stream[0].get_image()
        if raw_image == None:
            print('Warning: No frame grabbed')
            return np.copy(self.__last_frame)
        
        raw_image.defective_pixel_correct()
        
        # Convert to numpy array
        rgb_image = raw_image.convert("RGB")
        numpy_image = rgb_image.get_numpy_array()

        # Return frame in correct pixel format
        if self.__is_mono8:
            self.__last_frame = cv2.cvtColor(np.asarray(numpy_image), cv2.COLOR_RGB2GRAY)
        else:
            self.__last_frame = cv2.cvtColor(np.asarray(numpy_image), cv2.COLOR_RGB2BGR)
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
            self.__camera.stream_off()
            self.__camera.Width.set(width)
            self.__camera.Height.set(height)
            self.__camera.stream_on()

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
        param_range = self.__camera.ExposureTime.get_range()
        return param_range['min'], param_range['max']

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
        self.__camera.ExposureAuto.set(DahengVenus.modes[mode])

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
        self.__camera.GainAuto.set(DahengVenus.modes[mode])

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
        self.__camera.BalanceWhiteAuto.set(DahengVenus.modes[mode])
            
# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    camera = DahengVenus()
    camera.show_stream()
    camera.release()
 