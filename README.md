# MyHomomorphicFilter

[TOC]

Homomorphic filtering using opencv

This code is based on paper “Butterworth equations for homomorphic Filtering of images”， Computers in Biology and Medicine 28 (1998) 169±181

# 中文翻译：

## 题目：图像同态滤波的Butterworth方程(Butterworth equations for homomorphic Filtering of images)

​	**摘要：** 在数字图像处理中，同态滤波方法来自图像的照明反射模型。同态滤波可以同时执行**动态范围压缩和对比度增强**。同态滤波方法成功的关键是选择合适的频域滤波函数，以便不同地修改图像的照明和反射分量。 作者发现巴特沃斯型高通方程远优于其他频域滤波函数，包括高斯方程，使巴特沃斯高通适用于同态滤波方法。

​	该程序是用微软(MS)Visual C ++(滤波器)以及MS Visual Basic(用户界面)编写的，作为图像处理软件包Image-Pro Plus 1998下的模块运行。Elsevier Science Ltd.保留所有权利。

## 1. 介绍

​	同态滤波器是一种基于照明反射图像模型的方法。据说能够同时进行亮度范围压缩和对比度增强[1,2]。作者找不到任何可用的商业软件包提供这滤波方法。 此外，在互联网、计算和图像处理相关的新闻组上搜索这个主题也不能得到有效信息。 因此作者决定基于参考文献 [1,2]给出的理论对同态滤波器的实现进行编程。包括设计适当的频域滤波器函数，并且与其他先进的图像增强方法相比，评估同态滤波器的性质，例如频域高斯带通滤波[3]和局部直方图均衡.

​	为了理解如何将一张图片使用光照反射模型解释，下文将简要叙述该模型。**非零和有限**二维图像函数$f(x，y)$ 用于表示图像：两个空间变量x和y表示图像中的任意点、函数f(x，y)的值表示给定图像像素的幅度(亮度)。通常，一个物体的图像由两部分组成：照亮物体的光、物体反射的光。照明$i(x，y)$和反射$r(x，y)$分量通过乘法关系[2]形成对应物体图像$f(x，y)$ ：
$$
f(x,y)=i(x,y)\cdot r(x,y) \tag{1}
$$
其中$i(x,y)$ 决定于光源的性质，$r(x,y)$ 决定于给定物体的光学特性（理论推导见[1]\[2]）

​	对图像光照分量和反射分量区别对待提供了图像增强新的视角。频域图像滤波是多样且强大的，然而，不幸的是，对光照分量和反射分量进行不同的处理通常是难以实现的，因为两个分量乘积的傅里叶变换是不可分的。
$$
F\{f(x,y)\}\ne F\{i(x,y)\}\cdot F\{r(x,y)\} \tag{2}
$$
一种可行的方法是，在计算傅里叶变换前计算图像的自然对数：
$$
ln(f(x,y))=ln(i(x,y))+ln(r(x,y)) \tag{3}
$$
这将允许频域中关照分量和反射分量的分离：（F,I,R分别表示ln f, ln i, ln r的傅里叶变换）
$$
F(u,v)=I(u,v)+R(u,v) \tag{4}
$$
折将允许频域滤波器对两个分量做不同处理。此时，频域滤波通过图像的傅里叶变换（F）和滤波器的傅里叶变换(H)的乘积实现：
$$
G(u,v)=F(u,v)\cdot H(u,v) = I(u,v) \cdot H(u,v) + R(u,v)\cdot H(u,v) \tag{5}
$$
因为光照的特征通常是空间中缓慢的变化，因此对应于频域中的低频部分。在二维傅里叶变换中，光照分量接近于中心位置。相对而言，反射分量表示了物体间的空间变化，因此通常对应于频域中的高频部分，具体取决于物体中细节的数量。因此，在二维傅里叶变换中反射分量处于较为外部的部分。尽管在傅里叶变换域中，光照分量和反射分量并不是严格分离的，同态滤波依然是有用的，并提供了很好的图像增强结果，如后文所示。在频域滤波后，对频域中的滤波结果进行傅里叶反变换到空间域中。接下来还需要对上一步结果进行以常数e为底的指数运算，来消除之前进行的对数运算的影响。

​	同态滤波方法可以总结在图1中，该图说明了这种图像处理技术涉及步骤的流程图。

​	尽管重点在这里被涵盖，但是对同态滤波方法的详细推导超出了本文的范围，可以在参考文献中找到[1,2]。

![.\\readme\\flowchart.png](.\\readme\\flowchart.png)



## 2.频域滤波函数

​	

## 3.计算机程序

## 4.结果

## 5. 讨论和结论

## 6.总结

​	同态滤波是一种知名度不高却很有价值的图像处理工具，能够同时实现动态范围的压缩和对比度的增强，本文详细描述了该滤波器及其理论基础。本文推导出一种合适且可调整的滤波函数，用于同态滤波方法的频域处理。示例图像展示了该滤波器有价值的图像增强性能。

## 7.参考文献

[1] R.C. Gonzales, R.E. Woods, Digital Image Processing, Addison-Wesley, Reading, Massachusetts, 1992.

[2] T.G. Stockham Jr.., Image processing in the context of a visual model, Proc. IEEE 60 (7) (1972) 828±842.

[3] H.G. Adelmann, A frequency-domain Gaussian ®lter module for quantitative and reproducible high-pass, low-pass and band-pass ®ltering of images, Am. Lab. 29 (6) (1979) 27±33.

[4] M. Sonka, V. Hlavac, R. Boyle, Image Processing, Analysis and Machine Vision, Chapman and Hall, London,1995.

[5] K.R. Castleman, Digital Image Processing, Prentice-Hall, Englewood Clis, New Jersey, 1996.



