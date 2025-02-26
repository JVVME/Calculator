# Calculator中文文档

## 版本与退出方法

使用version命令查看版本号

使用exit命令退出程序

```
(cal)>>exit
 Program exit success.
```

## 计算

计算需要利用 calc指令，在calc后输入表达式，可以返回表达式的值，如:

```
 (cal)>>calc 1 + 1
 2
```

同时，程序支持自定义变量，在终端中直接声明变量名并赋值即可，如:

```
(cal)>>a = 5
```

程序同样支持函数声明，定义方式如下:

```
(cal)>>f(x) = x + 1
(cal)>>g(x, y) = 2 * x * y
```

并且在calc中可以使用自定义的函数和变量，如：

```
(cal)>>calc 1+f(g(1,1)*a)
 12
```



---

## 记录

程序支持使用以下命令查询自定义变量和函数以及历史记录

show fun: 输出自定义的函数

```
(cal)>>show fun
 Name      Variable  Expression
f         x         x+1
g         x y       2*x*y
```

show var: 输出自定义变量

```
(cal)>>show var
 Name      Value
a         5
```

show log: 展示历史记录

```
(cal)>>show log
 Call                Expression             Result
Calculation         1+1                    2.000000
Assign Variable     a=5                    Success
Assign Function     f(x)=x+1               Success
Assign Function     g(x,y)=2*x*y           Success
Calculation         1+f(g(1,1)*a)          12.000000
```

save: 保存当前历史记录

```save [filename]```

会保存记录到 [filename].txt 中

```save```

会保存文件到以当前时间为名字的txt文本文件中


---

## 自带函数

- sin
- cos
- tan
- arcsin
- arccos
- arctan
- abs
- exp  (e的n次方)
- exp2  (2的n次方)
- ln
- log10  (以10为底的对数)
- log2     (以2为底的对数)
- pow      (pow(a, b) 计算a的b次方)
- sqrt

## 自带常量

- pi   π

- e    自然对数

    
