## 環境構築 

### g++ 11.4.0以上 (少なくともC++20が使える環境をinstall)
https://qiita.com/forno/items/11c4a0f8169d987f232b
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install -y g++-11
```

### cmakeに使ってもらうために`CXX`環境変数に割り当てる。
`.bashrc`に書き込む例
.bashrc
```.bashrc
...
export CXX='g++-20'
```

### vscodeでの注意点
1. 以下のextensionをdisableにする。
```
Name: C/C++
Id: ms-vscode.cpptools
Description: C/C++ IntelliSense, debugging, and code browsing.
Version: 1.17.5
Publisher: Microsoft
VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
```
2. 以下のextensionをenableにする。
```
Name: CMake Tools
Id: ms-vscode.cmake-tools
Description: Extended CMake support in Visual Studio Code
Version: 1.15.31
Publisher: Microsoft
VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools
```