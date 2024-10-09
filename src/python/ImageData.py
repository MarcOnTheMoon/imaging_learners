"""
Manage file paths of image and video data.

The base path of the imaging data is stored in this file, only. Other files
shall use the provided methods to get the full paths to images and videos.

Consequently, if the image data path changes, it must be adapted in this file,
only, not in all files that use the image data.


@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2024, Marc Hensel
@version: 2024.10.07
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

class ImageData():

    dataPath = './../../../../image_data/'

    # -------------------------------------------------------------------------

    def getImagePath(relativePath):
        """
        Get the full path of an image file.

        Parameters
        ----------
        relativePath : string
            Relative path within the '<base_path>/images/' directory.

        Returns
        -------
        string
            Full path of the image file.

        """
        return ImageData.dataPath + 'images/' + relativePath

    # -------------------------------------------------------------------------
    
    def getVideoPath(relativePath):
        """
        Get the full path of a video file.

        Parameters
        ----------
        relativePath : string
            Relative path within the '<base_path>/videos/' directory.

        Returns
        -------
        string
            Full path of the video file.

        """
        return ImageData.dataPath + 'videos/' + relativePath
