"""
Allied Vision Alvium 1800 cameras for use with OpenCV.

Requires Allied Vision's Vimba for Python library (vmbpy). Download the
installation file vmbpy-*.whl. If you have installed the Vimba X Viewer on your
system, the file may already be present on your system. In my case it was
located in following directory:
    
    C:\Program Files\Allied Vision\Vimba X\api\python
    
For installation call pip with <x.y.z> being the Vimba version:

    pip install '<path>/vmbpy-<x.y.z>-py3-none-any.whl

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2024, Marc Hensel
@version: 2024.12.16
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

from vmbpy import VmbSystem, PixelFormat, FrameStatus
import cv2

class AvAlvium():
    """
    Selected specifications:    
        'Alvium 1800 U-319': 2064×1544, 54 fps
        'Alvium 1800 U-500': 2592×1944, 68 fps

    """

    # ========== Constructor ==================================================

    def __init__(self, cameraID=0, pixelFormat='CV_BGR8', binX=1, binY=1):
        """
        Initialize the camera.
        
        Parameters
        ----------
        cameraID : int, optional
            Camera id within all detected Vimba cameras. The default is 0.
        pixelFormat : string, optional
            Grab 24-bit BGR color or 8-bit gray frames. The default is color ('CV_BGR8').
        binX : int, optional
            Number of pixels to bin horizontally (e.g., 2 will half the width). The default is 1.
        binY : int, optional
            Number of pixels to bin vertically (e.g., 2 will half the height). The default is 1.

        Returns
        -------
        None.

        """
        # Get Vimba system
        self.__vimba = VmbSystem.get_instance()
        
        with self.__vimba:
            # Find camera
            self.__camera = self.__get_camera(cameraID)

            # Setup camera
            with self.__camera:
                # Color or gray values
                self.set_pixel_format(pixelFormat)
                
                # Acquisition
                self.set_auto_exposure('Continuous')
                self.set_auto_white_balance('Continuous')

                # Image size
                self.set_binning(horizontal=binX, vertical=binY)
                print(f'Sensor size  : {self.__camera.SensorWidth.get()} x {self.__camera.SensorHeight.get()} px')
                print(f'Image size   : {self.__camera.Width.get()} x {self.__camera.Height.get()} px')
            
            # Init streaming
            self.__handler = Handler()
            self.__isStreaming = False
                                
    # -------------------------------------------------------------------------

    def __get_camera(self, cameraID):
        """
        Get the Vimba camera.
        
        A camera ID can be specified for the case when more than one Vimba
        cameras are connected to the system. ID 0 corresponds to the first
        camera, ID 1 to the second, and so on.

        Parameters
        ----------
        cameraID : int
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
        if len(cameras) > cameraID:
            camera = cameras[cameraID]
            print('Found camera : {}'.format(camera.get_name()))
        else:
            raise Exception('Alvium camera not found')
                
        return camera

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
        with self.__vimba:
            with self.__camera:
                print('Reset camera : {}'.format(self.__camera.get_name()))
                self.__camera.DeviceReset.run()

    # ========== Setters ======================================================
    
    def set_auto_exposure(self, mode='Continuous'):
        """
        Set the camera feature ExposureAuto.

        Parameters
        ----------
        mode : string, optional
            Value to be set. The default is 'Continuous'.

        Returns
        -------
        None.

        """
        self.__camera.ExposureAuto.set(mode)

    # -------------------------------------------------------------------------

    def set_auto_white_balance(self, mode='Continuous'):
        """
        Set the camera feature BalanceWhiteAuto.

        Parameters
        ----------
        mode : string, optional
            Value to be set. The default is 'Continuous'.

        Returns
        -------
        None.

        """
        self.__camera.BalanceWhiteAuto.set(mode)

    # -------------------------------------------------------------------------
    
    def set_binning(self, horizontal=1, vertical=1):
        """
        Set the pixel binning.
        
        Binning N combines N sensor pixels to a single frame pixel. For
        instance, horizontal = 2 halves the frame width, while vertical = 2
        halves the frame height.
        
        Note: Decreasing the binning, again, seems to have no effect until the
        camera has been reseted and detected, again. The reset command is
        called in the method release().

        Parameters
        ----------
        horizontal : int, optional
            Number of pixels to bin horizontally. The default is 1.
        vertical : int, optional
            Number of pixels to bin vertically. The default is 1.

        Returns
        -------
        None.

        """
        self.__camera.BinningHorizontal.set(horizontal)
        self.__camera.BinningVertical.set(vertical)

    # -------------------------------------------------------------------------
    
    def set_pixel_format(self, pixelFormat):
        """
        Sets the pixel format of grabbed images.
        
        For use with OpenCV, following formats are supported:
            'CV_UINT8' :  8-bit gray (= color format PixelFormat.Mono8)
            'CV_BGR8'  : 24-bit BGR (= color format PixelFormat.Bgr8)
        
        Parameters
        ----------
        pixelFormat : string
            Pixel format of grabbed frames. The default is color ('CV_BGR8').

        Raises
        ------
        Exception
            Raised if pixel format is not supported by the camera

        Returns
        -------
        None.

        """
        assert pixelFormat in ['CV_UINT8', 'CV_BGR8']
        
        pixelFormat = PixelFormat.Bgr8 if (pixelFormat == 'CV_BGR8') else PixelFormat.Mono8
        supportedFormats = self.__camera.get_pixel_formats()
        
        if pixelFormat in supportedFormats:
            self.__camera.set_pixel_format(pixelFormat)
            print('Pixel format : {} (OpenCV)'.format(pixelFormat))
        else:
            raise Exception('Pixel format {} (OpenCV) not supported'.format(pixelFormat))

    # ========== Start / stop streaming =======================================

    def start_streaming(self):
        """
        Start streaming frames.

        Returns
        -------
        None.

        """
        if self.__isStreaming == False:
            self.__camera.start_streaming(handler=self.__handler, buffer_count=10)
            self.__isStreaming = True

    # -------------------------------------------------------------------------

    def stop_streaming(self):
        """
        Stop streaming frames.

        Returns
        -------
        None.

        """
        if self.__isStreaming == True:
            self.__camera.stop_streaming()
            self.__isStreaming = False

    # ========== Getters ======================================================
    
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
    
    def get_frame(self):
        """
        Grab the last frame from the camera stream.

        Returns
        -------
        numpy.ndarray
            Frame in OpenCV compatible numpy format.

        """
        return self.__handler.frame if (self.__isStreaming == True) else None

    # ========== Show camera stream ===========================================
    
    def show_stream(self):
        """
        Diplay camera stream in an OpenCV window.
        
        The stream and window are closed by pressing any key.

        Returns
        -------
        None.

        """
        with self.__vimba:
            with self.__camera:
                windowName = self.get_name() + ' (Press any key to terminate)'
                self.start_streaming()
                
                while True:
                    frame = self.get_frame()
                    if frame is not None:
                        cv2.imshow(windowName, frame)
                    
                    if cv2.waitKey(1) > 0:
                        cv2.destroyWindow(windowName)
                        break
                    
                self.stop_streaming()

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
    alvium = AvAlvium(cameraID=0, pixelFormat='CV_BGR8', binX=2, binY=2)
    alvium.show_stream()    
    alvium.release()
