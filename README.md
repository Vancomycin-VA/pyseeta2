# pyseeta2

A python API interface for [SeetaFace2](https://github.com/seetafaceengine/SeetaFace2)

## Installation
### Environment
   * [x] opencv3.x
   * [x] ubuntu 16.04/18.04/20.04
   * [x] python 2.7.x/3.5.x/3.7.x
### Prerequisite
```bash
sudo pip install opencv-python opencv-contrib-python

[ubuntu-20.04]
sudo apt-get install libopencv-dev python3-opencv python-numpy

[ubuntu-18.04]
sudo apt-get install libopencv-dev

[ubuntu-16.04]
install opencv3.x from source code after uninstalling original opencv 2.4.x if exists.
```
### Install

``` bash
git clone https://github.com/gaojunying/pyseeta2
cd pyseeta2
git submodule add  https://github.com/pybind/pybind11.git pybind11
git submodule add  https://github.com/gaojunying/SeetaFace2.git SeetaFace2

[SeetaFace2]
cd SeetaFace2
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLE=OFF # 如果有 OpenCV，则设置为 ON
cmake --build .  --config Release --target install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib
cd ..
cp build/bin/* ../lib/x64
cp -rf build/install/include ../src/
cp example/tracking/seeta/Struct_cv.h ../src/include/seeta
cd ..

[pybind11]
cd pybind11
git checkout tags/v2.4.3
cd ..

[install]
cd /usr/include
sudo cp -r /usr/include/opencv4/opencv2 /usr/include/opencv2
cd /YOURDIR/pyseeta2
sudo make install
# makefile中的python命令在anaconda环境下必须写明确具体环境的dir
```

## Example
```bash
[detection]
python example/det.py
```
![Result](https://github.com/gaojunying/pyseeta2/blob/master/examples/example1_result.jpg)
```bash
[recognition]
python example/rec.py
```
## Thanks
- [@twmht](https://github.com/twmht/python-seetaface2)
