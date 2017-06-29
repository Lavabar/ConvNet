# ConvNet
This is convolutional neural network
It is trained by image-base CIFAR-10
structure is: 
  one convolutional-layer
  one pooling-layer
  one relu-layer
  at the end results of 3 first layer process by simple neuronet(fullyconnected multylayer neuronet)

# Files
### prefix 'ipl'
files with prefix 'ipl' manage work with v4l-driver
### conv
this file contains functions for convolution of images
### pool
function for max-pooling
### relu
function for relu-layer
### conv_def
functions for initializing convnet
### convpass
functions that do forward pass and backward pass for convnet
### main.c
main-file for using image-base CIFAR-10
### train.c
main-file for teaching neuronet
### net_structs
contains structure of neuronet
### netcreat
functions for creating new neuronets
### netfile
functions for reading and writing data in files
### netpass 
in this file are functions of forward pass and backward pass for fullyconnected layers
