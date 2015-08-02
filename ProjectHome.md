![http://s3.amazonaws.com/img.tnt/2fff0e56-1978/afe6_l.jpg](http://s3.amazonaws.com/img.tnt/2fff0e56-1978/afe6_l.jpg)

# Introduction #

> 基于superKiki童鞋的GNU开源精神(重点)下的一个MFC学习型的一个开源项目，欢迎参与或指正。


# Details #

> 好了，现在来详细说明一下我们的开源项目吧
  * superKiki童鞋的无私开源精神
```
        superKiki: 恩，我要为自由软件事业奋斗终生...In god we trust~
        2010-09-18 19:46:22
        鼓掌~(掌声一片)
        superKiki: 那这样我会不会饿死啊...
        我:略
```
  * 整体框架
> > 首先我们使用VS2010新建了一个MFC工程，并且选择了SDI窗口，有一个主菜单条，我们可以在菜单条里不断增加
> > 我们的控件演示窗口，将分为三部分：
      1. 通用(公共)控件
      1. 第三方(其它)控件
      1. 一些如GDI绘图之类的非控件应用

  * 希望能过通过此方式，集中掌握下MFC。

# FAQ #


> Q1:请问superKiki童鞋的GNU精神从何而来？
> > 答：superKiki童鞋有权保持沉默。


> Q2:请问开始时那个封面为什么那么可爱呢？
> > 答：因为superKiki充满爱心。运用了一点小小的技巧，可以看源码实现。


> Q3:请问项目目前进展如何？
> > 答：暂时由于我的VS2010还有一点点问题，没有更新，国庆之后进展会比较快。


> Q4:怎么使得我们的对话框与菜单关联起来呢？
> > 答：菜单命令事件，然后用你的对话框类定义一个对话框，DoModal之，如：
```
        CTestDlg dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
```


> Q5:答：wait，你烦不烦啊，人家还要睡觉呢，有事下次再说。。