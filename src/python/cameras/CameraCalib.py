"""
Camera calibration using ChArUco boards.

Requires OpenCV library with contributions:
    
    pip install opencv-contrib-python

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025
@version: 2025.02.20
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import numpy as np
import json

class CameraCalib():

    # ========== Constructor ==================================================
    
    # TODO Impact of field sizes on calibration data (i.e., calculating mm from pixels)?
    # TODO Method to read data from JSON file
    # TODO Method to undistort images

    def __init__(self, squares_x=7, squares_y=5, square_px=200, marker_px=150, margin=20):
        """
        Constructor.

        Parameters
        ----------
        squares_x : int, optional
            Number of chessboard fields in horizontal direction. The default is 7.
        squares_y : int, optional
            Number of chessboard fields in vertical direction. The default is 5.
        square_px : int, optional
            Size of chessboard fields in pixel. The default is 200.
        marker_px : int, optional
            Size of ArUco markers in pixel. The default is 150.
        margin : int, optional
            Margin (empty space) around chessboard. The default is 20.

        Returns
        -------
        None.

        """
        # Dimensions
        self.__x = squares_x
        self.__y = squares_y
        self.__square_px = square_px
        self.__marker_px = marker_px
        self.__margin = margin
        
        # Init board
        dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
        self.__board = cv2.aruco.CharucoBoard((self.__x, self.__y), square_px, marker_px, dictionary)

    # ========== Save board to image file =====================================

    def save_board_image(self):
        """
        Save ChArUco board to image file.

        Returns
        -------
        None.

        """
        # Create image
        image_size = tuple(i * self.__square_px + 2 * self.__margin for i in (self.__x, self.__y))
        image = self.__board.generateImage(outSize=image_size, img=None, marginSize=self.__margin)
        
        # Display image
        cv2.imshow('Board', image)
        
        # Save image to file
        file_name = f'./CameraCalib_4x4-50_{self.__x}x{self.__y}.png'
        cv2.imwrite(file_name, image)
        print(f'Image saved to file: {file_name}')

    # ========== Calibrate ====================================================
    
    def __add_image(self, image):
        """
        Analyse ChArUco image and add detected corners to calibration data.

        Parameters
        ----------
        image : numpy.ndarray
            Image containing ChArUco board.

        Returns
        -------
        None.

        """
        # Detect ArUco markers and chessboard corners
        detector = cv2.aruco.CharucoDetector(self.__board)
        corners, ids, marker_corners, marker_ids = detector.detectBoard(image)
 
        # Visualize ArUco markers (scaled down by 1/2)
        marker_image = image.copy()
        cv2.aruco.drawDetectedMarkers(marker_image, marker_corners, marker_ids)
        marker_image = cv2.resize(marker_image, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_LINEAR)
        cv2.imshow('Detected ArUco markers', marker_image)

        # Add ChArUco corners and ids
        if ids is not None and len(corners) > 3:
            self.__charuco_corners.append(corners)
            self.__charuco_ids.append(ids)
        else:
            print('Not enough corners detected')

    # -------------------------------------------------------------------------

    def calibrate(self, camera, camera_name, lens_name):
        """
        Interactively calibrate the camera using images of the ChArUco board.
        
        Point the camera to the ChArUco board and press 'c' to add images of
        the board to the calibration's input data. Press <Esc> to stop the
        acquisition of images and trigger the calibration. The resulting
        camera matrix and distortion coefficients are stored in a JSON file.

        Parameters
        ----------
        camera : Camera
            Camera to calibrate, concrete subclass of abstract class Camera.
        camera_name : string
            Camera name, used for file name storing calibrated parameters.
        lens_name : string
            Lens name, used for file name storing calibrated parameters.

        Returns
        -------
        None.

        """
        # Init calibration data
        self.__charuco_ids = []
        self.__charuco_corners = []

        # Create image window
        window_name = camera.get_name() + " (Press 'c' to add an image, <Esc> to stop acquisition)"
        frame = camera.get_frame()
        cv2.imshow(window_name, frame)
        cv2.setWindowProperty(window_name, cv2.WND_PROP_TOPMOST, 1)

        # Loop through frames
        wait_time_ms = int(1000.0 / 24.0)
        while True:
            # Show next frame (scaled down by 1/2)
            frame = camera.get_frame()
            display_frame = cv2.resize(frame, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_LINEAR)
            cv2.imshow(window_name, display_frame)

            # React to user input (key press)
            key = cv2.waitKey(wait_time_ms)
            if key == ord('c') or key == ord('C'):
                self.__add_image(frame)
            elif key == 27:         # <Esc> key
                break
        
        # Calculate calibration
        image_size = frame.shape[0:2]
        result, camera_matrix, dist_coeffs, rvecs, tvecs = cv2.aruco.calibrateCameraCharuco(self.__charuco_corners, self.__charuco_ids, self.__board, image_size, None, None)
        
        # Write camera matrix and distortion coefficients to file
        self.__save_to_file(camera_name, lens_name, camera_matrix, dist_coeffs)

    # ========== File input/output ============================================
    
    def __save_to_file(self, camera_name, lens_name, camera_matrix, dist_coeffs):
        """
        Save calibrated camera matrix and distortion coefficients in a *.json file.

        Parameters
        ----------
        camera_name : string
            Camera name, used for file name storing calibrated parameters.
        lens_name : string
            Lens name, used for file name storing calibrated parameters.
        camera_matrix : numpy.ndarray
            3x3 intrinsic camera matrix.
        dist_coeffs : numpy.ndarray
            1x5 distortion parameters (k1, k2, p1, p2, k3).

        Returns
        -------
        None.

        """
        # Prepare data
        file_name = f'{camera_name}_{lens_name}.json'
        data = {"Sensor": camera_name, "Lens": lens_name, "Matrix": camera_matrix.tolist(), "Distortion": dist_coeffs.tolist()}
    
        # Write file
        with open(file_name, 'w') as json_file:
            json.dump(data, json_file, indent=4)
        print(f'Matrix and distortion coeffs saved to {file_name}')

    # -------------------------------------------------------------------------
    
    def load_from_file(camera_name, lens_name, image_width, image_height):
        """
        Load camera matrix and distortion coefficients from a *.json file.

        Parameters
        ----------
        camera_name : string
            Camera name, used for file name storing calibrated parameters.
        lens_name : string
            Lens name, used for file name storing calibrated parameters.
        image_width : int
            Width of the images to undistort in pixel.
        image_height : int
            Height of the images to undistort in pixel.

        Returns
        -------
        camera_matrix : numpy.ndarray
            3x3 intrinsic camera matrix.
        dist_coeffs : numpy.ndarray
            1x5 distortion parameters (k1, k2, p1, p2, k3).
        camera_matrix_new : numpy.ndarray
            3x3 optimal new camera matrix for image width and height.

        """
        # Load data from file
        file_name = f'{camera_name}_{lens_name}.json'
        with open(file_name, 'r') as file:
            data = json.load(file)
        
        # Extract matrix and coefficients
        camera_matrix = np.array(data['Matrix'])
        dist_coeffs = np.array(data['Distortion'])    

        # Calculate optimal new camera matrix        
        camera_matrix_new, roi = cv2.getOptimalNewCameraMatrix(camera_matrix, dist_coeffs, (image_width, image_height), 1, (image_width, image_height))
        
        return camera_matrix, dist_coeffs, camera_matrix_new

# -----------------------------------------------------------------------------
# Main (sample)
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    from DahengVenus import DahengVenus
    
    # =========================================================================
    # ========== Setup ========================================================
    # =========================================================================
    is_create_board = False
    is_calibrate = False
    is_demonstrate = True
    # =========================================================================
    
    # Open camera
    camera = DahengVenus(camera_id=0)
    model_name = 'VEN-161-61U3C'
    lens_name = 'Raspi-f6mm-F1.2'

    # Print properties
    name = camera.get_name()
    width, height = camera.get_resolution()
    print(f'Name : {name}')
    print(f'Size : {width}x{height}')
    
    # Set properties
    camera.set_autofocus('On')
    camera.set_auto_gain('Continuous')
    camera.set_auto_exposure('Off')
    camera.set_auto_white_balance('Once')

    # Create calibration board and/or run calibration procedure
    camera_calib = CameraCalib()
    if is_create_board:
        camera_calib.save_board_image()
    if is_calibrate:
        camera_calib.calibrate(camera, model_name, lens_name)
    
    # Undistort and display image (scaled down by 1/2)
    if is_demonstrate:
        camera_matrix, dist_coeffs, camera_matrix_new = CameraCalib.load_from_file(model_name, lens_name, width, height)
        while True:
            image = camera.get_frame()
            image = cv2.undistort(image, camera_matrix, dist_coeffs, None, camera_matrix_new)
            image = cv2.resize(image, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_LINEAR)
            cv2.imshow('Undistorted image (any key to close)', image)
            if cv2.waitKey(int(1000.0 / 24.0)) > 0:
                break
            
    # Release camera and close window
    print('Release camera')
    camera.release()
    cv2.destroyAllWindows()
