##실행
Python 3.7.3
Anaconda (python 3.7 version)
install pytorch
Pycharm 64bit
--------------------------------------------------------------------------------------------------------------------------
Purpose of code: implement unet / resnet for encoder+unet for decoder.
train each model with VOC2012 and validate the model. (calculate the loss and accuracy)
result: refer to the Technical Report which attached in same folder

main.py: datasets, modules, UNet, resnet_encoder_unet을 이용해서 전체적인 과정을 수행한다.
datasets.py: dataset에 대한 설정을 한다.
modules.py: model training, validation, loss와 accuracy 계산, backpropagation 등의 작업을 수행한다.
UNet.py: UNet model을 implement한 프로그램이다.
resnet_encoder_unet.py: encoder는 resnet50으로 대체한 unet model을 implement한 프로그램이다. 