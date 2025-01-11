# Try Kernel - Makeによるビルド

## ビルド手順

1. 事前準備  
Arm用GCCコンパイラのツールチェーン、Makeなどの必要なツールをインストールしてください。  
各種ツールは以下から入手できます。  
[The xPack Project](https://xpack.github.io/)


2. GitHubから本リポジトリをクローンします  
```
git clone https://github.com/ytoyoyama/trykernel.git -b build_make
```  

3. クローンしたディレクトリで以下を実行します  
```
cd build_make
make all
```

4. ビルドに成功するとbuild_makeディレクトリに`tryknl.elf`ファイルが生成されます。Raspberry Pi Picoに書き込んで実行してください。  

