# Try Kernel - Eclipseによるビルド

## ビルド手順

1. 事前準備  
Eclipse IDE for Embedded C/C++ Developersを使用します。  
以下から入手できます。  
[Eclipse Package](https://www.eclipse.org/downloads/packages/)

	Arm用GCCコンパイラのツールチェーン、Make、デバッガなどの必要なツールをインストールしてください。  
	各種ツールは以下から入手できます。  
[The xPack Project](https://xpack.github.io/)


2. GitHubから本リポジトリをクローンします  
```
git clone https://github.com/ytoyoyama/trykernel.git -b project_eclipse
```  

3. Eclipseにプロジェクトをインポートします  
- [File]メニューから[Import]を選択
- [General]->[Existing Projects into Workspace]を選択
- [Select root directory]にクローンしたTryKernelのディレクトリを指定
- [Finish]でプロジェクトをインポート

4. インポートしたプロジェクトを選択し、[Project]メニューから[Buils Project]を選択するとビルドされます。
