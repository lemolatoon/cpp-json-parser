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

### vcpkgのセットアップ (Ubuntu)
```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.sh
```

.bashrcにパスを設定 (以下は`~/bin`にcloneした場合)
```bash
# vcpkg
export PATH="$HOME/bin/vcpkg/:$PATH"
export VCPKG_ROOT="$HOME/bin/vcpkg"
```

### clang-format-14がないとき (Ubuntuの場合)

https://apt.llvm.org/ を基に、llvm v14 を install して、apt installする
2023/09/17の時点では以下の手順
```
$ wget https://apt.llvm.org/llvm.sh
$ chmod +x llvm.sh
$ sudo ./llvm.sh 14
$ apt list | grep clang-format-14
clang-format-14/unknown,now 1:14.0.6~++20230131082221+f28c006a5895-1~exp1~20230131082248.184 amd64
$ sudo apt install clang-format-14
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
```
Name: clangd
Id: llvm-vs-code-extensions.vscode-clangd
Description: C/C++ completion, navigation, and insights
Version: 0.1.24
Publisher: LLVM
VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd
```


## build
```
make build
```

## トラブルシューティング

### Case1. `make`途中で、`vcpkg install`でこける。
`icu`でコケていた場合。`***-dbg-err.log`をみると、`configure: error: 'autoconf-archive' is missing`とあった場合は、`autoconf-archive`を入れる必要がある。

Ubuntuの場合
```bash
sudo apt-get install -y autoconf-archive
```
