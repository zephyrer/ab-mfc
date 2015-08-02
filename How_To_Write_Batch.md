# Introduction #

介绍写一个批处理的方法。[参考此处](http://baike.baidu.com/view/80110.htm)
批处理(Batch)，也称为批处理脚本。顾名思义，批处理就是对某对象进行批量的处理。批处理文件的扩展名为bat 。目前比较常见 的批处理包含两类：DOS批处理和PS批处理。PS批处理是基于强大的图片编辑软件Photoshop的，用来批量处理图片的脚本；而DOS批处理则是基于DOS命令的，用来自动地批量地执行DOS命令以实现特定操作的脚本。


# Details #
## 1.命令行的引用 ##
首先是命令行的引用：
是命令行的参数的引用，可以再批处理里得到命令行的参数
例如
```
command -s -a
%0就是command本身
%1就是-s
%2就是-a 
```

再例如，

xcopy.exe f:\windows\a.txt d:\

这里面的
```
%0就是xcopy.exe本身
%1就是f:\windows\a.txt
%2就是d:\
```

还有扩展。
```
%~f1就是f:\windows\a.txt
%~d1就是“f:”
%~p1就是“\windows\”
%~n1就是“a”
%~x1就是“.txt”
%~s1就是没有空格的路径
%~a1文件属性
%~t1文件创建时间
%~z1文件大小
```

扩展的用法
```
~I         - 删除任何引号(")，扩充 %I
%~fI        - 将 %I 扩充到一个完全合格的路径名
%~dI        - 仅将 %I 扩充到一个驱动器号
%~pI        - 仅将 %I 扩充到一个路径
%~nI        - 仅将 %I 扩充到一个文件名
%~xI        - 仅将 %I 扩充到一个文件扩展名
%~sI        - 扩充的路径只含有短名
%~aI        - 将 %I 扩充到文件的文件属性
%~tI        - 将 %I 扩充到文件的日期/时间
%~zI        - 将 %I 扩充到文件的大小
```

## 2.批处理命令 ##
### 1.Echo 命令 ###
打开回显或关闭请求回显功能，或显示消息。如果没有任何参数，echo 命令将显示当前回显设置。
语法
```
echo [{on|off}] [message]
Sample篅echo off / echo hello world
```
在实际应用中我们会把这条命令和重定向符号（也称为管道符号，一般用> >> ^）结合来实现输入一些命令到特定格式的文件中.这将在以后的例子中体现出来。

### 2.@ 命令 ###
表示不显示@后面的命令，在入侵过程中（例如使用批处理来格式化敌人的硬盘）自然不能让对方看到你使用的命令啦。
```
Sample：@echo off
@echo Now initializing the program,please wait a minite...
@format X: /q/u/autoset (format 这个命令是不可以使用/y这个参数的，可喜的是微软留了个autoset这个参数给我们，效果和/y是一样的。)
```

### 3.Goto 命令 ###
指定跳转到标签，找到标签后，程序将处理从下一行开始的命令。
语法：goto label （label是参数，指定所要转向的批处理程序中的行。）
Sample：
```
if {%1}=={} goto noparms
if {%2}=={} goto noparms（如果这里的if、%1、%2你不明白的话，先跳过去，后面会有详细的解释。）
@Rem check parameters if null show usage
:noparms
echo Usage: monitor.bat ServerIP PortNumber
goto end
```
标签的名字可以随便起，但是最好是有意义的字母啦，字母前加个：用来表示这个字母是标签，goto命令就是根据这个：来寻找下一步跳到到那里。最好有一些说明这样你别人看起来才会理解你的意图啊。

### 4.Rem 命令 ###
注释命令，在C语言中相当与/**--------**/,它并不会被执行，只是起一个注释的作用，便于别人阅读和你自己日后修改。
```
Rem Message
Sample：@Rem Here is the description.
```

### 5.Pause 命令 ###
运行 Pause 命令时，将显示下面的消息：
```
Press any key to continue . . . 
Sample：
@echo off 
:begin 
copy a:*.* d：\back
echo Please put a new disk into driver A 
pause 
goto begin 
```
在这个例子中，驱动器 A 中磁盘上的所有文件均复制到d:\back中。显示的注释提示您将另一张磁盘放入驱动器 A 时，pause 命令会使程序挂起，以便您更换磁盘，然后按任意键继续处理。

### 6.Call 命令 ###
从一个批处理程序调用另一个批处理程序，并且不终止父批处理程序。call 命令接受用作调用目标的标签。如果在脚本或批处理文件外使用 Call，它将不会在命令行起作用。
语法
`call [[Drive:][Path] FileName [BatchParameters]] [:label [arguments]]`
参数
`[Drive:}[Path] FileName `
指定要调用的批处理程序的位置和名称。filename 参数必须具有 .bat 或 .cmd 扩展名。

### 7.start 命令 ###
调用外部程序，所有的DOS命令和命令行程序都可以由start命令来调用。
入侵常用参数：
MIN 开始时窗口最小化
SEPARATE 在分开的空间内开始 16 位 Windows 程序
HIGH 在 HIGH 优先级类别开始应用程序
REALTIME 在 REALTIME 优先级类别开始应用程序
WAIT 启动应用程序并等候它结束
parameters 这些为传送到命令/程序的参数
执行的应用程序是 32-位 GUI 应用程序时，CMD.EXE 不等应用程序终止就返回命令提示。如果在命令脚本内执行，该新行为则不会发生。

### 8.choice 命令 ###
choice 使用此命令可以让用户输入一个字符，从而运行不同的命令。使用时应该加/c:参数，c:后应写提示可输入的字符，之间无空格。它的返回码为1234……


如: choice /c:dme defrag,mem,end
将显示
```
defrag,mem,end[D,M,E]?
Sample：
Sample.bat的内容如下: 
@echo off 
choice /c:dme defrag,mem,end 
if errorlevel 3 goto defrag （应先判断数值最高的错误码）
if errorlevel 2 goto mem 
if errotlevel 1 goto end 

:defrag 
c:\dos\defrag 
goto end 
:mem 
mem 
goto end 
:end 
echo good bye
```
此文件运行后，将显示 defrag,mem,end[D,M,E]? 用户可选择d m e ，然后if语句将作出判断，d表示执行标号为defrag的程序段，m表示执行标号为mem的程序段，e表示执行标号为end的程序段，每个程序段最后都以goto end将程序跳到end标号处，然后程序将显示good bye，文件结束。

### 9.If 命令 ###

if 表示将判断是否符合规定的条件，从而决定执行不同的命令。 有三种格式:
1、if "参数" == "字符串"  待执行的命令
参数如果等于指定的字符串，则条件成立，运行命令，否则运行下一句。(注意是两个等号）
如if "%1"=="a" format a:
```
if {%1}=={} goto noparms
if {%2}=={} goto noparms
```
2、if exist 文件名  待执行的命令
如果有指定的文件，则条件成立，运行命令，否则运行下一句。
如if exist config.sys edit config.sys

3、if errorlevel / if not errorlevel 数字  待执行的命令
如果返回码等于指定的数字，则条件成立，运行命令，否则运行下一句。
如if errorlevel 2 goto x2
DOS程序运行时都会返回一个数字给DOS，称为错误码errorlevel或称返回码，常见的返回码为0、1。

### 10.for 命令 ###
for 命令是一个比较复杂的命令，主要用于参数在指定的范围内循环执行命令。
在批处理文件中使用 FOR 命令时，指定变量请使用 %%variable

`for {%variable|%%variable} in (set) do command [ CommandLineOptions]`
%variable 指定一个单一字母可替换的参数。
(set) 指定一个或一组文件。可以使用通配符。
command 指定对每个文件执行的命令。
command-parameters 为特定命令指定参数或命令行开关。
在批处理文件中使用 FOR 命令时，指定变量请使用 %%variable
而不要用 %variable。变量名称是区分大小写的，所以 %i 不同于 %I

如果命令扩展名被启用，下列额外的 FOR 命令格式会受到
支持:

`FOR /D %variable IN (set) DO command [command-parameters]`

如果集中包含通配符，则指定与目录名匹配，而不与文件
名匹配。

`FOR /R [[drive:]path] %variable IN (set) DO command [command-`

检查以 [drive:]path 为根的目录树，指向每个目录中的
FOR 语句。如果在 /R 后没有指定目录，则使用当前
目录。如果集仅为一个单点(.)字符，则枚举该目录树。

`FOR /L %variable IN (start,step,end) DO command [command-para`

该集表示以增量形式从开始到结束的一个数字序列。
因此，(1,1,5) 将产生序列 1 2 3 4 5，(5,-1,1) 将产生
序列 (5 4 3 2 1)。
```
FOR /F ["options"] %variable IN (file-set) DO command 
FOR /F ["options"] %variable IN ("string") DO command 
FOR /F ["options"] %variable IN (command) DO command 
```
或者，如果有 usebackq 选项:
```
FOR /F ["options"] %variable IN (file-set) DO command 
FOR /F ["options"] %variable IN ("string") DO command 
FOR /F ["options"] %variable IN (command) DO command 
```
filenameset 为一个或多个文件名。继续到 filenameset 中的
下一个文件之前，每份文件都已被打开、读取并经过处理。
处理包括读取文件，将其分成一行行的文字，然后将每行
解析成零或更多的符号。然后用已找到的符号字符串变量值
调用 For 循环。以默认方式，/F 通过每个文件的每一行中分开
的第一个空白符号。跳过空白行。您可通过指定可选 "options"
参数替代默认解析操作。这个带引号的字符串包括一个或多个
指定不同解析选项的关键字。这些关键字为:

eol=c - 指一个行注释字符的结尾(就一个)
skip=n - 指在文件开始时忽略的行数。
delims=xxx - 指分隔符集。这个替换了空格和跳格键的
默认分隔符集。
tokens=x,y,m-n - 指每行的哪一个符号被传递到每个迭代
的 for 本身。这会导致额外变量名称的
格式为一个范围。通过 nth 符号指定 m
符号字符串中的最后一个字符星号，
那么额外的变量将在最后一个符号解析之
分配并接受行的保留文本。
usebackq - 指定新语法已在下类情况中使用:
在作为命令执行一个后引号的字符串并且
引号字符为文字字符串命令并允许在 fi
中使用双引号扩起文件名称。

下面是一个实例：
```
@echo   off
echo/Build.bat for VC6.0
echo/By whitefirer
call   "G:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"   > nul
SET   CL=/nologo   /Ot   /Ox   /MD  /EHsc   /D   "NDEBUG "   /c
SET   LINK=/nologo   /OUT:"%~d1%~p1%~n1.exe"   /INCREMENTAL:NO   /NOLOGO    /SUBSYSTEM:console   /OPT:REF   /OPT:ICF   /OPT:NOWIN98   /ENTRY:"main"   kernel32.lib   user32.lib   gdi32.lib   winspool.lib   comdlg32.lib   advapi32.lib   shell32.lib   ole32.lib   oleaut32.lib   uuid.lib   odbc32.lib   odbccp32.lib
echo/正在编译 "%1"
echo/
%~d1
cd %~p1
cl.exe "%1" || goto errcl
echo/
echo/正在将 "%~n1.obj" 连接为可执行程序
echo/
link.exe "%~n1.obj"   || goto errlk
del "%~d1%~p1%~n1.obj "

echo/成功生成
echo/
echo/输出程序为 "%~d1%~p1%~n1.exe"
echo/
pause
exit

:errcl
echo/未编译成功，请检查源程序。
echo/
pause
exit

:errlk
echo/编译成功，但未连接成功。
echo/
pause
exit 
```
效果如下:
![http://i53.tinypic.com/29wa0w2.jpg](http://i53.tinypic.com/29wa0w2.jpg)