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
