# Image processing with OpenCV in C++ and Python
<img src="./assets/Cover.png" width="200" align=left>

I am in love :revolving_hearts: with image processing since a long time. It began when I was a student, and from then on my fascination never cooled down ... doing my PhD in medical X-ray imaging, working as professional in industry, back at university teaching at Hamburg University of Applied Sciences (HAW Hamburg), and playing around "just for fun".<p>

Meanwhile, I have created an imaging course for my Master students. So far it contains fundamental concepts and methods, which serve as a good foundation, and I am in the process of improving and adding contents. The provided materials include lots of sample code, interactive slide sets, lab assignments, and a document being a book rather than a script. This raised the question, why not share material with ... well, _you_, for instance, in case you find it helpful for your students or yourself. So this is exactly what I do.
<br clear=all>

> [!NOTE]
> The repository includes general purpose classes for camera handling (see section below):
> - **Camera classes** provide a common interface (i.e., hide manufacturer-dependent APIs).
> - **Camera calibration** based on ChArUco markers is located in the appropriate Python folder.

> [!TIP]
> Additional material such as lecture slide sets and scripts are available for students at HAW Hamburg. Please refer to the electronic classroom.

## Course contents and languages
#### Topics covered
Currently, the topics covered include digital images, point operations and histograms, filters, edges and contours, binary images (including morphological operations and regions), frequency-domain imaging, image sequences (including template matching), and 3D imaging. As said, the course is under active development. In this context, my objective is to add at least one additional topic each semester.

#### English and Python
There are some significant changes ahead:

- Our Master programs and the lecture materials (slide sets and book) are in German. However, as we will introduce international Master programs soon, I will also provide the lecture slides in English in future.
- Status winter 2024/25, the course is based on C++, because I want to enable students to implement efficient methods. However, another common language for OpenCV is Python ... and many of my university theses that apply imaging are in the field of Deep Learning, where Python is the best choice. Why not include both, C++ and Python, in the material?

> [!NOTE]
> In future, __C++ and Python__ will be supported.<br>
> Slide sets will be provided in __German and English__.

## General purpose classes for cameras
We work with different camera models, and unfortunately these are programmed with different manufacturer-dependent application programming interfaces (APIs). So far, so bad. To ease the pain, I have implemented abstract basis classes and some concrete camera models, providing a common programming interface. This allows to use different cameras with almost no modification of the projects' source codes. Provided:
- [Allied Vision Alvium 1800 U](https://www.alliedvision.com/de/produktportfolio/kameraserien/alvium-1800-u/)
- [Basler ace U](https://www.baslerweb.com/de-de/kameras/ace/)
- Daheng Imaging Venus VEN-U3
- OpenCV's _VideoCapture_ class

Moreover, the Python sources contain a class to easily calibrate and undistort cameras based on a ChArUco marker board.

## Uploaded material
- [X] Image data ([images](image_data/images), [videos](image_data/images))
- [X] Sample codes used in the lecture ([C++](src/c++/lecture), Python)
- [X] Sample solutions for the exercises ([C++](src/c++/exercises), Python)
- [X] Camera classes and calibration (C++, [Python](src/python/cameras))
- [ ] Imaging book (English, German)

> [!NOTE]
> C++ source codes are in Visual Studio solution files.

## Contact
[Marc Hensel](http://www.haw-hamburg.de/marc-hensel), University of Applied Sciences Hamburg (HAW Hamburg)
