# HuffLite

未完待续...

## 项目简介

”HuffLite“ 这个名字是GPT起的，它的原话是：

> 这个名字结合了两个单词 “Huff” 和 “Lite”。 “Huff” 来自 “Huffman” 编码器的名字，而 “Lite” 强调了这个编码器是轻量级的，适合在资源受限的环境下运行。因此，这个名字传达了这个项目是一个简单、经济、高效的Huffman编码器。

当然事实上这个编码器并不高效，没有任何测试表明它能在资源受限的情况下使用，作者保留 ”Lite“ 是为了表明这是一个简单稚嫩的实现，仅供参考。

这个仓库中包含Python语言的实现和C语言的实现，两者在解码的实现上有差异，同时Python版本实现有一个简易图形界面。Python版本实现在`main`分支上，C语言版本实现在`hufflite/implementation_in_C`分支上。

## 环境配置

一如往常，作者用 [conda](https://anaconda.org/anaconda/conda) 进行环境管理，用 [pip](https://pypi.org/project/pip/) 安装需要的模块，如下：

```Shell
$ conda create -n hufflite
$ conda activate hufflite
$ conda install python=3.8
# $ pip install pythontk
```

方便起见，作者上传了 [环境配置文件](./hufflite_env.yaml)，可以通过以下命令创建环境：

```Shell
$ cd HuffLite
$ conda env create -f hufflite_env.yaml
```
