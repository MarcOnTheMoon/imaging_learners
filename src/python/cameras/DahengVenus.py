"""
Daheng Imaging Venus USB cameras for use with OpenCV.

Requires Daheng Imaging's gxipy library located in the respective sub folder.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025
@version: 2025.02.18
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from Camera import Camera
import gxipy as gx
import numpy as np
import cv2

class DahengVenus(Camera):
    """
    Selected specifications:    
        'VEN-161-61U3C-M01': 1440x1080, 61 fps
        'VEN-505-36U3C-M01': 2592x1944, 36.9 fps

    """
    
    modes = {
        'Off'           : gx.GxAutoEntry.OFF,
        'Once'          : gx.GxAutoEntry.ONCE,
        'Continuous'    : gx.GxAutoEntry.CONTINUOUS}
    
    # ========== Constructor ==================================================

    def __init__(self, camera_id=0, pixel_format='default'):
        """
        Initialize the camera.
        
        Pixel formats as declared in Camera.pixel_formats:
            'Default' - camera default (typically 24-bit BGR color)
            'Mono8'   - 8-bit grayscale

        Parameters
        ----------
        camera_id : int, optional
            Camera ID for all detected cameras of the given model. The default is 0.
        pixel_format : string, optional
            Pixel format of grabbed frames. The default is 'default'.

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
            print('WARNING: Camera not found.')
            return

        # Set model name
        vendor = info_list[camera_id].get('vendor_name')
        model = info_list[camera_id].get('model_name')
        self.__name = vendor + ' ' + model

        # Remember pixel format
        assert pixel_format in Camera.pixel_formats
        self.__is_mono8 = (pixel_format == 'mono8')

        # Reset camera to maximum resolution        
        sensor_width = self.__camera.SensorWidth.get()
        sensor_height = self.__camera.SensorHeight.get()
        self.set_resolution(width=sensor_width, height=sensor_height)
        
        # Start acquisition
        self.__camera.stream_on()

    # ========== Destructor ===================================================
    
    def release(self):
        """
        Reset camera and release resources.

        Returns
        -------
        None.

        """
        self.__camera.stream_off()
        self.__camera.DeviceReset.send_command()
        self.__camera.close_device()

    # ========== Grab frames ==================================================
    
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
            return self.__last_frame
        
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
            Model name and serial number.

        """
        return self.__name

    # ========== Image format =================================================

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
            self.__camera.stream_off()
            self.__camera.Width.set(width)
            self.__camera.Height.set(height)
            self.__camera.stream_on()

    # -------------------------------------------------------------------------

    def set_binning(self, x, y):
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
            self.__camera.stream_off()
            self.__camera.BinningHorizontal.set(x)
            self.__camera.BinningVertical.set(y)
            self.__camera.stream_on()
        else:
            print('WARNING: Binning not supported')

    # ========== Frame rate ===================================================

    def get_fps(self):
        """
        Get the speed as frames per second.

        Returns
        -------
        float
            Frames per second.

        """
        return self.__camera.AcquisitionFrameRate.get()

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
        print(self.__camera.AcquisitionFrameRate.get_range())
        self.__camera.AcquisitionFrameRate.set(fps)
        return self.get_fps() == fps

    # ========== Auto acquisition adjustments =================================
    
    def set_autofocus(self, switch):
        """
        Enable/disable autofocus.

        This feature is not supported.

        """
        print('WARNING: Autofocus not supported')

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

    # -------------------------------------------------------------------------
    
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
        self.__camera.BalanceWhiteAuto.set(DahengVenus.modes[mode])
            
        