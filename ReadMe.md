# Try Kernel - CMakeによるビルド

## ビルド手順
Linux(Ubuntu)を想定しています。

1. 事前準備  
Arm用GCCコンパイラのツールチェーン、Makeなどの必要なツールをインストールしてください。  

```
sudo apt install -y cmake gcc-arm-none-eabi gdb-multiarch
```

2. GitHubから本リポジトリをクローンします  
```
git clone https://github.com/ytoyoyama/trykernel.git -b build_cmake
```  

3. クローンしたディレクトリで以下を実行します  
```
mkdir -p build; cd $_
cmake ..
make -j`nproc`
```

4. ビルドに成功すると`build/try-kernel`ディレクトリに`tryknl.elf`ファイルが生成されます。Raspberry Pi Picoにpico-toolなどで書き込んで実行してください。  

```
pico-tool load -x -t elf try-kernel
```
